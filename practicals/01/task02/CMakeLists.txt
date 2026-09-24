get_filename_component(TASK_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

file(GLOB TASK_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)
list(FILTER TASK_SOURCES EXCLUDE REGEX "/[^/]+_main\\.cpp$")

add_executable("${TASK_NAME}" ${TASK_SOURCES})
target_include_directories("${TASK_NAME}" PRIVATE include tests)
target_link_libraries("${TASK_NAME}" PRIVATE practical_common)
target_compile_options("${TASK_NAME}" PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME "${TASK_NAME}" COMMAND "${TASK_NAME}")
set_tests_properties("${TASK_NAME}" PROPERTIES
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
