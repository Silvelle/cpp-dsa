#include "hash_table.hpp"

#include <algorithm>
#include <stdexcept>

namespace task01 {

HashTable::HashTable(size_t initialCapacity)
    : slots_(nextPrime(std::max(initialCapacity, size_t{3}))) {}

bool HashTable::isPrime(size_t number) {
  if (number < 2) {
    return false;
  }
  for (size_t divisor = 2; divisor <= number / divisor; ++divisor) {
    if (number % divisor == 0) {
      return false;
    }
  }
  return true;
}

size_t HashTable::nextPrime(size_t number) {
  while (!isPrime(number)) {
    ++number;
  }
  return number;
}

size_t HashTable::primaryHash(int key) const {
  return static_cast<size_t>(key) % slots_.size();
}

size_t HashTable::secondaryHash(int key) const {
  // При простом размере таблицы шаг от 1 до m - 1 взаимно прост с m.
  return 1 + static_cast<size_t>(key) % (slots_.size() - 1);
}

size_t HashTable::probeIndex(int key, size_t attempt) const {
  // Формула двойного хеширования: h(k, i) = (h1(k) + i * h2(k)) mod m.
  return (primaryHash(key) + attempt * secondaryHash(key)) % slots_.size();
}

size_t HashTable::findIndex(int key) const {
  for (size_t attempt = 0; attempt < slots_.size(); ++attempt) {
    const size_t index = probeIndex(key, attempt);
    const Slot &slot = slots_[index];

    // Пустая ячейка завершает цепочку пробирования.
    if (slot.state == SlotState::Empty) {
      return slots_.size();
    }
    if (slot.state == SlotState::Occupied && slot.key == key) {
      return index;
    }
  }
  return slots_.size();
}

bool HashTable::insert(const BankAccount &account) {
  if (findIndex(account.accountNumber) != slots_.size()) {
    return false;
  }

  // Учитываем и удалённые ячейки: их избыток удлиняет поиск.
  const double projectedOccupancy =
      static_cast<double>(size_ + deletedCount_ + 1) / slots_.size();
  if (projectedOccupancy > maxLoadFactor_) {
    rehash(slots_.size() * 2);
  }

  size_t dataIndex;
  if (freeDataIndices_.empty()) {
    dataIndex = data_.size();
    data_.push_back(account);
  } else {
    dataIndex = freeDataIndices_.back();
    freeDataIndices_.pop_back();
    data_[dataIndex] = account;
  }

  insertWithoutResize(account.accountNumber, dataIndex);
  return true;
}

void HashTable::insertWithoutResize(int key, size_t dataIndex) {
  size_t firstDeleted = slots_.size();

  for (size_t attempt = 0; attempt < slots_.size(); ++attempt) {
    const size_t index = probeIndex(key, attempt);
    Slot &slot = slots_[index];

    if (slot.state == SlotState::Deleted && firstDeleted == slots_.size()) {
      firstDeleted = index;
    }
    if (slot.state == SlotState::Empty) {
      const size_t target =
          firstDeleted == slots_.size() ? index : firstDeleted;
      slots_[target].key = key;
      slots_[target].dataIndex = dataIndex;
      slots_[target].state = SlotState::Occupied;
      ++size_;
      if (firstDeleted != slots_.size()) {
        --deletedCount_;
      }
      return;
    }
  }

  // Теоретически сюда можно попасть только при полностью занятом массиве.
  if (firstDeleted != slots_.size()) {
    slots_[firstDeleted].key = key;
    slots_[firstDeleted].dataIndex = dataIndex;
    slots_[firstDeleted].state = SlotState::Occupied;
    ++size_;
    --deletedCount_;
    return;
  }
  throw std::logic_error("В хеш-таблице нет свободной ячейки");
}

bool HashTable::erase(int accountNumber) {
  const size_t index = findIndex(accountNumber);
  if (index == slots_.size()) {
    return false;
  }

  // Метка Deleted сохраняет цепочку пробирования для остальных ключей.
  data_[slots_[index].dataIndex].reset();
  freeDataIndices_.push_back(slots_[index].dataIndex);
  slots_[index].state = SlotState::Deleted;
  --size_;
  ++deletedCount_;
  return true;
}

BankAccount *HashTable::find(int accountNumber) {
  const size_t index = findIndex(accountNumber);
  return index == slots_.size() ? nullptr : &*data_[slots_[index].dataIndex];
}

const BankAccount *HashTable::find(int accountNumber) const {
  const size_t index = findIndex(accountNumber);
  return index == slots_.size() ? nullptr : &*data_[slots_[index].dataIndex];
}

std::vector<BankAccount> HashTable::values() const {
  std::vector<BankAccount> result;
  result.reserve(size_);
  for (const std::optional<BankAccount> &account : data_) {
    if (account) {
      result.push_back(*account);
    }
  }
  return result;
}

size_t HashTable::size() const { return size_; }

size_t HashTable::capacity() const { return slots_.size(); }

double HashTable::loadFactor() const {
  return static_cast<double>(size_) / slots_.size();
}

void HashTable::rehash(size_t requestedCapacity) {
  std::vector<Slot> oldSlots = std::move(slots_);
  slots_ = std::vector<Slot>(nextPrime(requestedCapacity));
  size_ = 0;
  deletedCount_ = 0;

  // Индексы зависят от размера массива, поэтому вставляем записи заново.
  for (const Slot &slot : oldSlots) {
    if (slot.state == SlotState::Occupied) {
      insertWithoutResize(slot.key, slot.dataIndex);
    }
  }
}

} // namespace task01
