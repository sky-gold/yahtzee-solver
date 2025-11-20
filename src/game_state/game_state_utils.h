#pragma once

#include "game_state.h"
#include "short_game_state.h"
#include <type_traits>

// Template function to check if a category is used
template<typename GameStateType>
bool IsCategoryUsed(const GameStateType& state, Category category) {
    if constexpr (std::is_same_v<GameStateType, GameState>) {
        return state.GetCategoryScore(category).has_value();
    } else {
        return state.IsCategoryUsed(category);
    }
}

// Template function to get remaining rerolls
template<typename GameStateType>
size_t GetRemainingRerolls(const GameStateType& state) {
    return state.GetRemainingRerolls();
}

// Template function to get current dice
template<typename GameStateType>
const Dice& GetCurrentDice(const GameStateType& state) {
    return state.GetCurrentDice();
}

// Template function to check if yahtzee is recorded
template<typename GameStateType>
bool IsYahtzeeRecorded(const GameStateType& state) {
    if constexpr (std::is_same_v<GameStateType, GameState>) {
        return state.IsYahtzeeRecorded();
    } else {
        return state.IsYahtzeeRecorded();
    }
}

// Template function to get remaining upper bonus
template<typename GameStateType>
size_t GetRemainingUpperBonus(const GameStateType& state) {
    if constexpr (std::is_same_v<GameStateType, GameState>) {
        return state.GetRemainingUpperBonus();
    } else {
        return state.GetRemainingUpperBonus();
    }
}

// Check if current dice form a yahtzee
template<typename GameStateType>
bool IsCurrentDiceYahtzee(const GameStateType& state) {
    const Dice& dice = GetCurrentDice(state);
    for (size_t i = 0; i < 6; ++i) {
        if (dice.counts()[i] == 5) {
            return true;
        }
    }
    return false;
}

// Get the value of yahtzee if current dice form one
template<typename GameStateType>
size_t GetYahtzeeValue(const GameStateType& state) {
    const Dice& dice = GetCurrentDice(state);
    for (size_t i = 0; i < 6; ++i) {
        if (dice.counts()[i] == 5) {
            return i + 1;
        }
    }
    return 0;
}
