#include "benchmark.hpp"
#include "search_tests.hpp"

#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  try {
    std::vector<task01::TestResult> results;
    for (const int count : task01::recordCountsFromArguments(argc, argv)) {
      const task01::PreparedDataset dataset =
          task01::prepareDataset(count, "data");
      results.push_back(task01::runLinearSearchTest(
          dataset.binaryPath, dataset.recordCount, dataset.searchKey));
    }

    task01::printResults(std::cout, "LINEAR SEARCH", results);
    task01::saveResults("results/linear_search_results.csv", results);
    std::cout << "\nSaved to results/linear_search_results.csv\n";
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
  return 0;
}
