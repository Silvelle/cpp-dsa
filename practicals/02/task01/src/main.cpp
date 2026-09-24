#include "hash_table.hpp"
#include "hash_table_tests.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

using std::size_t;
using task01::BankAccount;
using task01::HashTable;

// Возвращает семь записей, которыми таблица заполняется при запуске.
std::vector<BankAccount> initialAccounts() {
  return {
      {1000000, "Иванов Иван Иванович", "Москва, ул. Тверская, д. 1"},
      {1000007, "Петров Пётр Петрович", "Москва, ул. Арбат, д. 12"},
      {1000014, "Сидорова Анна Олеговна", "Химки, ул. Молодёжная, д. 8"},
      {2345678, "Кузнецов Максим Игоревич", "Балашиха, пр-т Ленина, д. 4"},
      {3456789, "Смирнова Елена Павловна", "Москва, ул. Лесная, д. 15"},
      {4567890, "Орлов Денис Андреевич", "Мытищи, ул. Мира, д. 6"},
      {5678901, "Волкова Мария Сергеевна", "Москва, ул. Садовая, д. 19"},
  };
}

void fillTable(HashTable &table) {
  for (const BankAccount &account : initialAccounts()) {
    table.insert(account);
  }
}

void printHelp() {
  std::cout << "\nКоманды:\n"
            << "  добавить <номер>;<ФИО>;<адрес>\n"
            << "  найти <номер>\n"
            << "  удалить <номер>\n"
            << "  вывести\n"
            << "  статистика\n"
            << "  помощь\n"
            << "  выход\n";
}

void printAccount(const BankAccount &account) {
  std::cout << account.accountNumber << " | " << account.fullName << " | "
            << account.address << '\n';
}

bool validAccountNumber(int accountNumber) {
  return accountNumber >= 1000000 && accountNumber <= 9999999;
}

// Разбирает команду добавления с полями, разделёнными точкой с запятой.
bool parseAccount(const std::string &arguments, BankAccount &account) {
  std::stringstream input(arguments);
  std::string numberText;

  if (!std::getline(input, numberText, ';') ||
      !std::getline(input, account.fullName, ';') ||
      !std::getline(input, account.address)) {
    return false;
  }

  try {
    size_t processed = 0;
    account.accountNumber = std::stoi(numberText, &processed);
    return processed == numberText.size() &&
           validAccountNumber(account.accountNumber) &&
           !account.fullName.empty() && !account.address.empty();
  } catch (const std::exception &) {
    return false;
  }
}

void printTable(const HashTable &table) {
  const std::vector<BankAccount> accounts = table.values();
  std::cout << "\nСодержимое таблицы (" << accounts.size() << " записей):\n";
  for (const BankAccount &account : accounts) {
    printAccount(account);
  }
}

void printStatistics(const HashTable &table) {
  std::cout << "Записей: " << table.size()
            << ", размер массива: " << table.capacity()
            << ", заполнение: " << std::fixed << std::setprecision(2)
            << table.loadFactor() * 100.0 << "%\n";
}

void runDemo() {
  HashTable table(7);
  fillTable(table);

  std::cout << "Демонстрация варианта 13\n";
  printStatistics(table);

  std::cout << "Поиск счёта 1000007: ";
  if (const BankAccount *account = table.find(1000007)) {
    printAccount(*account);
  }

  std::cout << "Удаление счёта 1000007: "
            << (table.erase(1000007) ? "выполнено" : "запись не найдена")
            << '\n';
  std::cout << "Повторный поиск: "
            << (table.find(1000007) == nullptr ? "запись не найдена"
                                               : "запись найдена")
            << '\n';

  const BankAccount newAccount{6789012, "Фёдоров Алексей Романович",
                               "Москва, ул. Полевая, д. 3"};
  std::cout << "Вставка счёта " << newAccount.accountNumber << ": "
            << (table.insert(newAccount) ? "выполнена" : "ключ уже существует")
            << '\n';
  printStatistics(table);
}

void runInteractive() {
  HashTable table(7);
  fillTable(table);

  std::cout << "Хеш-таблица банковских счетов. Вариант 13.\n"
            << "Таблица автоматически заполнена семью записями.\n";
  printHelp();

  std::string line;
  while (std::cout << "\n> " && std::getline(std::cin, line)) {
    if (line == "выход" || line == "exit") {
      break;
    }
    if (line == "помощь" || line == "help") {
      printHelp();
    } else if (line == "вывести" || line == "list") {
      printTable(table);
    } else if (line == "статистика" || line == "stats") {
      printStatistics(table);
    } else if (line.rfind("найти ", 0) == 0 || line.rfind("find ", 0) == 0) {
      const size_t separator = line.find(' ');
      try {
        const int key = std::stoi(line.substr(separator + 1));
        if (const BankAccount *account = table.find(key)) {
          printAccount(*account);
        } else {
          std::cout << "Запись не найдена.\n";
        }
      } catch (const std::exception &) {
        std::cout << "Ошибка: номер счёта должен быть целым числом.\n";
      }
    } else if (line.rfind("удалить ", 0) == 0 ||
               line.rfind("delete ", 0) == 0) {
      const size_t separator = line.find(' ');
      try {
        const int key = std::stoi(line.substr(separator + 1));
        std::cout << (table.erase(key) ? "Запись удалена.\n"
                                       : "Запись не найдена.\n");
      } catch (const std::exception &) {
        std::cout << "Ошибка: номер счёта должен быть целым числом.\n";
      }
    } else if (line.rfind("добавить ", 0) == 0 || line.rfind("add ", 0) == 0) {
      const size_t separator = line.find(' ');
      BankAccount account{};
      if (!parseAccount(line.substr(separator + 1), account)) {
        std::cout << "Ошибка формата. Пример: добавить 1234567;Иванов Иван "
                     "Иванович;Москва, ул. Лесная, д. 1\n";
      } else {
        std::cout << (table.insert(account) ? "Запись добавлена.\n"
                                            : "Такой ключ уже существует.\n");
      }
    } else if (!line.empty()) {
      std::cout << "Неизвестная команда. Введите «помощь».\n";
    }
  }
}

} // namespace

int main(int argc, char *argv[]) {
  if (argc > 1 && std::string(argv[1]) == "--test") {
    return task01::runAllTests() ? 0 : 1;
  }
  if (argc > 1 && std::string(argv[1]) == "--demo") {
    runDemo();
    return 0;
  }

  runInteractive();
  return 0;
}
