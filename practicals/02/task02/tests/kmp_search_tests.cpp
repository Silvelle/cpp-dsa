#include "kmp_search_tests.hpp"

#include "kmp_search.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace task02 {
namespace {

class TestRunner {
public:
  void check(bool condition, const std::string &name) {
    ++total_;
    if (condition) {
      std::cout << "[УСПЕХ] " << name << '\n';
      return;
    }

    ++failed_;
    std::cout << "[ОШИБКА] " << name << '\n';
  }

  bool passed() const {
    std::cout << "\nПройдено проверок: " << total_ - failed_ << " из " << total_
              << '\n';
    return failed_ == 0;
  }

private:
  std::size_t total_ = 0;
  std::size_t failed_ = 0;
};

} // namespace

bool runAllTests() {
  TestRunner tests;

  SearchResult result = findLastOccurrenceKmp("ABABABA", "ABA");
  tests.check(result.lastIndex == 4, "несколько перекрывающихся вхождений");
  tests.check(result.comparisons == 7, "счётчик сравнений при успешном поиске");

  result = findLastOccurrenceKmp("algorithm", "search");
  tests.check(result.lastIndex == -1 && result.comparisons == 9,
              "образец отсутствует в небольшом тексте");

  result = findLastOccurrenceKmp("abc", "abcd");
  tests.check(result.lastIndex == -1, "образец длиннее текста");
  tests.check(result.comparisons == 3, "сравнения при образце длиннее текста");

  result = findLastOccurrenceKmp("aaaaa", "aaa");
  tests.check(result.lastIndex == 2 && result.comparisons == 5,
              "перекрывающиеся одинаковые символы");

  result = findLastOccurrenceKmp("KMP", "KMP");
  tests.check(result.lastIndex == 0 && result.comparisons == 3,
              "образец совпадает со всем текстом");

  const std::string largeText(100000, 'a');
  result = findLastOccurrenceKmp(largeText + 'b', "aaaab");
  tests.check(result.lastIndex == 99996 && result.comparisons == 199997,
              "успешный поиск в большом тексте");

  result = findLastOccurrenceKmp(largeText, "aaaab");
  tests.check(result.lastIndex == -1 && result.comparisons == 199996,
              "безуспешный поиск в большом тексте");

  bool exceptionThrown = false;
  try {
    findLastOccurrenceKmp("abc", "");
  } catch (const std::invalid_argument &) {
    exceptionThrown = true;
  }
  tests.check(exceptionThrown, "пустой образец нарушает предусловие");

  return tests.passed();
}

} // namespace task02
