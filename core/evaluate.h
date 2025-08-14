#pragma once
#include "state.h"
#include "const.h"
#include "utils.h"
#include "pst.h"
#include "attackglobals.h"
#include "movegen.h"
#include "evalutils.h"
#include "evalglobals.h"

namespace Evaluate {
    bool isPassedPawn(int square, bool isWhite, uint64_t opponentPawns);
    bool isPawnBlocked(int square, uint64_t opponent);
    bool isPawnIsolated(int square, uint64_t friendly);
    bool isDoubledPawn(int square, uint64_t friendlyPawns);

    int evaluatePawnStructure(int pieceType, int square, bool isWhite, uint64_t opponentPawns, uint64_t friendlyPawns, uint64_t opponent, uint64_t friendly);
    int evaluateKingSafety(int pieceType, int sq, uint64_t king, uint64_t pawns);
    int getPSTBonus(int pieceType, int sq, bool isWhite);
    int evaluateMobility(int pieceType, int sq, bool isWhite, uint64_t ownPieces, uint64_t enemyPieces);
    int evaluateMaterial(State& state, bool isWhite);
    int evaluate(State& state);
    void initialize();
}
