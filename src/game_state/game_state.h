#pragma once

#include "category.h"
#include "dice.h"

#include <optional>

class GameState {
private:
    std::array<std::optional<size_t>, NUM_CATEGORIES> categories_score_{};
    Dice dice_{};
    size_t rerolls_left_{2};

public:
    GameState() = default;
    ~GameState() = default;

    std::optional<size_t> GetCategoryScore(Category category) const;
    void AddScoreToCategory(Category category, size_t score);
    size_t GetRemainingRerolls() const;
    void SetRemainingRerolls(size_t count);
    const Dice &GetCurrentDice() const;
    void SetCurrentDice(const Dice &dice);

    size_t GetRemainingUpperBonus() const;
    bool IsYahtzeeRecorded() const;
};