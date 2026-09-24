#include "benchmark.hpp"
#include "search_tests.hpp"

#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  try {
    std::vector<task01::PreparedDataset> datasets;
    for (const int count : task01::recordCountsFromArguments(argc, argv)) {
      datasets.push_back(task01::prepareDataset(count, "data"));
    }

    std::vector<task01::TestResult> linearResults;
    std::vector<task01::TestResult> fibonacciResults;
    for (const task01::PreparedDataset &dataset : datasets) {
      linearResults.push_back(task01::runLinearSearchTest(
          dataset.binaryPath, dataset.recordCount, dataset.searchKey));
    }
    for (const task01::PreparedDataset &dataset : datasets) {
      fibonacciResults.push_back(task01::runFibonacciSearchTest(
          dataset.binaryPath, dataset.recordCount, dataset.searchKey));
    }

    task01::printResults(std::cout, "LINEAR SEARCH", linearResults);
    task01::printResults(std::cout, "FIBONACCI SEARCH", fibonacciResults);
    task01::saveResults("results/linear_search_results.csv", linearResults);
    task01::saveResults("results/fibonacci_search_results.csv",
                        fibonacciResults);
    std::cout << "\nSaved result tables to results/*.csv\n";
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
  return 0;
}
