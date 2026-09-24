#pragma once

#include <chrono>
#include <cstddef>
#include <utility>

namespace task01 {

struct OperationCounter {
  std::size_t comparisons = 0;
  std::size_t movements = 0;

  [[nodiscard]] std::size_t total() const {
    return comparisons + movements;
  }
};

template <typename Function>
double measureMilliseconds(Function &&function) {
  const auto startedAt = std::chrono::steady_clock::now();
  std::forward<Function>(function)();
  const auto finishedAt = std::chrono::steady_clock::now();

  return std::chrono::duration<double, std::milli>(finishedAt - startedAt)
      .count();
}

} // namespace task01
