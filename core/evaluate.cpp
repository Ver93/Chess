#include "evaluate.h"

namespace Evalaute {

    int getPassedPawnBonus(int pieceType, int square, uint64_t opponent, uint64_t friendly){
        if(pieceType != Const::PT_PAWN) return 0;
        int score = 0;
        uint64_t passed = Const::getPassedPawnFileMask(square);
        if(opponent & passed) return score;
        score += 50;
        if(!(friendly & passed)) score -= 25;
        return score;
    }

    int evaluateKingSafety(int pieceType, int sq, uint64_t king, uint64_t pawns) {
        if (pieceType != Const::PT_KING) return 0;
        uint64_t kingZone = AttackGlobals::ad->getKingTable(sq);
        uint64_t safety = (kingZone & pawns) & ~king;
        int pawnCount = Utils::countBits(safety);
        return pawnCount * 12 - (8 - pawnCount) * 8;
    }

    int getPSTBonus(int pieceType, int sq, bool isWhite) {
        int mirroredSq = isWhite ? sq : Utils::mirror(sq);
        switch (pieceType) {
            case Const::PT_PAWN:   return PST::pawn[mirroredSq];
            case Const::PT_KNIGHT: return PST::knight[mirroredSq];
            case Const::PT_BISHOP: return PST::bishop[mirroredSq];
            case Const::PT_ROOK:   return PST::rook[mirroredSq];
            case Const::PT_QUEEN:  return PST::queen[mirroredSq];
            case Const::PT_KING:   return PST::king[mirroredSq];
            default:               return 0;
        }
    }

    int evaluateMobility(int pieceType, int sq, bool isWhite, uint64_t ownPieces, uint64_t enemyPieces) {
        uint64_t moves = 0;
        switch (pieceType) {
            case Const::PT_KNIGHT:
                moves = MoveGen::getPseudo(sq, pieceType, isWhite, (ownPieces | enemyPieces));
                return Utils::countBits(moves) * Const::WT_KNIGHT;
            case Const::PT_BISHOP:
                moves = MoveGen::getPseudo(sq, pieceType, isWhite, (ownPieces | enemyPieces));
                return Utils::countBits(moves) * Const::WT_BISHOP;
            case Const::PT_ROOK:
                moves = MoveGen::getPseudo(sq, pieceType, isWhite, (ownPieces | enemyPieces));
                return Utils::countBits(moves) * Const::WT_ROOK;
            case Const::PT_QUEEN:
                moves = MoveGen::getPseudo(sq, pieceType, isWhite, (ownPieces | enemyPieces));
                return Utils::countBits(moves) * Const::WT_QUEEN;
            default:
                return 0;
        }
    }

    int evaluateMaterial(State& state, bool isWhite) {
        int score = 0;

        uint64_t temp     = (isWhite) ? state.turnOccupancy[Const::PC_WHITE]
                                      : state.turnOccupancy[Const::PC_BLACK];

        uint64_t friendly = temp;
        uint64_t opponent = (isWhite) ? state.turnOccupancy[Const::PC_BLACK]
                                      : state.turnOccupancy[Const::PC_WHITE];

        uint64_t king     = state.kingBitMap[state.turn]; 
        uint64_t pawns    = (isWhite) ? state.bitboards[Const::W_PAWN]
                                      : state.bitboards[Const::B_PAWN];

        while (temp) {
            int sq = Utils::popLSB(temp);
            int pieceIndex = state.squareToPieceIndex[sq];
            int pieceType = Const::getPieceType(pieceIndex);

            score += Const::getPieceValue(pieceType);
            score += getPSTBonus(pieceType, sq, isWhite);
            score += evaluateKingSafety(pieceType, sq, king, pawns);
            score += getPassedPawnBonus(pieceType, sq, opponent, friendly);
            score += evaluateMobility(pieceType, sq, isWhite, friendly, opponent);
        }

        return score;
    }

    int evaluate(State& state) {
        bool isWhite = state.turn == Const::PC_WHITE;

        int friendlyScore = evaluateMaterial(state, isWhite);
        int opponentScore = evaluateMaterial(state, !isWhite);

        return friendlyScore - opponentScore;
    }
}