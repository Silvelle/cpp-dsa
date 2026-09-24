#include "file_io.hpp"

#include <cstring>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

namespace task01 {
namespace {

constexpr int kMinCode = 100'000;
constexpr int kMaxCode = 999'999;

} // namespace

void generateTextFile(int count, const std::filesystem::path &path) {
  // Генерация случайных кодов
  std::mt19937 rng(12345);
  std::uniform_int_distribution<int> codeDist(kMinCode, kMaxCode);
  // Множество для хранения уникальных значений
  std::unordered_set<int> usedCodes;

  std::ofstream out(path);

  // Добавляем элементы в множество
  for (int i = 0; i < count; ++i) {
    int code;

    do {
      code = codeDist(rng);
    } while (!usedCodes.insert(code).second);

    out << code << " City_" << i + 1 << '\n';
  }
}

void convertTextToBinary(const std::filesystem::path &textPath,
                         const std::filesystem::path &binaryPath) {
  // Открываем текстовый файл и создаем массив городов
  std::ifstream in(textPath);
  std::vector<City> cities;

  // Считываем код и название каждого города
  int code;
  std::string name;
  while (in >> code >> name) {
    City city{};
    city.code = code;
    std::strncpy(city.name, name.c_str(), kCityNameSize - 1);
    cities.push_back(city);
  }

  // Записываем города в бинарный файл
  std::ofstream out(binaryPath, std::ios::binary);
  for (const auto &city : cities) {
    out.write(reinterpret_cast<const char *>(&city), sizeof(City));
  }
}

std::vector<City> readBinaryFile(const std::filesystem::path &path) {
  // Открываем файл в бинарном режиме
  std::ifstream in(path, std::ios::binary);
  std::vector<City> cities;
  City city;

  // Считываем записи и добавляем их в массив
  while (in.read(reinterpret_cast<char *>(&city), sizeof(City))) {
    cities.push_back(city);
  }

  // Возвращаем все прочитанные города
  return cities;
}

} // namespace task01
