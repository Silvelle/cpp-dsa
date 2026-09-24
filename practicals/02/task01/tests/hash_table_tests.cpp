#include "hash_table_tests.hpp"

#include "hash_table.hpp"

#include <iostream>
#include <string>

namespace task01 {

namespace {

class TestRunner {
public:
  void check(bool condition, const std::string &name) {
    ++total_;
    if (condition) {
      std::cout << "[УСПЕХ] " << name << '\n';
    } else {
      ++failed_;
      std::cerr << "[ОШИБКА] " << name << '\n';
    }
  }

  bool passed() const {
    std::cout << "\nПройдено проверок: " << total_ - failed_ << " из " << total_
              << "\n";
    return failed_ == 0;
  }

private:
  int total_{0};
  int failed_{0};
};

BankAccount account(int number, const std::string &name) {
  return {number, name, "Тестовый адрес"};
}

} // namespace

bool runAllTests() {
  TestRunner tests;
  HashTable table(7);

  // Эти ключи имеют одинаковый первый хеш при размере массива 7.
  tests.check(table.insert(account(1000000, "Первая запись")),
              "вставка первой записи");
  tests.check(table.insert(account(1000007, "Вторая запись")),
              "вставка записи с коллизией");
  tests.check(table.insert(account(1000014, "Третья запись")),
              "вставка второй записи с коллизией");
  tests.check(table.size() == 3, "корректный размер после вставки");

  const BankAccount *found = table.find(1000007);
  tests.check(found != nullptr && found->fullName == "Вторая запись",
              "поиск записи, вызвавшей коллизию");
  tests.check(table.find(9999999) == nullptr, "поиск отсутствующего ключа");
  tests.check(!table.insert(account(1000007, "Дубликат")),
              "отказ от вставки повторного ключа");

  tests.check(table.erase(1000000), "удаление первой записи цепочки");
  tests.check(table.find(1000000) == nullptr, "удалённая запись не находится");
  tests.check(table.find(1000007) != nullptr,
              "поиск за удалённой ячейкой остаётся корректным");
  tests.check(!table.erase(9999999), "удаление отсутствующего ключа");
  tests.check(table.insert(account(1000021, "Новая запись")),
              "повторное использование удалённой ячейки");

  const size_t oldCapacity = table.capacity();
  for (int key = 2000000; key < 2000020; ++key) {
    table.insert(account(key, "Запись для расширения"));
  }
  tests.check(table.capacity() > oldCapacity,
              "автоматическое расширение массива");
  tests.check(table.find(1000007) != nullptr && table.find(2000019) != nullptr,
              "сохранность записей после рехеширования");
  tests.check(table.values().size() == table.size(),
              "вывод содержит все занятые записи");

  return tests.passed();
}

} // namespace task01
