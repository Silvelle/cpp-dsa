#pragma once

#include "bank_account.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace task01 {

using std::size_t;

class HashTable {
public:
  explicit HashTable(size_t initialCapacity = 7);

  bool insert(const BankAccount &account);
  bool erase(int accountNumber);
  BankAccount *find(int accountNumber);
  const BankAccount *find(int accountNumber) const;

  std::vector<BankAccount> values() const;
  size_t size() const;
  size_t capacity() const;
  double loadFactor() const;

private:
  enum class SlotState { Empty, Occupied, Deleted };

  struct Slot {
    int key{};
    size_t dataIndex{};
    SlotState state{SlotState::Empty};
  };

  static constexpr double maxLoadFactor_ = 0.65;

  // Полезные данные и хеш-таблица хранятся в отдельных массивах.
  std::vector<std::optional<BankAccount>> data_;
  std::vector<size_t> freeDataIndices_;
  std::vector<Slot> slots_;
  size_t size_{0};
  size_t deletedCount_{0};

  static bool isPrime(size_t number);
  static size_t nextPrime(size_t number);
  size_t primaryHash(int key) const;
  size_t secondaryHash(int key) const;
  size_t probeIndex(int key, size_t attempt) const;
  size_t findIndex(int key) const;
  void rehash(size_t requestedCapacity);
  void insertWithoutResize(int key, size_t dataIndex);
};

} // namespace task01
