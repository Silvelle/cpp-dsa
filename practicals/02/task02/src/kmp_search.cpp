#include "kmp_search.hpp"

#include <stdexcept>

namespace task02 {

std::vector<std::size_t> buildPrefixFunction(std::string_view pattern) {
  std::vector<std::size_t> prefix(pattern.size(), 0);

  for (std::size_t i = 1; i < pattern.size(); ++i) {
    std::size_t matched = prefix[i - 1];

    while (matched > 0 && pattern[i] != pattern[matched]) {
      matched = prefix[matched - 1];
    }
    if (pattern[i] == pattern[matched]) {
      ++matched;
    }

    prefix[i] = matched;
  }

  return prefix;
}

SearchResult findLastOccurrenceKmp(std::string_view text,
                                   std::string_view pattern) {
  if (pattern.empty()) {
    throw std::invalid_argument("Образец не должен быть пустым");
  }

  const std::vector<std::size_t> prefix = buildPrefixFunction(pattern);
  SearchResult result;
  std::size_t textIndex = 0;
  std::size_t patternIndex = 0;

  while (textIndex < text.size()) {
    ++result.comparisons;

    if (text[textIndex] == pattern[patternIndex]) {
      ++textIndex;
      ++patternIndex;

      if (patternIndex == pattern.size()) {
        result.lastIndex =
            static_cast<std::ptrdiff_t>(textIndex - pattern.size());
        // Продолжаем поиск и допускаем перекрывающиеся вхождения.
        patternIndex = prefix[patternIndex - 1];
      }
    } else if (patternIndex > 0) {
      // Уже просмотренный символ текста сравнивается с более короткой рамкой.
      patternIndex = prefix[patternIndex - 1];
    } else {
      ++textIndex;
    }
  }

  return result;
}

} // namespace task02
