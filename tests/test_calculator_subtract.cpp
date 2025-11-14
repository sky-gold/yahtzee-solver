#include <gtest/gtest.h>
#include "calculator/calculator.h"

TEST(CalculatorSubtractTest, SubtractPositiveNumbers) {
    Calculator calc;
    EXPECT_EQ(calc.subtract(5, 3), 2);
    EXPECT_EQ(calc.subtract(10, 2), 8);
}

TEST(CalculatorSubtractTest, SubtractNegativeNumbers) {
    Calculator calc;
    EXPECT_EQ(calc.subtract(3, 5), -2);
    EXPECT_EQ(calc.subtract(-2, -3), 1);
}

TEST(CalculatorSubtractTest, SubtractZero) {
    Calculator calc;
    EXPECT_EQ(calc.subtract(5, 0), 5);
    EXPECT_EQ(calc.subtract(0, 5), -5);
    EXPECT_EQ(calc.subtract(0, 0), 0);
}