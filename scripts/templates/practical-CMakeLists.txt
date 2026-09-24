cmake_minimum_required(VERSION 3.16)

get_filename_component(PRACTICAL_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project("cpp_dsa_practical_${PRACTICAL_NAME}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(practical_common INTERFACE)
target_include_directories(practical_common INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/common"
)

enable_testing()

file(GLOB TASK_CMAKE_FILES CONFIGURE_DEPENDS
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/task*/CMakeLists.txt"
)

foreach(TASK_CMAKE_FILE IN LISTS TASK_CMAKE_FILES)
  get_filename_component(TASK_DIRECTORY "${TASK_CMAKE_FILE}" DIRECTORY)
  get_filename_component(TASK_NAME "${TASK_DIRECTORY}" NAME)
  add_subdirectory("${TASK_DIRECTORY}" "${TASK_NAME}-build")
endforeach()
