#pragma once
#include <any>
#include <string>
#include <tuple>
#include <vector>
#define GET_VARIABLE_NAME(Variable) (#Variable)
class Aircraft {
public:
  Aircraft(std::string&& manufacturer, std::string&& model, int const year)
    : Manufacturer{ std::move(manufacturer) }, Model{ std::move(model) }, Year{ year } {}
  [[nodiscard]] auto ReflectValues() const { return std::tie(Manufacturer, Model, Year); }
  [[nodiscard]] auto ReflectMemberNames() const { return std::tie(GET_VARIABLE_NAME(Manufacturer), GET_VARIABLE_NAME(Model), GET_VARIABLE_NAME(Year)); }
private:
  std::string Manufacturer{};
  std::string Model{};
  int Year{};
};