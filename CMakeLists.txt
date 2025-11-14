cmake_minimum_required(VERSION 3.15)
project(yahtzee_solver VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение Google Test

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Поддиректории
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()