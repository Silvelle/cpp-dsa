#include "benchmark.hpp"

#include "file_io.hpp"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>

namespace task01 {

std::vector<int> recordCountsFromArguments(int argc, char *argv[]) {
  if (argc == 1) {
    return {kDefaultRecordCounts.begin(), kDefaultRecordCounts.end()};
  }

  std::vector<int> counts;
  for (int argument = 1; argument < argc; ++argument) {
    char *end = nullptr;
    const long value = std::strtol(argv[argument], &end, 10);
    if (*argv[argument] == '\0' || *end != '\0' || value <= 0 ||
        value > std::numeric_limits<int>::max()) {
      throw std::invalid_argument("record counts must be positive integers");
    }
    counts.push_back(static_cast<int>(value));
  }
  return counts;
}

PreparedDataset prepareDataset(int recordCount,
                               const std::filesystem::path &dataDirectory) {
  std::filesystem::create_directories(dataDirectory);
  const std::string suffix = std::to_string(recordCount);
  const auto textPath = dataDirectory / ("cities_" + suffix + ".txt");
  const auto binaryPath = dataDirectory / ("cities_" + suffix + ".bin");

  generateTextFile(recordCount, textPath);
  convertTextToBinary(textPath, binaryPath);
  const std::vector<City> cities = readBinaryFile(binaryPath);
  if (cities.size() != static_cast<std::size_t>(recordCount)) {
    throw std::runtime_error("could not prepare " + suffix + " records");
  }

  // The last physical record gives the worst case for linear file search.
  return {binaryPath, cities.size(), cities.back().code};
}

void printResults(std::ostream &out, const char *title,
                  const std::vector<TestResult> &results) {
  out << '\n' << title << '\n';
  out << std::left << std::setw(12) << "Algorithm" << std::right
      << std::setw(10) << "n" << std::setw(14) << "t, ms" << std::setw(12)
      << "C" << std::setw(12) << "M" << std::setw(12) << "T=C+M"
      << std::setw(9) << "Found" << '\n';
  out << std::string(81, '-') << '\n';

  for (const TestResult &result : results) {
    out << std::left << std::setw(12) << result.algorithm << std::right
        << std::setw(10) << result.recordCount << std::setw(14) << std::fixed
        << std::setprecision(6) << result.milliseconds << std::setw(12)
        << result.comparisons << std::setw(12) << result.movements
        << std::setw(12) << result.comparisons + result.movements
        << std::setw(9) << (result.found ? "yes" : "no") << '\n';
  }
}

void saveResults(const std::filesystem::path &outputPath,
                 const std::vector<TestResult> &results) {
  std::filesystem::create_directories(outputPath.parent_path());
  std::ofstream output(outputPath);
  if (!output) {
    throw std::runtime_error("could not write " + outputPath.string());
  }

  output << "algorithm,n,t_ms,C,M,T,found\n";
  output << std::fixed << std::setprecision(6);
  for (const TestResult &result : results) {
    output << result.algorithm << ',' << result.recordCount << ','
           << result.milliseconds << ',' << result.comparisons << ','
           << result.movements << ','
           << result.comparisons + result.movements << ','
           << (result.found ? "yes" : "no") << '\n';
  }
}

} // namespace task01
