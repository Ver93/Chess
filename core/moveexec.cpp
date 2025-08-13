#include "moveexec.h"

namespace MoveExec {
    void setPiece(State& state, int square, int movingPiece) {
        state.bitboards[movingPiece] |= (1ULL << square);
    }

    void clearPiece(State& state, int square, int movingPiece) {
        state.bitboards[movingPiece] &= ~(1ULL << square);
    }

    void switchTurn(State& state){
        state.turn ^= 1;
    }

    void makeMove(State& state, const Move& move, Undo& undo) {
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
            case Const::MT_PROMOTION:
                clearPiece(state, move.from, move.movingPiece);
                setPiece(state, move.to, move.promotionPiece);
                break;
            case Const::MT_PROMOTION_CAPTURE:
                clearPiece(state, move.from, move.movingPiece);
                clearPiece(state, move.to, move.capturePiece);
                setPiece(state, move.to, move.promotionPiece);
                break;
            default:
                break;
        }

        if(move.movingPiece == Const::W_KING || move.movingPiece == Const::B_KING){
            state.kingMoved[state.turn] = true;
        }

        if(move.movingPiece == Const::B_ROOK || move.movingPiece == Const::W_ROOK){
            if(move.from == Const::SQUARE::A1) state.rooksMoved[Const::PC_WHITE][Const::C_QUEEN_SIDE] = true;
            if(move.from == Const::SQUARE::H1) state.rooksMoved[Const::PC_WHITE][Const::C_KING_SIDE] = true;
            if(move.from == Const::SQUARE::A8) state.rooksMoved[Const::PC_BLACK][Const::C_QUEEN_SIDE] = true;
            if(move.from == Const::SQUARE::H8) state.rooksMoved[Const::PC_BLACK][Const::C_KING_SIDE] = true;
        }

        if(move.capturePiece == Const::B_ROOK || move.capturePiece == Const::W_ROOK){
            if(move.to == Const::SQUARE::A1) state.rooksMoved[Const::PC_WHITE][Const::C_QUEEN_SIDE] = true;
            if(move.to == Const::SQUARE::H1) state.rooksMoved[Const::PC_WHITE][Const::C_KING_SIDE] = true;
            if(move.to == Const::SQUARE::A8) state.rooksMoved[Const::PC_BLACK][Const::C_QUEEN_SIDE] = true;
            if(move.to == Const::SQUARE::H8) state.rooksMoved[Const::PC_BLACK][Const::C_KING_SIDE] = true;
        }

        state.kingBitMap[Const::PC_WHITE] = state.bitboards[Const::W_KING];
        state.kingBitMap[Const::PC_BLACK] = state.bitboards[Const::B_KING];

        Utils::refreshOccupancy(state);
        Utils::refreshSquareToPieceIndex(state);
    }

    void undoMove(State& state, const Undo& undo) {
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