#pragma once

#include <vector>

#include "state.h"
#include "move.h"
#include "utils.h"
#include "attackglobals.h"

namespace MoveGen {
    uint64_t getPseudo(int square, int pieceType, bool isWhite, const uint64_t occupancy);
    void getPseudoAttacks(int from, int movingPiece, int pieceType, bool isWhite, const uint64_t occupancy, const uint64_t friendly, State& state, std::vector<Move>& pseudoMoves);
    std::vector<Move> generatePseudoMoves(State& state);
}