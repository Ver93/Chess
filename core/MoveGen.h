#pragma once
#include <vector>
#include "State.h"
#include "Move.h"
#include "Utils.h"
namespace MoveGen {

    inline uint64_t getPseudo(int square, int pieceType, bool isWhite, uint64_t occupancy){
        switch (pieceType){
        case Const::PT_PAWN: return AttackGlobals::ad->getPawnTable(square, isWhite);
        case Const::PT_KNIGHT: return AttackGlobals::ad->getKnightTable(square);
        case Const::PT_BISHOP: return AttackGlobals::ad->getBishopTable(square, occupancy);
        case Const::PT_ROOK: return AttackGlobals::ad->getRookTable(square, occupancy);
        case Const::PT_QUEEN: return (AttackGlobals::ad->getBishopTable(square, occupancy) | AttackGlobals::ad->getRookTable(square, occupancy));
        case Const::PT_KING: return AttackGlobals::ad->getKingTable(square);
        default: return 0ULL;
        }
    }

    inline void getPseudoAttacks(int from, int movingPiece, int pieceType, bool isWhite, uint64_t occupancy, uint64_t friendly, State& state, std::vector<Move>& pseudoMoves) {
        uint64_t pseudoAttacks = getPseudo(from, pieceType, isWhite, occupancy);
        pseudoAttacks &= ~friendly;

    switch (pieceType) {
        case Const::PT_PAWN: {
            int singlePush = isWhite ? from + Const::SINGLE_PUSH : from - Const::SINGLE_PUSH;
            int doublePush = isWhite ? from + Const::DOUBLE_PUSH : from - Const::DOUBLE_PUSH;

            bool singlePushBlocked = occupancy & (1ULL << singlePush);
            bool doublePushBlocked = occupancy & (1ULL << doublePush);
            bool isOnStartRank = (isWhite ? Const::RANK_2 : Const::RANK_7) & (1ULL << from);

            if (!singlePushBlocked)
                pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_QUIET));

            if (!singlePushBlocked && !doublePushBlocked && isOnStartRank)
                pseudoMoves.emplace_back(Move(from, doublePush, movingPiece, pieceType, Const::MT_QUIET));

            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                if (state.squareToPieceIndex[to] == Const::W_KING || state.squareToPieceIndex[to] == Const::B_KING)
                    continue;

                if (occupancy & (1ULL << to)) {
                    int capturePiece = state.squareToPieceIndex[to];
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_CAPTURE, capturePiece));
                }
            }
            break;
        }

        case Const::PT_KING: {
            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                if (state.squareToPieceIndex[to] == Const::W_KING || state.squareToPieceIndex[to] == Const::B_KING)
                    continue;

                if (occupancy & (1ULL << to)) {
                    int capturePiece = state.squareToPieceIndex[to];
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_CAPTURE, capturePiece));
                } else {
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_QUIET));
                }
            }
            break;
        }

        default:
            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                if (state.squareToPieceIndex[to] == Const::W_KING || state.squareToPieceIndex[to] == Const::B_KING)
                    continue;

                if (occupancy & (1ULL << to)) {
                    int capturePiece = state.squareToPieceIndex[to];
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_CAPTURE, capturePiece));
                } else {
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_QUIET));
                }
            }
            break;
        }
    }

    inline std::vector<Move> generatePseudoMoves(State& state){
            std::vector<Move> pseudoMoves;
            bool isWhite = (state.turn == Const::PC_WHITE);
            uint64_t friendly = state.turnOccupancy[state.turn];
            uint64_t opponent = state.turnOccupancy[state.turn ^ 1];
            uint64_t occupancy = friendly | opponent;
            uint64_t temp = friendly;
            
            while(temp){    
                int square = Utils::popLSB(temp);
                int movingPiece = state.squareToPieceIndex[square];
                int pieceType = Const::getPieceType(movingPiece);
                getPseudoAttacks(square, movingPiece, pieceType, isWhite, occupancy, friendly, state, pseudoMoves);
            }

        return pseudoMoves;
    }
}