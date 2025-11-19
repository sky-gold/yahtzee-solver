file(GLOB TEST_SOURCES 
    "*.cpp"
)

add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests PRIVATE
    yahtzee_lib
    gtest_main
)

target_include_directories(runTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Устанавливаем выходную директорию
set_target_properties(runTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME AllTests COMMAND runTests)