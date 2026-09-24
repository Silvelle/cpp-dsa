#include "search_tests.hpp"

#include "measurements.hpp"
#include "search_algorithms.hpp"

#include <optional>

namespace task01 {

TestResult runLinearSearchTest(const std::filesystem::path &binaryPath,
                               std::size_t recordCount, int searchKey) {
  OperationCounter counter;
  std::optional<City> foundCity;

  const double milliseconds = measureMilliseconds([&] {
    foundCity = linearSearchInFile(binaryPath, searchKey, counter);
  });

  return {"Linear", recordCount, milliseconds, counter.comparisons,
          counter.movements, foundCity.has_value()};
}

} // namespace task01
