#pragma once

#include "state.h"
#include "const.h"

namespace MoveVal {
    bool isKingInCheckBeforeMove(State& state);
    bool isKingInCheckAfterMove(State& state);
    bool isCastlingPathSafe(int toSquare, const uint64_t threatMap);
}