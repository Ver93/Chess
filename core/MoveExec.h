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
        return state.threatMap[state.turn] & state.kingBitMap[state.turn ^ 1];
    }
    
    inline bool isCastlingPathSafe(State& state, int toSquare, uint64_t threatMap) {
        uint64_t mask = 0;
        switch (toSquare) {
            case Const::SQ_G1: mask = (1ULL << Const::SQ_E1) | (1ULL << Const::SQ_F1) | (1ULL << Const::SQ_G1); break;
            case Const::SQ_C1: mask = (1ULL << Const::SQ_E1) | (1ULL << Const::SQ_D1) | (1ULL << Const::SQ_C1); break;
            case Const::SQ_G8: mask = (1ULL << Const::SQ_E8) | (1ULL << Const::SQ_F8) | (1ULL << Const::SQ_G8); break;
            case Const::SQ_C8: mask = (1ULL << Const::SQ_E8) | (1ULL << Const::SQ_D8) | (1ULL << Const::SQ_C8); break;
            default: return true;
        }
        return !(threatMap & mask);
    }

    void makeMove(State& state, Move& move, Undo& undo) {
        ::memcpy(undo.bitboards, state.bitboards, 12 * sizeof(uint64_t));
        ::memcpy(undo.squareToPieceIndex, state.squareToPieceIndex, sizeof(state.squareToPieceIndex));
        ::memcpy(undo.turnOccupancy, state.turnOccupancy, sizeof(state.turnOccupancy));
        ::memcpy(undo.kingBitMap, state.kingBitMap, sizeof(state.kingBitMap));
        ::memcpy(undo.kingMoved, state.kingMoved, sizeof(state.kingMoved));
        ::memcpy(undo.rooksMoved, state.rooksMoved, sizeof(state.rooksMoved));
        ::memcpy(undo.threatMap, state.threatMap, sizeof(state.threatMap));

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
            case Const::MT_CASTLE:
                clearPiece(state, move.from, move.movingPiece);
                clearPiece(state, move.rookSquare, move.rookPiece);
                setPiece(state, move.to, move.movingPiece);
                setPiece(state, move.rookDestination, move.rookPiece);
                break;
            default:
                break;
        }

        if(move.movingPiece == Const::W_KING || move.movingPiece == Const::B_KING){
            state.kingMoved[state.turn] = true;
        }

        if(move.movingPiece == Const::B_ROOK || move.movingPiece == Const::W_ROOK){
            if(move.from == Const::SQ_A1) state.rooksMoved[Const::PC_WHITE][Const::C_QUEEN_SIDE] = true;
            if(move.from == Const::SQ_H1) state.rooksMoved[Const::PC_WHITE][Const::C_KING_SIDE] = true;
            if(move.from == Const::SQ_A8) state.rooksMoved[Const::PC_BLACK][Const::C_QUEEN_SIDE] = true;
            if(move.from == Const::SQ_H8) state.rooksMoved[Const::PC_BLACK][Const::C_KING_SIDE] = true;
        }

        if(move.capturePiece == Const::B_ROOK || move.capturePiece == Const::W_ROOK){
            if(move.to == Const::SQ_A1) state.rooksMoved[Const::PC_WHITE][Const::C_QUEEN_SIDE] = true;
            if(move.to == Const::SQ_H1) state.rooksMoved[Const::PC_WHITE][Const::C_KING_SIDE] = true;
            if(move.to == Const::SQ_A8) state.rooksMoved[Const::PC_BLACK][Const::C_QUEEN_SIDE] = true;
            if(move.to == Const::SQ_H8) state.rooksMoved[Const::PC_BLACK][Const::C_KING_SIDE] = true;
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
        ::memcpy(state.kingMoved, undo.kingMoved, sizeof(undo.kingMoved));
        ::memcpy(state.rooksMoved, undo.rooksMoved, sizeof(undo.rooksMoved));
        ::memcpy(state.threatMap, undo.threatMap, sizeof(undo.threatMap));

        state.turn = undo.turn;
        state.enPassantSquare = undo.enPassantSquare;
        state.enPassantTarget = undo.enPassantTarget;

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
    }
}