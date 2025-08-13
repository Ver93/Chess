#pragma once

#include <vector>
#include <algorithm>

#include "state.h"
#include "move.h"
#include "const.h"

namespace MoveOrder {
    bool isFavorableCapture(const State& state, const Move& move);
    int scoreSingle(State& state, Move& move);
    void scoreMoves(State& state, std::vector<Move>& moves);
    void sortMoves(std::vector<Move>& moves);
}