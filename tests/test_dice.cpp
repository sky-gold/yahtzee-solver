#include <gtest/gtest.h>
#include "game_state/dice.h"

TEST(DiceTest, DefaultConstructor) {
    Dice dice;
    EXPECT_EQ(dice.total(), 0);
    EXPECT_EQ(dice.sum(), 0);
    for (size_t i = 1; i <= 6; ++i) {
        EXPECT_EQ(dice[i], 0);
    }
}

TEST(DiceTest, VectorConstructor) {
    Dice dice({1, 2, 3, 4, 5, 6, 1, 2});
    EXPECT_EQ(dice.total(), 8);
    EXPECT_EQ(dice[1], 2);
    EXPECT_EQ(dice[2], 2);
    EXPECT_EQ(dice[3], 1);
    EXPECT_EQ(dice[4], 1);
    EXPECT_EQ(dice[5], 1);
    EXPECT_EQ(dice[6], 1);
    EXPECT_EQ(dice.sum(), 1*2 + 2*2 + 3*1 + 4*1 + 5*1 + 6*1);
}

TEST(DiceTest, InitializerListConstructor) {
    Dice dice{1, 1, 1, 2, 2};
    EXPECT_EQ(dice.total(), 5);
    EXPECT_EQ(dice[1], 3);
    EXPECT_EQ(dice[2], 2);
    EXPECT_EQ(dice.sum(), 1*3 + 2*2);
}

TEST(DiceTest, AddDieValid) {
    Dice dice;
    dice.add_die(1);
    dice.add_die(1);
    dice.add_die(6);
    
    EXPECT_EQ(dice.total(), 3);
    EXPECT_EQ(dice[1], 2);
    EXPECT_EQ(dice[6], 1);
    EXPECT_EQ(dice.sum(), 1*2 + 6*1);
}

TEST(DiceTest, AddDieInvalid) {
    Dice dice;
    EXPECT_THROW(dice.add_die(0), std::invalid_argument);
    EXPECT_THROW(dice.add_die(7), std::invalid_argument);
}

TEST(DiceTest, IndexOperatorValid) {
    Dice dice({1, 2, 3});
    EXPECT_EQ(dice[1], 1);
    EXPECT_EQ(dice[2], 1);
    EXPECT_EQ(dice[3], 1);
}

TEST(DiceTest, IndexOperatorInvalid) {
    Dice dice;
    EXPECT_THROW(dice[0], std::out_of_range);
    EXPECT_THROW(dice[7], std::out_of_range);
}

TEST(DiceTest, Reset) {
    Dice dice({1, 2, 3});
    EXPECT_EQ(dice.total(), 3);
    
    dice.reset();
    EXPECT_EQ(dice.total(), 0);
    EXPECT_EQ(dice.sum(), 0);
    for (size_t i = 1; i <= 6; ++i) {
        EXPECT_EQ(dice[i], 0);
    }
}

TEST(DiceTest, CountsArray) {
    Dice dice({1, 1, 2, 3, 3, 3});
    const auto& counts = dice.counts();
    
    EXPECT_EQ(counts[0], 2); // 1's
    EXPECT_EQ(counts[1], 1); // 2's
    EXPECT_EQ(counts[2], 3); // 3's
    EXPECT_EQ(counts[3], 0); // 4's
    EXPECT_EQ(counts[4], 0); // 5's
    EXPECT_EQ(counts[5], 0); // 6's
}

TEST(DiceTest, SumCalculation) {
    Dice dice1({1, 2, 3, 4, 5});
    EXPECT_EQ(dice1.sum(), 15);
    
    Dice dice2({6, 6, 6});
    EXPECT_EQ(dice2.sum(), 18);
    
    Dice dice3({1});
    EXPECT_EQ(dice3.sum(), 1);
}
