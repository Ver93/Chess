#pragma once

#include <vector>
#include <algorithm>

#include "state.h"
#include "move.h"
#include "const.h"

namespace MoveOrder {
    inline int scoreSingle(State& state, Move& move){
        int score = 0;
        if(move.movingType == Const::MT_CAPTURE){
            int capturedValue = Const::getPieceValue(move.capturePiece);
            int attackerValue = Const::getPieceValue(move.movingPiece);
            score += 1000 + capturedValue - attackerValue;
        }
        return score;
    }

    inline void scoreMoves(State& state, std::vector<Move>& moves) {
        for (Move& move : moves) {
            move.score = scoreSingle(state, move);
        }
    }

    inline void sortMoves(std::vector<Move>& moves) {
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score > b.score;
        });
    }
}