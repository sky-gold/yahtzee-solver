#pragma once

#include "category.h"
#include "dice.h"
#include "game_state.h"

class ShortGameState {
private:
    std::array<bool, NUM_CATEGORIES> categories_used_{};
    Dice dice_{};
    size_t rerolls_left_{2};
    size_t remaining_upper_bonus_{};
    bool yahtzee_recorded_{};

public:
    ShortGameState() = default;
    ShortGameState(const GameState &full_state);

    ~ShortGameState() = default;

    bool IsCategoryUsed(Category category) const;
    void AddScoreToCategory(Category category, size_t score);
    size_t GetRemainingUpperBonus() const;
    bool IsYahtzeeRecorded() const;
    const Dice &GetCurrentDice() const;
    void SetCurrentDice(const Dice &dice);
    size_t GetRemainingRerolls() const;
    void SetRemainingRerolls(size_t count);
};
