#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <iosfwd>
#include <vector>

namespace task01 {

inline constexpr std::array<int, 3> kDefaultRecordCounts = {100, 1'000,
                                                            10'000};

struct TestResult {
  const char *algorithm;
  std::size_t recordCount;
  double milliseconds;
  std::size_t comparisons;
  std::size_t movements;
  bool found;
};

struct PreparedDataset {
  std::filesystem::path binaryPath;
  std::size_t recordCount;
  int searchKey;
};

std::vector<int> recordCountsFromArguments(int argc, char *argv[]);

PreparedDataset prepareDataset(int recordCount,
                               const std::filesystem::path &dataDirectory);

void printResults(std::ostream &out, const char *title,
                  const std::vector<TestResult> &results);

void saveResults(const std::filesystem::path &outputPath,
                 const std::vector<TestResult> &results);

} // namespace task01
