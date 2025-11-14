#include <iostream>
#include "calculator/calculator.h"

int main() {
    Calculator calc;
    std::cout << "Hello from Yahtzee Solver!" << std::endl;
    std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
    return 0;
}