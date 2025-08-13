#pragma once
#include "state.h"
#include "const.h"
#include "utils.h"
#include "pst.h"
#include "attackglobals.h"
#include "movegen.h"

namespace Evalaute {

    int getPassedPawnBonus(int pieceType, int square, uint64_t opponent, uint64_t friendly);
    
    int evaluateKingSafety(int pieceType, int sq, bool isWhite, uint64_t king, uint64_t pawns);
    
    int getPSTBonus(int pieceType, int sq, bool isWhite);
    
    int evaluateMobility(int pieceType, int sq, bool isWhite, uint64_t ownPieces, uint64_t enemyPieces);
    
    int evaluateMaterial(State& state, bool isWhite);
    
    int evaluate(State& state);
}
