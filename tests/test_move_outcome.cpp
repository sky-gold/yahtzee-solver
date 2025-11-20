#include <gtest/gtest.h>
#include "move/move_outcome.h"
#include "move/move.h"

TEST(MoveOutcomeTest, ApplyScoreMoveUpperSection) {
    GameState state;
    Dice dice({1, 1, 1, 2, 3});
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::Ones);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 3); // Three ones
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::Ones).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::Ones).value(), 3);
}

TEST(MoveOutcomeTest, ApplyScoreMoveYahtzee) {
    GameState state;
    Dice dice({6, 6, 6, 6, 6});
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::Yahtzee);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 50); // Yahtzee score
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::Yahtzee).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::Yahtzee).value(), 50);
}

TEST(MoveOutcomeTest, ApplyScoreMoveFullHouse) {
    GameState state;
    Dice dice({2, 2, 3, 3, 3});
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::FullHouse);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 25); // Full House score
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::FullHouse).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::FullHouse).value(), 25);
}

TEST(MoveOutcomeTest, ApplyScoreMoveUpperBonus) {
    GameState state;
    // Set up scores to be just below bonus threshold (62 points)
    state.AddScoreToCategory(Category::Ones, 3);
    state.AddScoreToCategory(Category::Twos, 6);
    state.AddScoreToCategory(Category::Threes, 9);
    state.AddScoreToCategory(Category::Fours, 12);
    state.AddScoreToCategory(Category::Fives, 15);
    // Total: 45, need 18 more from Sixes to get bonus
    
    Dice dice({6, 6, 6, 6, 6}); // Five sixes = 30 points
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::Sixes);
    auto outcome = ApplyMove(state, move);
    
    // Should get 30 for sixes + 35 bonus = 65 total
    EXPECT_EQ(outcome.score_delta, 65);
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::Sixes).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::Sixes).value(), 30);
}

TEST(MoveOutcomeTest, ApplyScoreMoveMultipleYahtzeeBonus) {
    GameState state;
    // Record first yahtzee
    state.AddScoreToCategory(Category::Yahtzee, 50);
    
    Dice dice({5, 5, 5, 5, 5}); // Another yahtzee
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::Yahtzee);
    auto outcome = ApplyMove(state, move);
    
    // Should get 50 for yahtzee + 100 bonus = 150 total
    EXPECT_EQ(outcome.score_delta, 150);
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::Yahtzee).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::Yahtzee).value(), 100); // 50 + 50?
}

TEST(MoveOutcomeTest, ApplyScoreMoveJokerRule) {
    GameState state;
    // Record first yahtzee
    state.AddScoreToCategory(Category::Yahtzee, 50);
    
    Dice dice({4, 4, 4, 4, 4}); // Another yahtzee
    state.SetCurrentDice(dice);
    
    // Use joker rule to score in upper section (Fours)
    ScoreMove move(Category::Fours);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 20); // Four fours = 20 points
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::Fours).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::Fours).value(), 20);
}

TEST(MoveOutcomeTest, ApplyRerrolMove) {
    GameState state;
    state.SetRemainingRerolls(2);
    
    std::vector<size_t> keep_values{1, 2};
    RerrolMove move(keep_values);
    
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 0); // No score change for reroll
    EXPECT_EQ(outcome.new_state.GetRemainingRerolls(), 1); // Decreased by 1
}

TEST(MoveOutcomeTest, ApplyMoveShortGameState) {
    ShortGameState state;
    Dice dice({1, 2, 3, 4, 5});
    state.SetCurrentDice(dice);
    
    ScoreMove move(Category::Chance);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 15); // Sum of all dice
    EXPECT_TRUE(outcome.new_state.IsCategoryUsed(Category::Chance));
}

TEST(MoveOutcomeTest, ApplyMoveInvalidCategory) {
    GameState state;
    Dice dice({1, 2, 3, 4, 5});
    state.SetCurrentDice(dice);
    
    // Try to score ThreeOfAKind with no three of a kind
    ScoreMove move(Category::ThreeOfAKind);
    auto outcome = ApplyMove(state, move);
    
    EXPECT_EQ(outcome.score_delta, 0); // No three of a kind
    EXPECT_TRUE(outcome.new_state.GetCategoryScore(Category::ThreeOfAKind).has_value());
    EXPECT_EQ(outcome.new_state.GetCategoryScore(Category::ThreeOfAKind).value(), 0);
}