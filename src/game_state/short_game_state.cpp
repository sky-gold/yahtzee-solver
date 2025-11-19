#include "short_game_state.h"

#include <algorithm>
#include <stdexcept>

ShortGameState::ShortGameState(const GameState &full_state) {
    for (size_t i = 0; i < NUM_CATEGORIES; ++i) {
        categories_used_[i] = full_state.GetCategoryScore(static_cast<Category>(i)).has_value();
    }
    dice_ = full_state.GetCurrentDice();
    rerolls_left_ = full_state.GetRemainingRerolls();
    remaining_upper_bonus_ = full_state.GetRemainingUpperBonus();
    yahtzee_recorded_ = full_state.IsYahtzeeRecorded();
}

bool ShortGameState::IsCategoryUsed(Category category) const {
    return categories_used_[static_cast<size_t>(category)];
}

void ShortGameState::AddScoreToCategory(Category category, size_t score) {
    if (category == Category::Yahtzee && score > 0) {
        yahtzee_recorded_ = true;
    }
    categories_used_[static_cast<size_t>(category)] = true; // Mark category as used
}

size_t ShortGameState::GetRemainingUpperBonus() const {
    return remaining_upper_bonus_;
}

bool ShortGameState::IsYahtzeeRecorded() const {
    return yahtzee_recorded_;
}

const Dice &ShortGameState::GetCurrentDice() const {
    return dice_;
}

void ShortGameState::SetCurrentDice(const Dice &dice) {
    dice_ = dice;
}

size_t ShortGameState::GetRemainingRerolls() const {
    return rerolls_left_;
}

void ShortGameState::SetRemainingRerolls(size_t count) {
    if (count > 3) {
        throw std::out_of_range("Rerolls cannot exceed 3");
    }
    rerolls_left_ = count;
}