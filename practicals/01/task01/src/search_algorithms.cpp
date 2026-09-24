#include "search_algorithms.hpp"

#include <algorithm>
#include <fstream>

namespace task01 {

std::optional<City> linearSearchInFile(const std::filesystem::path &binaryPath,
                                       int searchKey,
                                       OperationCounter &counter) {
  // Открываем бинарный файл и создаем буфер для одной записи
  std::ifstream input(binaryPath, std::ios::binary);
  City city{};
  ++counter.movements; // Инициализация буфера city

  // Последовательно считываем записи до конца файла
  while (++counter.comparisons &&
         input.read(reinterpret_cast<char *>(&city), sizeof(City))) {
    ++counter.movements; // Чтение одной записи в city

    // Сравниваем код города с искомым ключом
    ++counter.comparisons;
    if (city.code == searchKey) {
      return city;
    }
  }

  // Возвращаем пустой результат, если город не найден
  return std::nullopt;
}

std::vector<IndexEntry> buildIndex(const std::filesystem::path &binaryPath) {
  // Открываем бинарный файл и создаем массив индекса
  std::ifstream input(binaryPath, std::ios::binary);
  std::vector<IndexEntry> index;
  City city{};
  std::size_t recordNumber = 0;

  // Сохраняем код города и номер его записи в файле
  while (input.read(reinterpret_cast<char *>(&city), sizeof(City))) {
    index.push_back({city.code, recordNumber});
    ++recordNumber;
  }

  // Сортируем индекс по коду города
  std::sort(index.begin(), index.end(),
            [](const IndexEntry &left, const IndexEntry &right) {
              return left.key < right.key;
            });
  return index;
}

std::optional<std::size_t> fibonacciSearch(const std::vector<IndexEntry> &index,
                                           int searchKey,
                                           OperationCounter &counter) {
  // Создаем первые три числа Фибоначчи
  std::size_t fibPrevious = 0;
  std::size_t fibCurrent = 1;
  std::size_t fibNext = fibPrevious + fibCurrent;
  counter.movements += 3;

  // Находим число Фибоначчи не меньше размера индекса
  while (++counter.comparisons && fibNext < index.size()) {
    fibPrevious = fibCurrent;
    fibCurrent = fibNext;
    fibNext = fibPrevious + fibCurrent;
    counter.movements += 3;
  }

  // Храним границу уже отброшенной левой части индекса
  std::size_t offsetPlusOne = 0;
  ++counter.movements;

  // Сужаем область поиска с помощью чисел Фибоначчи
  while (++counter.comparisons && fibNext > 1) {
    ++counter.comparisons; // Сравнение внутри std::min
    const std::size_t candidate =
        std::min(offsetPlusOne + fibPrevious, index.size()) - 1;
    ++counter.movements; // Инициализация позиции candidate

    // Если ключ больше, отбрасываем левую часть
    ++counter.comparisons;
    if (index[candidate].key < searchKey) {
      fibNext = fibCurrent;
      fibCurrent = fibPrevious;
      fibPrevious = fibNext - fibCurrent;
      offsetPlusOne = candidate + 1;
      counter.movements += 4;
    } else {
      // Если ключ меньше, отбрасываем правую часть
      ++counter.comparisons;
      if (index[candidate].key > searchKey) {
        fibNext = fibPrevious;
        fibCurrent -= fibPrevious;
        fibPrevious = fibNext - fibCurrent;
        counter.movements += 3;
      } else {
        // Возвращаем позицию найденного элемента
        return candidate;
      }
    }
  }

  // Проверяем последний оставшийся элемент
  ++counter.comparisons;
  if (offsetPlusOne < index.size()) {
    ++counter.comparisons;
    if (index[offsetPlusOne].key == searchKey) {
      return offsetPlusOne;
    }
  }

  return std::nullopt;
}

std::optional<City> readRecordAt(const std::filesystem::path &binaryPath,
                                 std::size_t recordNumber,
                                 OperationCounter &counter) {
  // Открываем бинарный файл и создаем буфер для записи
  std::ifstream input(binaryPath, std::ios::binary);
  City city{};
  ++counter.movements; // Инициализация буфера city

  // Переходим к нужной записи и считываем ее
  input.seekg(static_cast<std::streamoff>(recordNumber * sizeof(City)));
  input.read(reinterpret_cast<char *>(&city), sizeof(City));
  ++counter.movements;   // Чтение записи
  ++counter.comparisons; // Проверка успешного чтения

  // Возвращаем пустой результат при ошибке чтения
  if (!input) {
    return std::nullopt;
  }
  return city;
}

} // namespace task01
