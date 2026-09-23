#pragma once

#include <cstddef>

namespace dsa {
struct OperationCounter {
  std::size_t movements = 0;
  std::size_t comparison = 0;
};
} // namespace dsa
