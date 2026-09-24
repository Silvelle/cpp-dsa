# Practical 01 — Task 01

The program creates text datasets with unique city codes, converts them to
binary files without changing the original record order, and compares linear
file search with Fibonacci search through a sorted in-memory index. The
Fibonacci test retrieves the final record by direct file access after finding
its index entry.

From this `task01` directory, configure the parent practical and then run all
required sizes (100, 1,000, and 10,000 records):

```sh
cmake -S .. -B ../build
cmake --build ../build
../build/task01-build/task01
```

Run selected sizes:

```sh
../build/task01-build/task01 100 1000 10000
```

The result columns are the record count `n`, elapsed milliseconds `t`, key and
control comparisons `C`, data movements/assignments `M`, and `T = C + M`.
Dataset preparation and construction of the auxiliary Fibonacci index are not
included in the search timing.

Counting follows the algorithm code directly: every evaluation of a loop or
branch condition increments `C`, including the final failed loop check. Every
algorithm-variable initialization, assignment, increment/decrement, and
successful record read increments `M`. Infrastructure operations such as
constructing a file stream or positioning its read pointer are not counted.

Results are printed to the terminal and saved as CSV files:

- `results/linear_search_results.csv`
- `results/fibonacci_search_results.csv`

The practical-level `../CMakeLists.txt` owns the C++ settings and the shared
`../common` include directory. Each repeated task has a small `CMakeLists.txt`
that lists its own source files and creates its executable.
