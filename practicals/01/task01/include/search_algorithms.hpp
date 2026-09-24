#pragma once

#include "city.hpp"
#include "measurements.hpp"

#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>

namespace task01 {

struct IndexEntry {
  int key;
  std::size_t recordNumber;
};

std::optional<City>
linearSearchInFile(const std::filesystem::path &binaryPath, int searchKey,
                   OperationCounter &counter);

std::vector<IndexEntry>
buildIndex(const std::filesystem::path &binaryPath);

std::optional<std::size_t>
fibonacciSearch(const std::vector<IndexEntry> &index, int searchKey,
                OperationCounter &counter);

std::optional<City>
readRecordAt(const std::filesystem::path &binaryPath, std::size_t recordNumber,
             OperationCounter &counter);

} // namespace task01
