# Собрать все .cpp файлы из tests/
file(GLOB TEST_SOURCES "*.cpp")

# Test executable
add_executable(runTests ${TEST_SOURCES})

# Линкуем с нашей библиотекой и gtest
target_link_libraries(runTests PRIVATE
    yahtzee_lib
    gtest_main
)

# Включаем директории для заголовков
target_include_directories(runTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Устанавливаем выходную директорию
set_target_properties(runTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME AllTests COMMAND runTests)