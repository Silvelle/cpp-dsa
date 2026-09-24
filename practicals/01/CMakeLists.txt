cmake_minimum_required(VERSION 3.16)

project(cpp_dsa_practical_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(practical_common INTERFACE)
target_include_directories(practical_common INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/common"
)

add_subdirectory(task01 task01-build)

# Add future tasks in the same way:
# add_subdirectory(task02 task02-build)
