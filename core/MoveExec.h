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

    inline bool isOpponentKingInCheck(State& state){
        return state.threatMap & state.kingBitMap[state.turn ^ 1];
    }

    void makeMove(State& state, Move& move, Undo& undo) {
        ::memcpy(undo.bitboards, state.bitboards, 12 * sizeof(uint64_t));
        ::memcpy(undo.squareToPieceIndex, state.squareToPieceIndex, sizeof(state.squareToPieceIndex));
        ::memcpy(undo.turnOccupancy, state.turnOccupancy, sizeof(state.turnOccupancy));
        ::memcpy(undo.kingBitMap, state.kingBitMap, sizeof(state.kingBitMap));
        undo.turn = state.turn;
        undo.enPassantSquare = state.enPassantSquare;
        undo.enPassantTarget = state.enPassantTarget;

        state.enPassantSquare = Const::NO_VALUE;
        state.enPassantTarget = Const::NO_VALUE;

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
            case Const::DOUBLE_PUSH:
                clearPiece(state, move.from, move.movingPiece);
                setPiece(state, move.to, move.movingPiece);
                state.enPassantSquare = (state.turn == Const::PC_WHITE) ? move.to - Const::SINGLE_PUSH : move.to + Const::SINGLE_PUSH;
                state.enPassantTarget = move.to;
                break;
            case Const::MT_ENPASSANT:
                clearPiece(state, move.from, move.movingPiece);
                clearPiece(state, move.enPassantTargetSquare, move.enPassantCapturePiece);
                setPiece(state, move.to, move.movingPiece);
                break;
            default:
                break;
        }

        state.kingBitMap[Const::PC_WHITE] = state.bitboards[Const::W_KING];
        state.kingBitMap[Const::PC_BLACK] = state.bitboards[Const::B_KING];

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
        state.turn ^= 1;
    }

    void undoMove(State& state, Undo& undo) {
        ::memcpy(state.bitboards, undo.bitboards, 12 * sizeof(uint64_t));
        ::memcpy(state.squareToPieceIndex, undo.squareToPieceIndex, sizeof(undo.squareToPieceIndex));
        ::memcpy(state.turnOccupancy, undo.turnOccupancy, sizeof(undo.turnOccupancy));
        ::memcpy(state.kingBitMap, undo.kingBitMap, sizeof(undo.kingBitMap));
        state.turn = undo.turn;
        state.enPassantSquare = undo.enPassantSquare;
        state.enPassantTarget = undo.enPassantTarget;

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
    }
}