#pragma once

#include <chrono>
#include <functional>
#include <utility>

namespace bench {

template <typename Function, typename... Args>
auto measure_time(Function &&function, Args &&...args) {
  const auto start = std::chrono::steady_clock::now();

  std::invoke(std::forward<Function>(function), std::forward<Args>(args)...);

  const auto end = std::chrono::steady_clock::now();
  return end - start;
}

} // namespace bench
