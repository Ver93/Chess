#include "evaluate.h"

namespace Evaluate {

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

    bool isPassedPawn(int square, bool isWhite, uint64_t opponentPawns){
        uint64_t passedMask = (isWhite) ? EvalGlobals::ed.whitePawnPassedMask[square] : EvalGlobals::ed.blackPawnPassedMask[square]; 
        return !(passedMask & opponentPawns);
    }

    bool isPawnBlocked(int square, uint64_t opponent){
        uint64_t fileMask = EvalGlobals::ed.singleFileMask[square];
        return (fileMask & opponent);
    }

    bool isPawnIsolated(int square, uint64_t friendly){
        uint64_t relevantFiles = EvalGlobals::ed.adjacentPawnMask[square];
        return !(relevantFiles & friendly);
    }

    bool isDoubledPawn(int square, uint64_t friendlyPawns) {
        uint64_t fileMask = EvalGlobals::ed.singleFileMask[square];
        int pawns = Utils::countBits(fileMask & friendlyPawns);
        return pawns > 1;
    }


    int evaluatePawnStructure(int pieceType, int square, bool isWhite, uint64_t opponentPawns, uint64_t friendlyPawns, uint64_t opponent, uint64_t friendly) {
        if (pieceType != Const::PT_PAWN) return 0;
        int score = 0;

        bool passed   = isPassedPawn(square, isWhite, opponentPawns);
        bool blocked  = isPawnBlocked(square, opponent);
        bool isolated = isPawnIsolated(square, friendly);
        bool doubled  = isDoubledPawn(square, friendlyPawns);

        if (passed) {
            score += 60; 
            if (!blocked) score += 20;
            if (isolated) score -= 10;
        } else {
            score -= 10;
        }

        score += blocked ? -30 : 10;
        score += isolated ? -20 : 10;
        score += doubled ? -15 : 5;

        return score;
    }

    int evaluateMaterial(State& state, bool isWhite) {
        int score = 0;

        uint64_t temp     = (isWhite) ? state.turnOccupancy[Const::PC_WHITE]
                                      : state.turnOccupancy[Const::PC_BLACK];

        uint64_t friendly = temp;
        uint64_t opponent = (isWhite) ? state.turnOccupancy[Const::PC_BLACK]
                                      : state.turnOccupancy[Const::PC_WHITE];

        uint64_t king     = state.kingBitMap[state.turn]; 

        uint64_t friendlyPawns    = (isWhite) ? state.bitboards[Const::W_PAWN]
                                      : state.bitboards[Const::B_PAWN];

        uint64_t opponentPawns    = (isWhite) ? state.bitboards[Const::B_PAWN]
                                      : state.bitboards[Const::W_PAWN];

        while (temp) {
            int sq = Utils::popLSB(temp);
            int pieceIndex = state.squareToPieceIndex[sq];
            int pieceType = Const::getPieceType(pieceIndex);

            score += Const::getPieceValue(pieceType);
            score += getPSTBonus(pieceType, sq, isWhite);
            score += evaluateKingSafety(pieceType, sq, king, friendlyPawns);
            score += evaluateMobility(pieceType, sq, isWhite, friendly, opponent);
            score += evaluatePawnStructure(pieceType, sq, isWhite, opponentPawns, friendlyPawns, opponent, friendly);
        }

        return score;
    }

    int evaluate(State& state) {
        bool isWhite = state.turn == Const::PC_WHITE;

        int friendlyScore = evaluateMaterial(state, isWhite);
        int opponentScore = evaluateMaterial(state, !isWhite);

        return friendlyScore - opponentScore;
    }

    void initialize(){
        for (int square = 0; square < 64; square++){
            EvalGlobals::ed.singleFileMask[square] = EvalUtils::generateRelevantSingleFileMask(square);
            EvalGlobals::ed.adjacentPawnMask[square] = EvalUtils::generateRelevantAdjacentFileMask(square);
            EvalGlobals::ed.whitePawnPassedMask[square] = EvalUtils::generatePassedPawnMask(square, true);
            EvalGlobals::ed.blackPawnPassedMask[square] = EvalUtils::generatePassedPawnMask(square, false);
        }
        
    }
}