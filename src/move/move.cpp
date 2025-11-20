#include "move.h"
#include "../game_state/game_state_utils.h"

#include <stdexcept>
#include <set>
#include <algorithm>

RerrolMove::RerrolMove(const std::vector<size_t>& keep_values) 
    : keep_values_(keep_values) {
    for (size_t value : keep_values) {
        if (value < 1 || value > 6) {
            throw std::out_of_range("Dice value must be between 1 and 6");
        }
    }
}

const std::vector<size_t>& RerrolMove::GetKeepValues() const {
    return keep_values_;
}

ScoreMove::ScoreMove(Category category) 
    : category_(category) {}

Category ScoreMove::GetCategory() const {
    return category_;
}

// Helper function to generate all possible reroll moves without duplicates
template<typename GameStateType>
std::vector<RerrolMove> GenerateRerollMoves(const GameStateType& state) {
    std::set<std::vector<size_t>> unique_combinations;
    const Dice& dice = GetCurrentDice(state);
    
    // Get all current dice values
    std::vector<size_t> current_values;
    for (size_t value = 1; value <= 6; ++value) {
        for (size_t count = 0; count < dice[value]; ++count) {
            current_values.push_back(value);
        }
    }
    
    // Generate all possible subsets of dice to keep (2^n possibilities)
    size_t n = current_values.size();
    for (size_t mask = 0; mask < (1ULL << n); ++mask) {
        std::vector<size_t> keep_values;
        for (size_t i = 0; i < n; ++i) {
            if (mask & (1ULL << i)) {
                keep_values.push_back(current_values[i]);
            }
        }
        
        // Sort to make combination unique regardless of order
        std::sort(keep_values.begin(), keep_values.end());
        unique_combinations.insert(keep_values);
    }
    
    // Convert set to vector of RerrolMove
    std::vector<RerrolMove> moves;
    for (const auto& combination : unique_combinations) {
        moves.emplace_back(combination);
    }
    
    return moves;
}

// Helper function to check if yahtzee is recorded with positive score
template<typename GameStateType>
bool IsYahtzeeRecordedWithPositiveScore(const GameStateType& state) {
    if constexpr (std::is_same_v<GameStateType, GameState>) {
        auto score = state.GetCategoryScore(Category::Yahtzee);
        return score.has_value() && score.value() > 0;
    } else {
        // For ShortGameState, we have the yahtzee_recorded_ flag
        return state.IsYahtzeeRecorded();
    }
}

// Helper function to generate all possible score moves
template<typename GameStateType>
std::vector<ScoreMove> GenerateScoreMoves(const GameStateType& state) {
    std::vector<ScoreMove> moves;
    
    bool is_yahtzee = IsCurrentDiceYahtzee(state);
    bool yahtzee_recorded_positive = IsYahtzeeRecordedWithPositiveScore(state);
    
    // Check for joker rules (multiple yahtzees)
    if (is_yahtzee && yahtzee_recorded_positive) {
        size_t yahtzee_value = GetYahtzeeValue(state);
        Category corresponding_upper_category = static_cast<Category>(yahtzee_value - 1);
        
        // Joker rule: if corresponding upper category is open, must choose it
        if (!IsCategoryUsed(state, corresponding_upper_category)) {
            moves.emplace_back(corresponding_upper_category);
            return moves;
        } else {
            // If upper category is filled, can choose any lower category
            for (size_t i = static_cast<size_t>(Category::ThreeOfAKind); 
                 i <= static_cast<size_t>(Category::Chance); ++i) {
                Category category = static_cast<Category>(i);
                if (!IsCategoryUsed(state, category)) {
                    moves.emplace_back(category);
                }
            }
            return moves;
        }
    }
    
    // Normal case: can choose any unused category
    for (size_t i = 0; i < NUM_CATEGORIES; ++i) {
        Category category = static_cast<Category>(i);
        if (!IsCategoryUsed(state, category)) {
            moves.emplace_back(category);
        }
    }
    
    return moves;
}

// Main template function to get all possible moves
template<typename GameStateType>
std::vector<Move> GetPossibleMoves(const GameStateType& state) {
    std::vector<Move> moves;
    
    // If we have rerolls left, generate all possible reroll moves
    if (GetRemainingRerolls(state) > 0) {
        auto reroll_moves = GenerateRerollMoves(state);
        for (const auto& reroll_move : reroll_moves) {
            moves.emplace_back(reroll_move);
        }
    }
    
    // Generate all possible scoring moves
    auto score_moves = GenerateScoreMoves(state);
    for (const auto& score_move : score_moves) {
        moves.emplace_back(score_move);
    }
    
    return moves;
}

// Explicit template instantiation
template std::vector<Move> GetPossibleMoves<GameState>(const GameState& state);
template std::vector<Move> GetPossibleMoves<ShortGameState>(const ShortGameState& state);
