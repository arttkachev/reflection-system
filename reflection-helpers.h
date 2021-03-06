#pragma once
#include <ostream>
#include <type_traits>
#include <string>

// helper functions
template <typename T>
static auto GetType(T const& val) {
  return typeid(val).name();
}

// types
template <typename T>
struct remove_const_ref {
  using type = std::remove_reference_t<std::remove_const_t<T>>;
};

template <typename Type>
constexpr static auto type_v = type_v<typename remove_const_ref<Type>::type>;

template <>
constexpr static auto type_v<int> = "int";

template <>
constexpr static auto type_v<std::string> = "string";

template <>
constexpr static auto type_v<float> = "float";

// tuple helpers
template <typename Tuple, typename Functor, size_t Index = 0>
auto tuple_for_each(Tuple const& tpl, Functor const& f)->void {
  constexpr auto tuple_size = std::tuple_size_v<Tuple>;
  if constexpr (Index < tuple_size) {
    f(std::get<Index>(tpl));
    tuple_for_each<Tuple, Functor, Index + 1>(tpl, f);
  }
}

template <typename Members, typename Values, typename Functor, size_t Index = 0>
auto reflect(Members const& members, Values const& values, Functor const& f)->void {
  constexpr auto tuple_size = std::tuple_size_v<Members>;
  if constexpr (Index < tuple_size) {
    f(std::get<Index>(members), std::get<Index>(values));
    reflect<Members, Values, Functor, Index + 1>(members, values, f);
  }
}
// detector
struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const&) = delete;
  void operator=(nonesuch const&) = delete;
};
namespace detail {
  template <class Default, class AlwaysVoid, template<class...> class Op, class... Args>
  struct detector {
    using value_t = std::false_type;
    using type = Default;
  };
  template <class Default, template<class...> class Op, class... Args>
  struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
  };
} // namespace detail

template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template <typename T>
using has_reflect_member = decltype(&T::ReflectValues);

template<typename T>
constexpr bool is_reflectable_v = is_detected<has_reflect_member, T>::value;

// operator overloading for reflectable objects
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator==(T const& a, T const& b)-> std::enable_if_t<IsReflectable, bool> {
  return a.ReflectValues() == b.ReflectValues();
}
template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator!=(T const& a, T const& b)-> std::enable_if_t<IsReflectable, bool> {
  return a.ReflectValues() != b.ReflectValues();
}

template <typename T, bool IsReflectable = is_reflectable_v<T>>
auto operator<<(std::ostream& ostr, T const& v)-> std::enable_if_t<IsReflectable, std::ostream&> {
  reflect(v.ReflectMemberNames(), v.ReflectValues(), [&ostr](auto const& m, auto const& v) {
    ostr << type_v<decltype(v)> << " " << m << " " << v << std::endl;
    });
  return ostr;
}