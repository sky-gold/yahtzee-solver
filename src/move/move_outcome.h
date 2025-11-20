#pragma once

#include "move.h"
#include "../game_state/game_state.h"
#include "../game_state/short_game_state.h"
#include <stdexcept>

template<typename GameStateType>
struct MoveOutcome {
    GameStateType new_state;
    size_t score_delta;
};

// Declaration of ApplyMove function
template<typename GameStateType>
MoveOutcome<GameStateType> ApplyMove(const GameStateType& state, const Move& move);
