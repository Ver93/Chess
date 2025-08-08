#pragma once
#include "State.h"
#include "Move.h"
#include "Undo.h"
#include "Const.h"
#include "Utils.h"

namespace MoveExec {
    inline void setPiece(State& state, int square, int movingPiece) {
        state.bitboards[movingPiece] |= (1ULL << square);
    }

    inline void clearPiece(State& state, int square, int movingPiece) {
        state.bitboards[movingPiece] &= ~(1ULL << square);
    }

    void makeMove(State& state, Move& move, Undo& undo) {
        ::memcpy(undo.bitboards, state.bitboards, 12 * sizeof(uint64_t));
        ::memcpy(undo.squareToPieceIndex, state.squareToPieceIndex, sizeof(state.squareToPieceIndex));
        ::memcpy(undo.turnOccupancy, state.turnOccupancy, sizeof(state.turnOccupancy));
        undo.turn = state.turn;

        switch (move.movingType) {
            case Const::MT_QUIET:
                clearPiece(state, move.from, move.movingPiece);
                setPiece(state, move.to, move.movingPiece);
                break;
            case Const::MT_CAPTURE:
                clearPiece(state, move.from, move.movingPiece);
                clearPiece(state, move.to, move.capturePiece);
                setPiece(state, move.to, move.movingPiece);
                break;
            default:
                break;
        }

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
        state.turn ^= 1;
    }

    void undoMove(State& state, Undo& undo) {
        ::memcpy(state.bitboards, undo.bitboards, 12 * sizeof(uint64_t));
        ::memcpy(state.squareToPieceIndex, undo.squareToPieceIndex, sizeof(undo.squareToPieceIndex));
        ::memcpy(state.turnOccupancy, undo.turnOccupancy, sizeof(undo.turnOccupancy));
        state.turn = undo.turn;

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
    }
}