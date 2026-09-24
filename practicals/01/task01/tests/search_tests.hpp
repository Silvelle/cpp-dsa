#pragma once

#include "benchmark.hpp"

#include <cstddef>
#include <filesystem>

namespace task01 {

TestResult runLinearSearchTest(const std::filesystem::path &binaryPath,
                               std::size_t recordCount, int searchKey);

TestResult runFibonacciSearchTest(const std::filesystem::path &binaryPath,
                                  std::size_t recordCount, int searchKey);

} // namespace task01
