add_executable(task01
  src/main.cpp
  src/file_io.cpp
  src/search_algorithms.cpp
  tests/benchmark.cpp
  tests/linear_search_test.cpp
  tests/fibonacci_search_test.cpp
)

target_include_directories(task01 PRIVATE include tests)
target_link_libraries(task01 PRIVATE practical_common)
target_compile_options(task01 PRIVATE -Wall -Wextra -Wpedantic)
