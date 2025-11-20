#include <gtest/gtest.h>

#include "move/move.h"
#include "game_state/game_state.h"
#include "game_state/short_game_state.h"

#include <algorithm>

TEST(MoveTest, RerrolMoveConstructor) {
    std::vector<size_t> keep_values{1, 3, 5};
    RerrolMove move(keep_values);
    
    const auto& result = move.GetKeepValues();
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 3);
    EXPECT_EQ(result[2], 5);
}

TEST(MoveTest, RerrolMoveConstructorInvalid) {
    std::vector<size_t> keep_values{0, 7}; // Invalid dice values
    EXPECT_THROW(RerrolMove move(keep_values), std::out_of_range);
}

TEST(MoveTest, ScoreMoveConstructor) {
    ScoreMove move(Category::Sixes);
    EXPECT_EQ(move.GetCategory(), Category::Sixes);
}

TEST(MoveTest, VariantUsage) {
    std::vector<size_t> keep_values{1, 2};
    Move reroll_move = RerrolMove(keep_values);
    Move score_move = ScoreMove(Category::FullHouse);
    
    EXPECT_TRUE(std::holds_alternative<RerrolMove>(reroll_move));
    EXPECT_TRUE(std::holds_alternative<ScoreMove>(score_move));
}

TEST(MoveTest, GetPossibleMovesNormalCase) {
    GameState state;
    // Set up initial dice
    Dice initial_dice({1, 2, 3, 4, 5});
    state.SetCurrentDice(initial_dice);
    
    // Initially should have both reroll and score moves
    auto moves = GetPossibleMoves(state);
    
    // Should have 32 reroll moves (2^5 combinations) + 13 score moves
    EXPECT_EQ(moves.size(), 32 + 13);
    
    // Test after using some categories
    state.AddScoreToCategory(Category::Ones, 3);
    state.AddScoreToCategory(Category::Yahtzee, 50);
    
    auto moves2 = GetPossibleMoves(state);
    // Should have 32 reroll moves + 11 score moves (13 - 2 used)
    EXPECT_EQ(moves2.size(), 32 + 11);
}

TEST(MoveTest, GetPossibleMovesWithDuplicateDice) {
    GameState state;
    
    // Test with duplicate dice values: three 1s and two 2s
    Dice duplicate_dice({1, 1, 1, 2, 2});
    state.SetCurrentDice(duplicate_dice);
    
    auto moves = GetPossibleMoves(state);
    
    // Count reroll moves - should be unique combinations
    size_t reroll_moves_count = 0;
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            reroll_moves_count++;
        }
    }
    
    // For dice [1,1,1,2,2], unique reroll combinations should be 12
    // (keep 0-3 ones and 0-2 twos, but excluding the case of keeping 0 ones and 0 twos twice)
    EXPECT_EQ(reroll_moves_count, 12);
    
    // Verify all reroll moves are unique
    std::set<std::vector<size_t>> unique_combinations;
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            const auto& reroll_move = std::get<RerrolMove>(move);
            const auto& values = reroll_move.GetKeepValues();
            // Already sorted in the function, but sort again to be sure
            std::vector<size_t> sorted_values = values;
            std::sort(sorted_values.begin(), sorted_values.end());
            unique_combinations.insert(sorted_values);
        }
    }
    
    // Should have 12 unique combinations
    EXPECT_EQ(unique_combinations.size(), 12);
}

TEST(MoveTest, GetPossibleMovesRerrolMoveUnique) {
    GameState state;
    
    // Test with specific duplicate pattern
    Dice dice({1, 1, 2, 2, 3});
    state.SetCurrentDice(dice);
    
    auto moves = GetPossibleMoves(state);
    
    // Count and verify reroll moves are unique
    std::set<std::vector<size_t>> unique_combinations;
    size_t reroll_count = 0;
    
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            reroll_count++;
            const auto& reroll_move = std::get<RerrolMove>(move);
            const auto& values = reroll_move.GetKeepValues();
            std::vector<size_t> sorted_values = values;
            std::sort(sorted_values.begin(), sorted_values.end());
            unique_combinations.insert(sorted_values);
        }
    }
    
    // Should have same count after deduplication
    EXPECT_EQ(reroll_count, unique_combinations.size());
    
    // Verify some specific combinations exist
    bool found_empty = false;
    bool found_all_ones = false;
    bool found_two_ones_one_two = false;
    
    for (const auto& combination : unique_combinations) {
        if (combination.empty()) {
            found_empty = true;
        } else if (combination == std::vector<size_t>{1, 1}) {
            found_all_ones = true;
        } else if (combination == std::vector<size_t>{1, 1, 2}) {
            found_two_ones_one_two = true;
        }
    }
    
    EXPECT_TRUE(found_empty);
    EXPECT_TRUE(found_all_ones);
    EXPECT_TRUE(found_two_ones_one_two);
}

