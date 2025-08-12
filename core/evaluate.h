#pragma once
#include "state.h"
#include "const.h"
#include "utils.h"

namespace Evalaute {
    inline int evaluateMaterial(State& state, bool isWhite) {
        int score = 0;
        uint64_t occupancy = (isWhite) ? state.turnOccupancy[Const::PC_WHITE]
                                       : state.turnOccupancy[Const::PC_BLACK];

        while (occupancy) {
            int sq = Utils::popLSB(occupancy);
            int pieceIndex = state.squareToPieceIndex[sq];
            int pieceType = Const::getPieceType(pieceIndex);
            score += Const::getPieceValue(pieceType);
        }

        return score;
    }

    inline int evaluate(State& state) {
        bool isWhite = state.turn == Const::PC_WHITE;

        int friendlyScore = evaluateMaterial(state, isWhite);
        int opponentScore = evaluateMaterial(state, !isWhite);

        return friendlyScore - opponentScore;
    }
}