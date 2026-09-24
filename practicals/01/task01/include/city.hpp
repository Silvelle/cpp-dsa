#pragma once

#include <cstddef>

namespace task01 {

inline constexpr std::size_t kCityNameSize = 40;

struct City {
  int code;
  char name[kCityNameSize]{};
};

} // namespace task01
