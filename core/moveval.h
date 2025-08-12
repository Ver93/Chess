#pragma once

#include "state.h"
#include "const.h"

namespace MoveVal {
    
    inline bool isKingInCheckBeforeMove(State& state){
        return state.threatMap[state.turn ^ 1] & state.kingBitMap[state.turn];
    }

    inline bool isKingInCheckAfterMove(State& state){
        return state.threatMap[state.turn] & state.kingBitMap[state.turn ^ 1];
    }

    inline bool isCastlingPathSafe(State& state, int toSquare, const uint64_t threatMap) {
        uint64_t mask = 0;
        switch (toSquare) {
            case Const::SQUARE::G1: mask = Const::W_CASTLE_THREAT_KS; break;
            case Const::SQUARE::C1: mask = Const::W_CASTLE_THREAT_QS; break;
            case Const::SQUARE::G8: mask = Const::B_CASTLE_THREAT_KS; break;
            case Const::SQUARE::C8: mask = Const::B_CASTLE_THREAT_QS; break;
            default: return true;
        }
        return !(threatMap & mask);
    }
}