TEST(MoveTest, GetPossibleMovesWithAllSameDice) {
    GameState state;
    
    // Test with all dice having the same value (Yahtzee)
    Dice yahtzee_dice({6, 6, 6, 6, 6});
    state.SetCurrentDice(yahtzee_dice);
    
    auto moves = GetPossibleMoves(state);
    
    // Count reroll moves
    size_t reroll_moves_count = 0;
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            reroll_moves_count++;
        }
    }
    
    // For all same dice [6,6,6,6,6], unique combinations: keep 0-5 dice
    // Total: 6 combinations (keep 0,1,2,3,4,5)
    EXPECT_EQ(reroll_moves_count, 6);
    
    // Verify specific combinations
    std::set<std::vector<size_t>> combinations;
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            const auto& reroll_move = std::get<RerrolMove>(move);
            combinations.insert(reroll_move.GetKeepValues());
        }
    }
    
    EXPECT_TRUE(combinations.count(std::vector<size_t>{}) > 0); // keep none
    EXPECT_TRUE(combinations.count(std::vector<size_t>{6}) > 0); // keep one
    EXPECT_TRUE(combinations.count(std::vector<size_t>{6, 6, 6, 6, 6}) > 0); // keep all
}

TEST(MoveTest, GetPossibleMovesNoRerolls) {
    GameState state;
    // Set up initial dice
    Dice initial_dice({1, 2, 3, 4, 5});
    state.SetCurrentDice(initial_dice);
    state.SetRemainingRerolls(0);
    
    auto moves = GetPossibleMoves(state);
    // Should have only score moves (no reroll moves)
    bool has_reroll_moves = false;
    for (const auto& move : moves) {
        if (std::holds_alternative<RerrolMove>(move)) {
            has_reroll_moves = true;
            break;
        }
    }
    EXPECT_FALSE(has_reroll_moves);
    EXPECT_EQ(moves.size(), 13);
}

TEST(MoveTest, GetPossibleMovesJokerRuleUpperOpen) {
    GameState state;
    
    // Set up yahtzee already recorded and current yahtzee
    state.AddScoreToCategory(Category::Yahtzee, 50);
    Dice yahtzee_dice({6, 6, 6, 6, 6});
    state.SetCurrentDice(yahtzee_dice);
    
    auto moves = GetPossibleMoves(state);
    
    // Should only be able to score in Sixes category (corresponding upper)
    size_t score_moves_count = 0;
    Category allowed_category = Category::Sixes;
    
    for (const auto& move : moves) {
        if (std::holds_alternative<ScoreMove>(move)) {
            score_moves_count++;
            const auto& score_move = std::get<ScoreMove>(move);
            EXPECT_EQ(score_move.GetCategory(), allowed_category);
        }
    }
    
    EXPECT_EQ(score_moves_count, 1);
}

TEST(MoveTest, GetPossibleMovesJokerRuleUpperFilled) {
    GameState state;
    
    // Set up yahtzee already recorded and current yahtzee
    state.AddScoreToCategory(Category::Yahtzee, 50);
    state.AddScoreToCategory(Category::Sixes, 30); // Fill corresponding upper category
    Dice yahtzee_dice({6, 6, 6, 6, 6});
    state.SetCurrentDice(yahtzee_dice);
    
    auto moves = GetPossibleMoves(state);
    
    // Should be able to score in any lower category except Yahtzee
    size_t score_moves_count = 0;
    
    for (const auto& move : moves) {
        if (std::holds_alternative<ScoreMove>(move)) {
            score_moves_count++;
            const auto& score_move = std::get<ScoreMove>(move);
            Category category = score_move.GetCategory();
            // Should be a lower category (ThreeOfAKind through Chance)
            EXPECT_GE(static_cast<size_t>(category), static_cast<size_t>(Category::ThreeOfAKind));
            EXPECT_LE(static_cast<size_t>(category), static_cast<size_t>(Category::Chance));
        }
    }
    
    // There are 7 lower categories total, but Yahtzee is already used
    EXPECT_EQ(score_moves_count, 6);
}

TEST(MoveTest, GetPossibleMovesShortGameState) {
    GameState full_state;
    // Set up initial dice
    Dice initial_dice({1, 2, 3, 4, 5});
    full_state.SetCurrentDice(initial_dice);
    ShortGameState state(full_state);
    
    auto moves = GetPossibleMoves(state);
    EXPECT_EQ(moves.size(), 32 + 13);
}

TEST(MoveTest, GetPossibleMovesWithPartialDice) {
    GameState state;
    // Test with fewer than 5 dice (edge case)
    Dice partial_dice({1, 2, 3});
    state.SetCurrentDice(partial_dice);
    
    auto moves = GetPossibleMoves(state);
    // Should have 2^3 = 8 reroll moves + 13 score moves
    EXPECT_EQ(moves.size(), 8 + 13);
}
