#include "movegen.h"

namespace MoveGen {
    uint64_t getPseudo(int square, int pieceType, bool isWhite, const uint64_t occupancy){
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

    void getPseudoAttacks(int from, int movingPiece, int pieceType, bool isWhite, const uint64_t occupancy, const uint64_t friendly, State& state, std::vector<Move>& pseudoMoves) {
        uint64_t pseudoAttacks = getPseudo(from, pieceType, isWhite, occupancy);
        pseudoAttacks &= ~friendly;

    switch (pieceType) {
        case Const::PT_PAWN: {
            int singlePush = isWhite ? from + Const::SINGLE_PUSH : from - Const::SINGLE_PUSH;
            int doublePush = isWhite ? from + Const::DOUBLE_PUSH : from - Const::DOUBLE_PUSH;

            bool singlePushBlocked = occupancy & Const::SQUARE_MASK[singlePush];
            bool doublePushBlocked = occupancy & Const::SQUARE_MASK[doublePush];
            bool isOnStartRank     = (isWhite ? Const::RANK_2 : Const::RANK_7) & Const::SQUARE_MASK[from];
            bool canPromotionPush = (isWhite ? Const::RANK_8 : Const::RANK_1) & Const::SQUARE_MASK[singlePush];
            
            if (!singlePushBlocked) {
                if(canPromotionPush){
                    for (int i = 0; i < Const::NUM_PROMOTION_PIECES; ++i) {
                        int promoPiece = Const::getPromotionPieces(isWhite)[i];
                        pseudoMoves.emplace_back(Move::promotion(from, singlePush, movingPiece, pieceType, promoPiece));
                    }
                } else {
                    pseudoMoves.emplace_back(Move::quiet(from, singlePush, movingPiece, pieceType));
                }
            }

            if (!singlePushBlocked && !doublePushBlocked && isOnStartRank) pseudoMoves.emplace_back(Move(from, doublePush, movingPiece, pieceType, Const::DOUBLE_PUSH));

            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                int capturePiece = state.squareToPieceIndex[to];
                bool isCapture = capturePiece != Const::NO_VALUE;
                
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if(to == state.enPassantSquare){
                    int enPassantCapturePiece = state.squareToPieceIndex[state.enPassantTarget]; 
                    pseudoMoves.emplace_back(Move::enPassant(from, to, movingPiece, pieceType, enPassantCapturePiece, state.enPassantTarget));
                    continue;
                }

                if (isCapture) {
                    bool isPromotionCapture = (isWhite ? Const::RANK_8 : Const::RANK_1) & Const::SQUARE_MASK[to];
                    if(isPromotionCapture) {
                        for (int i = 0; i < Const::NUM_PROMOTION_PIECES; ++i) {
                            int promoPiece = Const::getPromotionPieces(isWhite)[i];
                            pseudoMoves.emplace_back(Move::promotionCapture(from, to, movingPiece, pieceType, promoPiece, capturePiece));
                        }
                    } else {
                        pseudoMoves.emplace_back(Move::capture(from, to, movingPiece, pieceType, capturePiece));
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
                bool isCapture = capturePiece != Const::NO_VALUE;
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if (isCapture) {
                    pseudoMoves.emplace_back(Move::capture(from, to, movingPiece, pieceType, capturePiece));
                } else {
                    pseudoMoves.emplace_back(Move::quiet(from, to, movingPiece, pieceType));
                }
            }

            

            if(state.kingMoved[state.turn]) return;
            if(state.kingBitMap[state.turn] & state.threatMap[state.turn ^ 1]) return;
            uint64_t queenSideSquares = (isWhite) ? Const::W_CASTLE_BLOCKER_QS : Const::B_CASTLE_BLOCKER_QS;
            uint64_t kingSideSquares = (isWhite) ? Const::W_CASTLE_BLOCKER_KS : Const::B_CASTLE_BLOCKER_KS;
            bool isQueenSideBlocked = occupancy & queenSideSquares;
            bool isKingSideBlocked = occupancy & kingSideSquares;

            if(!isQueenSideBlocked && !state.rooksMoved[state.turn][Const::C_QUEEN_SIDE]){
                int rookSquare = isWhite ? Const::SQUARE::A1 : Const::SQUARE::A8;
                int rookDestination = isWhite ? Const::SQUARE::D1 : Const::SQUARE::D8;
                int kingDestination = isWhite ? Const::SQUARE::C1 : Const::SQUARE::C8;
                int rookPiece = isWhite ? Const::W_ROOK : Const::B_ROOK;
                pseudoMoves.emplace_back(Move::castle(from, kingDestination, movingPiece, pieceType, rookSquare, rookDestination, rookPiece));
            }

            if(!isKingSideBlocked && !state.rooksMoved[state.turn][Const::C_KING_SIDE]){
                int rookSquare = isWhite ? Const::SQUARE::H1 : Const::SQUARE::H8;
                int rookDestination = isWhite ? Const::SQUARE::F1 : Const::SQUARE::F8;
                int kingDestination = isWhite ? Const::SQUARE::G1 : Const::SQUARE::G8;
                int rookPiece = isWhite ? Const::W_ROOK : Const::B_ROOK;
                pseudoMoves.emplace_back(Move::castle(from, kingDestination, movingPiece, pieceType, rookSquare, rookDestination, rookPiece));
            }

            break;
        }

        default:
            while (pseudoAttacks) {
                int to = Utils::popLSB(pseudoAttacks);
                int capturePiece = state.squareToPieceIndex[to];
                bool isCapture = capturePiece != Const::NO_VALUE;
                if (capturePiece == Const::W_KING || capturePiece == Const::B_KING)
                    continue;

                if (isCapture) {
                    pseudoMoves.emplace_back(Move::capture(from, to, movingPiece, pieceType, capturePiece));
                } else {
                    pseudoMoves.emplace_back(Move::quiet(from, to, movingPiece, pieceType));
                }
            }
            break;
        }
    }

    std::vector<Move> generatePseudoMoves(State& state){
            std::vector<Move> pseudoMoves;
            const bool isWhite = (state.turn == Const::PC_WHITE);
            const uint64_t friendly = state.turnOccupancy[state.turn];
            const uint64_t opponent = state.turnOccupancy[state.turn ^ 1];
            const uint64_t occupancy = friendly | opponent;
            uint64_t temp = friendly;
            
            while(temp){    
                const int square = Utils::popLSB(temp);
                const int movingPiece = state.squareToPieceIndex[square];
                const int pieceType = Const::getPieceType(movingPiece);
                getPseudoAttacks(square, movingPiece, pieceType, isWhite, occupancy, friendly, state, pseudoMoves);
            }

        return pseudoMoves;
    }
}