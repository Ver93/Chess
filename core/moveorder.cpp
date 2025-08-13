#include "moveorder.h"

namespace MoveOrder {
    bool isFavorableCapture(const State& state, const Move& move) {
        int attackerValue = Const::getPieceValue(move.movingPiece);
        int victimValue = Const::getPieceValue(move.capturePiece);

        return victimValue >= attackerValue;
    }

    int scoreSingle(State& state, Move& move){
        int score = 0;
        if(move.movingType == Const::MT_CAPTURE){
            int capturedValue = Const::getPieceValue(move.capturePiece);
            int attackerValue = Const::getPieceValue(move.movingPiece);

            if (isFavorableCapture(state, move)) {
                score += 1000 + capturedValue - attackerValue;
            } else {
                if(state.threatMap[state.turn ^ 1] & Const::SQUARE_MASK[move.to]){
                    score = -1000;
                }
                score -= 300;
            }
        }
        return score;
    }

    void scoreMoves(State& state, std::vector<Move>& moves) {
        for (Move& move : moves) {
            move.score = scoreSingle(state, move);
        }
    }

    void sortMoves(std::vector<Move>& moves) {
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score > b.score;
        });
    }
}