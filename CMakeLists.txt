# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

# Set the project name
project(yahtzee_solver)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source and test files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

include_directories(${SRC_DIR})

# Main executable
add_executable(yahtzee_solver ${SRC_DIR}/main.cpp)

# Enable testing
enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Собрать все .cpp файлы из папки tests/
file(GLOB TEST_SOURCES "${TEST_DIR}/*.cpp")

# Test executable
add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests gtest_main)

add_test(NAME AllTests COMMAND runTests)