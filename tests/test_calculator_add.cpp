#include <gtest/gtest.h>
#include "calculator/calculator.h"

TEST(CalculatorAddTest, AddPositiveNumbers) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 3), 5);
    EXPECT_EQ(calc.add(10, 20), 30);
}

TEST(CalculatorAddTest, AddNegativeNumbers) {
    Calculator calc;
    EXPECT_EQ(calc.add(-2, -3), -5);
    EXPECT_EQ(calc.add(-10, 5), -5);
}

TEST(CalculatorAddTest, AddZero) {
    Calculator calc;
    EXPECT_EQ(calc.add(0, 0), 0);
    EXPECT_EQ(calc.add(5, 0), 5);
    EXPECT_EQ(calc.add(0, 7), 7);
}