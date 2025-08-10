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
        uint64_t opponent = occupancy & ~friendly;

    switch (pieceType) {
        case Const::PT_PAWN: {
            int singlePush = isWhite ? from + Const::SINGLE_PUSH : from - Const::SINGLE_PUSH;
            int doublePush = isWhite ? from + Const::DOUBLE_PUSH : from - Const::DOUBLE_PUSH;

            bool singlePushBlocked = occupancy & (1ULL << singlePush);
            bool doublePushBlocked = occupancy & (1ULL << doublePush);
            bool isOnStartRank     = (isWhite ? Const::RANK_2 : Const::RANK_7) & (1ULL << from);
            bool canPromotionPush = (isWhite ? Const::RANK_8 : Const::RANK_1) & (1ULL << singlePush);
            
            if (!singlePushBlocked) {
                if(canPromotionPush){
                        pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_PROMOTION, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_KNIGHT : Const::B_KNIGHT, Const::NO_VALUE));
                        pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_PROMOTION, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_BISHOP : Const::B_BISHOP, Const::NO_VALUE));
                        pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_PROMOTION, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_ROOK: Const::B_ROOK, Const::NO_VALUE));
                        pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_PROMOTION, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_QUEEN : Const::B_QUEEN, Const::NO_VALUE));
                } else {
                    pseudoMoves.emplace_back(Move(from, singlePush, movingPiece, pieceType, Const::MT_QUIET));
                }
            }

            if (!singlePushBlocked && !doublePushBlocked && isOnStartRank) pseudoMoves.emplace_back(Move(from, doublePush, movingPiece, pieceType, Const::DOUBLE_PUSH));

            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                int capturePiece = state.squareToPieceIndex[to];
                bool isCapture = opponent & (1ULL << to);
                bool isPromotionCapture = (isWhite ? Const::RANK_8 : Const::RANK_1) & (1ULL << to);
                
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if(to == state.enPassantSquare){
                    int enPassantCapturePiece = state.squareToPieceIndex[state.enPassantTarget]; 
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_ENPASSANT, enPassantCapturePiece, state.enPassantTarget));
                    continue;
                }

                if (isCapture) {
                    if(isPromotionCapture) {
                        pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_PROMOTION_CAPTURE, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_KNIGHT : Const::B_KNIGHT, capturePiece));
                        pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_PROMOTION_CAPTURE, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_BISHOP : Const::B_BISHOP, capturePiece));
                        pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_PROMOTION_CAPTURE, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_ROOK: Const::B_ROOK, capturePiece));
                        pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_PROMOTION_CAPTURE, Const::NO_VALUE, Const::NO_VALUE, Const::NO_VALUE, (isWhite) ? Const::W_QUEEN : Const::B_QUEEN, capturePiece));
                    } else {
                        pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_CAPTURE, capturePiece));
                    }
                    continue;
                }
            }
            break;
        }

        case Const::PT_KING: {
            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                int capturePiece = state.squareToPieceIndex[to];
                bool isCapture = opponent & (1ULL << to);
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if (isCapture) {
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_CAPTURE, capturePiece));
                } else {
                    pseudoMoves.emplace_back(Move(from, to, movingPiece, pieceType, Const::MT_QUIET));
                }
            }

            

            if(state.kingMoved[state.turn]) return;
            if(state.kingBitMap[state.turn] & state.threatMap[state.turn ^ 1]) return;
            uint64_t queenSideSquares = (isWhite) ? ((1ULL << Const::SQ_B1) | (1ULL << Const::SQ_C1) | (1ULL << Const::SQ_D1)) : ((1ULL << Const::SQ_B8) |(1ULL << Const::SQ_C8) | (1ULL << Const::SQ_D8));
            uint64_t kingSideSquares = (isWhite) ? ((1ULL << Const::SQ_F1) | (1ULL << Const::SQ_G1)) : ((1ULL << Const::SQ_F8) | (1ULL << Const::SQ_G8));
            bool isQueenSideBlocked = occupancy & queenSideSquares;
            bool isKingSideBlocked = occupancy & kingSideSquares;

            if(!isQueenSideBlocked && !state.rooksMoved[state.turn][Const::C_QUEEN_SIDE]){
                int rookSquare = isWhite ? Const::SQ_A1 : Const::SQ_A8;
                int rookDestination = isWhite ? Const::SQ_D1 : Const::SQ_D8;
                int kingDestination = isWhite ? Const::SQ_C1 : Const::SQ_C8;
                int rookPiece = isWhite ? Const::W_ROOK : Const::B_ROOK;
                pseudoMoves.emplace_back(Move(from, kingDestination, movingPiece, pieceType, Const::MT_CASTLE, rookSquare, rookDestination, rookPiece));
            }

            if(!isKingSideBlocked && !state.rooksMoved[state.turn][Const::C_KING_SIDE]){
                int rookSquare = isWhite ? Const::SQ_H1 : Const::SQ_H8;
                int rookDestination = isWhite ? Const::SQ_F1 : Const::SQ_F8;
                int kingDestination = isWhite ? Const::SQ_G1 : Const::SQ_G8;
                int rookPiece = isWhite ? Const::W_ROOK : Const::B_ROOK;
                pseudoMoves.emplace_back(Move(from, kingDestination, movingPiece, pieceType, Const::MT_CASTLE, rookSquare, rookDestination, rookPiece));
            }

            break;
        }

        default:
            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                int capturePiece = state.squareToPieceIndex[to];
                bool isCapture = opponent & (1ULL << to);
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if (isCapture) {
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