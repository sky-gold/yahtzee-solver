#include <gtest/gtest.h>
#include "game_state/short_game_state.h"
#include "game_state/game_state.h"

TEST(ShortGameStateTest, DefaultConstructor) {
    ShortGameState state;
    
    // Check default values
    EXPECT_EQ(state.GetRemainingRerolls(), 2);
    EXPECT_FALSE(state.IsYahtzeeRecorded());
    EXPECT_EQ(state.GetCurrentDice().total(), 0);
}

TEST(ShortGameStateTest, ConstructorFromGameState) {
    GameState full_state;
    
    // Set up some scores in full state
    full_state.AddScoreToCategory(Category::Ones, 3);
    full_state.AddScoreToCategory(Category::Yahtzee, 50);
    full_state.SetRemainingRerolls(1);
    
    Dice dice({1, 2, 3, 4, 5});
    full_state.SetCurrentDice(dice);
    
    // Create short state from full state
    ShortGameState short_state(full_state);
    
    // Test category usage
    EXPECT_TRUE(short_state.IsCategoryUsed(Category::Ones));
    EXPECT_TRUE(short_state.IsCategoryUsed(Category::Yahtzee));
    EXPECT_FALSE(short_state.IsCategoryUsed(Category::Twos));
    EXPECT_FALSE(short_state.IsCategoryUsed(Category::FullHouse));
    
    // Test other properties
    EXPECT_EQ(short_state.GetRemainingRerolls(), 1);
    EXPECT_TRUE(short_state.IsYahtzeeRecorded());
    EXPECT_EQ(short_state.GetCurrentDice().total(), 5);
}

TEST(ShortGameStateTest, AddScoreToCategory) {
    ShortGameState state;
    
    // Initially category should not be used
    EXPECT_FALSE(state.IsCategoryUsed(Category::Ones));
    
    // Add score to category (marks it as used)
    state.AddScoreToCategory(Category::Ones, 3);
    EXPECT_TRUE(state.IsCategoryUsed(Category::Ones));
    
    // Test yahtzee recording
    EXPECT_FALSE(state.IsYahtzeeRecorded());
    state.AddScoreToCategory(Category::Yahtzee, 50);
    EXPECT_TRUE(state.IsYahtzeeRecorded());
}

TEST(ShortGameStateTest, SetAndGetRerolls) {
    ShortGameState state;
    
    EXPECT_EQ(state.GetRemainingRerolls(), 2);
    
    state.SetRemainingRerolls(0);
    EXPECT_EQ(state.GetRemainingRerolls(), 0);
    
    state.SetRemainingRerolls(3);
    EXPECT_EQ(state.GetRemainingRerolls(), 3);
    
    // Test invalid reroll count
    EXPECT_THROW(state.SetRemainingRerolls(4), std::out_of_range);
}

TEST(ShortGameStateTest, SetAndGetDice) {
    ShortGameState state;
    
    Dice dice({6, 6, 6, 1, 2});
    state.SetCurrentDice(dice);
    
    const Dice& current_dice = state.GetCurrentDice();
    EXPECT_EQ(current_dice.total(), 5);
    EXPECT_EQ(current_dice[6], 3);
    EXPECT_EQ(current_dice[1], 1);
    EXPECT_EQ(current_dice[2], 1);
}

TEST(ShortGameStateTest, RemainingUpperBonus) {
    GameState full_state;
    
    // Set up upper section scores that don't reach bonus
    full_state.AddScoreToCategory(Category::Ones, 2);
    full_state.AddScoreToCategory(Category::Twos, 4);
    // Total: 6, remaining: 57
    
    ShortGameState short_state(full_state);
    EXPECT_EQ(short_state.GetRemainingUpperBonus(), 57);
}

TEST(ShortGameStateTest, YahtzeeRecordedFlag) {
    ShortGameState state;
    
    // Initially not recorded
    EXPECT_FALSE(state.IsYahtzeeRecorded());
    
    // Set through constructor from GameState with yahtzee
    GameState full_state_with_yahtzee;
    full_state_with_yahtzee.AddScoreToCategory(Category::Yahtzee, 50);
    
    ShortGameState state_with_yahtzee(full_state_with_yahtzee);
    EXPECT_TRUE(state_with_yahtzee.IsYahtzeeRecorded());
    
    // Set through AddScoreToCategory
    state.AddScoreToCategory(Category::Yahtzee, 50);
    EXPECT_TRUE(state.IsYahtzeeRecorded());
}

TEST(ShortGameStateTest, CategoryUsageTracking) {
    ShortGameState state;
    
    // Initially no categories used
    for (size_t i = 0; i < NUM_CATEGORIES; ++i) {
        EXPECT_FALSE(state.IsCategoryUsed(static_cast<Category>(i)));
    }
    
    // Use some categories
    state.AddScoreToCategory(Category::Ones, 3);
    state.AddScoreToCategory(Category::FullHouse, 25);
    state.AddScoreToCategory(Category::Chance, 18);
    
    // Check used categories
    EXPECT_TRUE(state.IsCategoryUsed(Category::Ones));
    EXPECT_TRUE(state.IsCategoryUsed(Category::FullHouse));
    EXPECT_TRUE(state.IsCategoryUsed(Category::Chance));
    
    // Check some unused categories
    EXPECT_FALSE(state.IsCategoryUsed(Category::Twos));
    EXPECT_FALSE(state.IsCategoryUsed(Category::Yahtzee));
    EXPECT_FALSE(state.IsCategoryUsed(Category::LargeStraight));
}
