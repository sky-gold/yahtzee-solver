#include "game_state.h"

#include <algorithm>
#include <stdexcept>


std::optional<size_t> GameState::GetCategoryScore(Category category) const {
    return categories_score_.at(static_cast<size_t>(category));
}

void GameState::AddScoreToCategory(Category category, size_t score)
{
    categories_score_[static_cast<size_t>(category)] = categories_score_.at(static_cast<size_t>(category)).value_or(0) + score;
}

size_t GameState::GetRemainingRerolls() const
{
    return rerolls_left_;
}

void GameState::SetRemainingRerolls(size_t count)
{
    if (count > 3)
    {
        throw std::out_of_range("Rerolls cannot exceed 3");
    }
    rerolls_left_ = count;
}

const Dice &GameState::GetCurrentDice() const
{
    return dice_;
}

void GameState::SetCurrentDice(const Dice &dice)
{
    dice_ = dice;
}

size_t GameState::GetRemainingUpperBonus() const
{
    size_t upper_score = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        if (categories_score_[i].has_value())
        {
            upper_score += categories_score_[i].value();
        }
    }
    return upper_score >= 63 ? 0 : 63 - upper_score;
}

bool GameState::IsYahtzeeRecorded() const
{
    constexpr size_t yahtzee_category = static_cast<size_t>(Category::Yahtzee);
    return categories_score_[yahtzee_category].has_value() && categories_score_[yahtzee_category].value() > 0;
}