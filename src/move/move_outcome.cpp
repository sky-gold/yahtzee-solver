#include "move_outcome.h"
#include <algorithm>

// Helper function to calculate score for a category based on dice
size_t CalculateScore(const Dice& dice, Category category) {
    const auto& counts = dice.counts();
    
    switch (category) {
        case Category::Ones: return counts[0] * 1;
        case Category::Twos: return counts[1] * 2;
        case Category::Threes: return counts[2] * 3;
        case Category::Fours: return counts[3] * 4;
        case Category::Fives: return counts[4] * 5;
        case Category::Sixes: return counts[5] * 6;
        
        case Category::ThreeOfAKind: 
            for (size_t count : counts) {
                if (count >= 3) {
                    return dice.sum();
                }
            }
            return 0;
            
        case Category::FourOfAKind:
            for (size_t count : counts) {
                if (count >= 4) {
                    return dice.sum();
                }
            }
            return 0;
            
        case Category::FullHouse: {
            bool has_three = false;
            bool has_two = false;
            for (size_t count : counts) {
                if (count == 3) has_three = true;
                if (count == 2) has_two = true;
            }
            // Also check for Yahtzee (5 of a kind) which counts as Full House in some rules
            if (has_three && has_two) return 25;
            for (size_t count : counts) {
                if (count == 5) return 25; // Yahtzee can be used as Full House
            }
            return 0;
        }
            
        case Category::SmallStraight: {
            // Check for sequences of 4 consecutive numbers
            std::array<bool, 6> present{false};
            for (size_t i = 0; i < 6; ++i) {
                if (counts[i] > 0) present[i] = true;
            }
            
            // Check sequences: 1-2-3-4, 2-3-4-5, 3-4-5-6
            if ((present[0] && present[1] && present[2] && present[3]) ||
                (present[1] && present[2] && present[3] && present[4]) ||
                (present[2] && present[3] && present[4] && present[5])) {
                return 30;
            }
            return 0;
        }
            
        case Category::LargeStraight: {
            // Check for sequences of 5 consecutive numbers
            std::array<bool, 6> present{false};
            for (size_t i = 0; i < 6; ++i) {
                if (counts[i] > 0) present[i] = true;
            }
            
            // Check sequences: 1-2-3-4-5, 2-3-4-5-6
            if ((present[0] && present[1] && present[2] && present[3] && present[4]) ||
                (present[1] && present[2] && present[3] && present[4] && present[5])) {
                return 40;
            }
            return 0;
        }
            
        case Category::Yahtzee:
            for (size_t count : counts) {
                if (count == 5) return 50;
            }
            return 0;
            
        case Category::Chance:
            return dice.sum();
            
        default:
            return 0;
    }
}

// Helper function to check if current dice form a yahtzee
bool IsYahtzee(const Dice& dice) {
    const auto& counts = dice.counts();
    for (size_t count : counts) {
        if (count == 5) return true;
    }
    return false;
}

// Template specialization for GameState
template<>
MoveOutcome<GameState> ApplyMove<GameState>(const GameState& state, const Move& move) {
    GameState new_state = state;
    size_t score_delta = 0;
    
    if (std::holds_alternative<ScoreMove>(move)) {
        const ScoreMove& score_move = std::get<ScoreMove>(move);
        Category category = score_move.GetCategory();
        
        // Calculate base score for the category
        size_t base_score = CalculateScore(state.GetCurrentDice(), category);
        
        // Handle Yahtzee bonus rules
        bool is_yahtzee = IsYahtzee(state.GetCurrentDice());
        bool yahtzee_recorded = state.IsYahtzeeRecorded();
        
        if (is_yahtzee && yahtzee_recorded && category != Category::Yahtzee) {
            // Joker rules apply - can use any category
            score_delta = base_score;
            new_state.AddScoreToCategory(category, base_score);
            
            // Check for upper bonus
            if (static_cast<size_t>(category) < 6) { // Upper section category
                size_t current_upper = 0;
                for (size_t i = 0; i < 6; ++i) {
                    if (auto score = new_state.GetCategoryScore(static_cast<Category>(i))) {
                        current_upper += score.value();
                    }
                }
                if (current_upper >= 63) {
                    score_delta += 35; // Upper section bonus
                }
            }
        } else {
            // Normal scoring
            score_delta = base_score;
            new_state.AddScoreToCategory(category, base_score);
            
            // Check for upper bonus
            if (static_cast<size_t>(category) < 6) { // Upper section category
                size_t current_upper = 0;
                for (size_t i = 0; i < 6; ++i) {
                    if (auto score = new_state.GetCategoryScore(static_cast<Category>(i))) {
                        current_upper += score.value();
                    }
                }
                if (current_upper >= 63) {
                    score_delta += 35; // Upper section bonus
                }
            }
            
            // Handle Yahtzee bonus for multiple yahtzees
            if (category == Category::Yahtzee && base_score == 50 && yahtzee_recorded) {
                score_delta += 100; // Yahtzee bonus
            }
        }
    } else if (std::holds_alternative<RerrolMove>(move)) {
        const RerrolMove& reroll_move = std::get<RerrolMove>(move);
        
        // For reroll moves, just decrease reroll count
        // Actual dice rerolling should be handled separately
        size_t current_rerolls = state.GetRemainingRerolls();
        if (current_rerolls > 0) {
            new_state.SetRemainingRerolls(current_rerolls - 1);
        }
        // Note: Dice don't change here - that should be handled by the caller
    } else {
        throw std::invalid_argument("Unknown move type");
    }
    
    return MoveOutcome<GameState>{new_state, score_delta};
}

// Template specialization for ShortGameState
template<>
MoveOutcome<ShortGameState> ApplyMove<ShortGameState>(const ShortGameState& state, const Move& move) {
    ShortGameState new_state = state;
    size_t score_delta = 0;
    
    if (std::holds_alternative<ScoreMove>(move)) {
        const ScoreMove& score_move = std::get<ScoreMove>(move);
        Category category = score_move.GetCategory();
        
        // Calculate base score for the category
        size_t base_score = CalculateScore(state.GetCurrentDice(), category);
        
        // Handle Yahtzee bonus rules
        bool is_yahtzee = IsYahtzee(state.GetCurrentDice());
        bool yahtzee_recorded = state.IsYahtzeeRecorded();
        
        if (is_yahtzee && yahtzee_recorded && category != Category::Yahtzee) {
            // Joker rules apply - can use any category
            score_delta = base_score;
            new_state.AddScoreToCategory(category, base_score);
        } else {
            // Normal scoring
            score_delta = base_score;
            new_state.AddScoreToCategory(category, base_score);
            
            // Handle Yahtzee bonus for multiple yahtzees
            if (category == Category::Yahtzee && base_score == 50 && yahtzee_recorded) {
                score_delta += 100; // Yahtzee bonus
            }
        }
    } else if (std::holds_alternative<RerrolMove>(move)) {
        const RerrolMove& reroll_move = std::get<RerrolMove>(move);
        
        // For reroll moves, just decrease reroll count
        size_t current_rerolls = state.GetRemainingRerolls();
        if (current_rerolls > 0) {
            new_state.SetRemainingRerolls(current_rerolls - 1);
        }
        // Note: Dice don't change here - that should be handled by the caller
    } else {
        throw std::invalid_argument("Unknown move type");
    }
    
    return MoveOutcome<ShortGameState>{new_state, score_delta};
}

// Explicit template instantiation
template struct MoveOutcome<GameState>;
template struct MoveOutcome<ShortGameState>;
