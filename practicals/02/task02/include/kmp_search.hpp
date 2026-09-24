#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace task02 {

struct SearchResult {
  std::ptrdiff_t lastIndex = -1;
  std::size_t comparisons = 0;
};

// Строит префикс-функцию образца для алгоритма Кнута-Морриса-Пратта.
std::vector<std::size_t> buildPrefixFunction(std::string_view pattern);

// Возвращает индекс последнего вхождения образца и число сравнений символов.
// Предусловие: образец не пуст.
// Постусловие: lastIndex равен индексу начала последнего вхождения либо -1,
// а comparisons содержит число сравнений символов текста и образца.
SearchResult findLastOccurrenceKmp(std::string_view text,
                                   std::string_view pattern);

} // namespace task02
