#pragma once

#include <string>

namespace task01 {

// Полезные данные одной записи варианта 13.
struct BankAccount {
  int accountNumber{};
  std::string fullName;
  std::string address;
};

} // namespace task01
