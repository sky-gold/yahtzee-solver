#pragma once

#include <array>
#include <vector>
#include <cstddef>

class Dice {
private:
    std::array<size_t, 6> counts_; // Количество выпавших костей с значениями 1-6

public:
    // Конструктор по умолчанию (все нули)
    Dice();
    
    // Конструктор из вектора выпавших значений
    Dice(const std::vector<size_t>& values);
    
    // Конструктор из списка инициализации
    Dice(std::initializer_list<size_t> values);
    
    // Получить количество костей с определенным значением (1-6)
    size_t operator[](size_t value) const;
    
    // Получить общее количество костей
    size_t total() const;
    
    // Получить сумму всех костей
    size_t sum() const;
    
    // Сбросить все счетчики в ноль
    void reset();
    
    // Добавить кость с определенным значением
    void add_die(size_t value);
    
    // Получить массив счетчиков
    const std::array<size_t, 6>& counts() const;
};