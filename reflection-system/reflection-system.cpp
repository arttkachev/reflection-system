// reflection-system.cpp
#include <iostream>
#include "reflection-helpers.h"
#include "aircraft.h"
int main() {
  Aircraft const Airbus{ "Airbus", "A320", 2001 };
  Aircraft const Boeing{ "Boeing", "B737", 2009 };
  std::cout << GetType(Airbus);
  std::cout << std::endl;
  std::cout << Airbus;
  int exitCode{};
  while (exitCode != 1) {
    std::cin >> exitCode;
  }
}