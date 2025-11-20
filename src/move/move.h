#pragma once

#include <variant>
#include <vector>
#include <cstddef>
#include "../game_state/category.h"

class RerrolMove {
private:
    std::vector<size_t> keep_values_; // Values of dice to keep

public:
    RerrolMove() = default;
    explicit RerrolMove(const std::vector<size_t>& keep_values);
    
    const std::vector<size_t>& GetKeepValues() const;
};

class ScoreMove {
private:
    Category category_; // Category to score in

public:
    ScoreMove() = default;
    explicit ScoreMove(Category category);
    
    Category GetCategory() const;
};

using Move = std::variant<RerrolMove, ScoreMove>;

// Template function to get all possible moves from a game state
template<typename GameStateType>
std::vector<Move> GetPossibleMoves(const GameStateType& state);