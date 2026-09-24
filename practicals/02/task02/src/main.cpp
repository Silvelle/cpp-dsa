#include "kmp_search.hpp"
#include "kmp_search_tests.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void runDemo() {
  const std::string text = "ABABABA";
  const std::string pattern = "ABA";
  const task02::SearchResult result =
      task02::findLastOccurrenceKmp(text, pattern);

  std::cout << "Текст: " << text << '\n'
            << "Образец: " << pattern << '\n'
            << "Индекс последнего вхождения: " << result.lastIndex << '\n'
            << "Сравнений символов текста и образца: " << result.comparisons
            << '\n';
}

void runInteractive() {
  std::string text;
  std::string pattern;

  std::cout << "Введите текст: ";
  std::getline(std::cin, text);
  std::cout << "Введите образец: ";
  std::getline(std::cin, pattern);

  try {
    const task02::SearchResult result =
        task02::findLastOccurrenceKmp(text, pattern);

    if (result.lastIndex == -1) {
      std::cout << "Образец в тексте не найден.\n";
    } else {
      std::cout << "Индекс последнего вхождения: " << result.lastIndex << '\n';
    }
    std::cout << "Количество сравнений: " << result.comparisons << '\n';
  } catch (const std::invalid_argument &error) {
    std::cerr << "Ошибка: " << error.what() << '\n';
  }
}

} // namespace

int main(int argc, char *argv[]) {
  if (argc > 1 && std::string(argv[1]) == "--test") {
    return task02::runAllTests() ? 0 : 1;
  }
  if (argc > 1 && std::string(argv[1]) == "--demo") {
    runDemo();
    return 0;
  }

  runInteractive();
  return 0;
}
