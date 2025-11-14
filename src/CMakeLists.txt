# Создаем библиотеку из всех исходников кроме main.cpp
file(GLOB_RECURSE LIB_SOURCES "*.cpp")
set(MAIN_CPP "main.cpp")
list(REMOVE_ITEM LIB_SOURCES ${MAIN_CPP})

# Библиотека с основной логикой
add_library(yahtzee_lib STATIC ${LIB_SOURCES})

# Включаем директории для заголовков
target_include_directories(yahtzee_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Основная программа
add_executable(${PROJECT_NAME} ${MAIN_CPP})
target_link_libraries(${PROJECT_NAME} PRIVATE yahtzee_lib)

# Устанавливаем выходную директорию
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)