#include <gtest/gtest.h>
#include "game_state/game_state.h"

TEST(GameStateTest, DefaultConstructor) {
    GameState state;
    
    // Check default rerolls
    EXPECT_EQ(state.GetRemainingRerolls(), 2);
    
    // Check all categories are initially empty
    for (size_t i = 0; i < NUM_CATEGORIES; ++i) {
        EXPECT_FALSE(state.GetCategoryScore(static_cast<Category>(i)).has_value());
    }
    
    // Check default dice is empty
    EXPECT_EQ(state.GetCurrentDice().total(), 0);
    
    // Check yahtzee not recorded
    EXPECT_FALSE(state.IsYahtzeeRecorded());
}

TEST(GameStateTest, AddScoreToCategory) {
    GameState state;
    
    // Add score to Ones category
    state.AddScoreToCategory(Category::Ones, 3);
    auto ones_score = state.GetCategoryScore(Category::Ones);
    EXPECT_TRUE(ones_score.has_value());
    EXPECT_EQ(ones_score.value(), 3);
    
    // Add more score to same category
    state.AddScoreToCategory(Category::Ones, 2);
    ones_score = state.GetCategoryScore(Category::Ones);
    EXPECT_TRUE(ones_score.has_value());
    EXPECT_EQ(ones_score.value(), 5);
    
    // Add score to different category
    state.AddScoreToCategory(Category::Yahtzee, 50);
    auto yahtzee_score = state.GetCategoryScore(Category::Yahtzee);
    EXPECT_TRUE(yahtzee_score.has_value());
    EXPECT_EQ(yahtzee_score.value(), 50);
}

TEST(GameStateTest, SetAndGetRerolls) {
    GameState state;
    
    EXPECT_EQ(state.GetRemainingRerolls(), 2);
    
    state.SetRemainingRerolls(1);
    EXPECT_EQ(state.GetRemainingRerolls(), 1);
    
    state.SetRemainingRerolls(0);
    EXPECT_EQ(state.GetRemainingRerolls(), 0);
    
    state.SetRemainingRerolls(3);
    EXPECT_EQ(state.GetRemainingRerolls(), 3);
    
    // Test invalid reroll count
    EXPECT_THROW(state.SetRemainingRerolls(4), std::out_of_range);
}

TEST(GameStateTest, SetAndGetDice) {
    GameState state;
    
    Dice dice({1, 2, 3, 4, 5});
    state.SetCurrentDice(dice);
    
    const Dice& current_dice = state.GetCurrentDice();
    EXPECT_EQ(current_dice.total(), 5);
    EXPECT_EQ(current_dice[1], 1);
    EXPECT_EQ(current_dice[2], 1);
    EXPECT_EQ(current_dice[3], 1);
    EXPECT_EQ(current_dice[4], 1);
    EXPECT_EQ(current_dice[5], 1);
}

TEST(GameStateTest, GetRemainingUpperBonus) {
    GameState state;
    
    // Initially no scores, so full bonus remaining
    EXPECT_EQ(state.GetRemainingUpperBonus(), 63);
    
    // Add some upper section scores
    state.AddScoreToCategory(Category::Ones, 3);   // 3 points
    state.AddScoreToCategory(Category::Twos, 6);   // 6 points
    state.AddScoreToCategory(Category::Threes, 9); // 9 points
    state.AddScoreToCategory(Category::Fours, 12); // 12 points
    state.AddScoreToCategory(Category::Fives, 15); // 15 points
    state.AddScoreToCategory(Category::Sixes, 18); // 18 points
    // Total: 3+6+9+12+15+18 = 63
    
    EXPECT_EQ(state.GetRemainingUpperBonus(), 0);
    
    // Test with less than bonus threshold
    GameState state2;
    state2.AddScoreToCategory(Category::Ones, 2);
    state2.AddScoreToCategory(Category::Twos, 4);
    // Total: 6, remaining: 63-6 = 57
    EXPECT_EQ(state2.GetRemainingUpperBonus(), 57);
}

TEST(GameStateTest, IsYahtzeeRecorded) {
    GameState state;
    
    // Initially no yahtzee recorded
    EXPECT_FALSE(state.IsYahtzeeRecorded());
    
    // Add zero score to yahtzee (should not count as recorded)
    state.AddScoreToCategory(Category::Yahtzee, 0);
    EXPECT_FALSE(state.IsYahtzeeRecorded());
    
    // Add positive score to yahtzee
    state.AddScoreToCategory(Category::Yahtzee, 50);
    EXPECT_TRUE(state.IsYahtzeeRecorded());
}

TEST(GameStateTest, MixedCategories) {
    GameState state;
    
    // Test upper section categories
    state.AddScoreToCategory(Category::Ones, 3);
    state.AddScoreToCategory(Category::Sixes, 18);
    
    // Test lower section categories
    state.AddScoreToCategory(Category::ThreeOfAKind, 17);
    state.AddScoreToCategory(Category::FullHouse, 25);
    state.AddScoreToCategory(Category::Yahtzee, 50);
    
    // Verify all scores
    EXPECT_EQ(state.GetCategoryScore(Category::Ones).value(), 3);
    EXPECT_EQ(state.GetCategoryScore(Category::Sixes).value(), 18);
    EXPECT_EQ(state.GetCategoryScore(Category::ThreeOfAKind).value(), 17);
    EXPECT_EQ(state.GetCategoryScore(Category::FullHouse).value(), 25);
    EXPECT_EQ(state.GetCategoryScore(Category::Yahtzee).value(), 50);
    
    // Verify unused categories are still empty
    EXPECT_FALSE(state.GetCategoryScore(Category::Twos).has_value());
    EXPECT_FALSE(state.GetCategoryScore(Category::LargeStraight).has_value());
}
