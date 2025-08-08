#pragma once
#include "State.h"
#include "Utils.h"
#include "AttackGlobals.h"

namespace ThreatGen
{
    inline void updateThreats(State& state) {
        state.threatMap = 0ULL;

        uint64_t opponentPieces = (state.turn == Const::PC_WHITE) ? state.turnOccupancy[Const::PC_WHITE] : state.turnOccupancy[Const::PC_BLACK];
        uint64_t occ = state.occupancy;

        uint64_t temp = opponentPieces;
        while(temp){
        int sq = Utils::popLSB(temp);
        int pieceIndex = state.squareToPieceIndex[sq];
        int pieceType = Const::getPieceType(pieceIndex);
        bool isWhitePiece = pieceIndex < 6;
        

            switch (pieceType) {
                case Const::PT_PAWN: state.threatMap |= AttackGlobals::ad->getPawnTable(sq, isWhitePiece); break;
                case Const::PT_KNIGHT: state.threatMap |= AttackGlobals::ad->getKnightTable(sq); break;
                case Const::PT_BISHOP: state.threatMap |= AttackGlobals::ad->getBishopTable(sq, occ); break;
                case Const::PT_ROOK: state.threatMap |= AttackGlobals::ad->getRookTable(sq, occ); break;
                case Const::PT_QUEEN: state.threatMap |= AttackGlobals::ad->getBishopTable(sq, occ) | AttackGlobals::ad->getRookTable(sq, occ); break;
                case Const::PT_KING: state.threatMap |= AttackGlobals::ad->getKingTable(sq); break;
                default: break;
            }
        }
    }
}
