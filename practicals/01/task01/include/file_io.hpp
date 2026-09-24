#pragma once

#include "city.hpp"

#include <filesystem>
#include <vector>

namespace task01 {

void generateTextFile(int count, const std::filesystem::path &path);

void convertTextToBinary(const std::filesystem::path &textPath,
                         const std::filesystem::path &binaryPath);

std::vector<City> readBinaryFile(const std::filesystem::path &path);

} // namespace task01
