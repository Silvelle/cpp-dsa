#include "search_tests.hpp"

#include "measurements.hpp"
#include "search_algorithms.hpp"

#include <optional>

namespace task01 {

TestResult runFibonacciSearchTest(const std::filesystem::path &binaryPath,
                                  std::size_t recordCount, int searchKey) {
  // Building the auxiliary table is preparation, not part of search time.
  const std::vector<IndexEntry> index = buildIndex(binaryPath);
  OperationCounter counter;
  std::optional<City> foundCity;

  const double milliseconds = measureMilliseconds([&] {
    const std::optional<std::size_t> position =
        fibonacciSearch(index, searchKey, counter);
    if (position) {
      foundCity = readRecordAt(binaryPath, index[*position].recordNumber,
                               counter);
    }
  });

  return {"Fibonacci", recordCount, milliseconds, counter.comparisons,
          counter.movements, foundCity.has_value()};
}

} // namespace task01
