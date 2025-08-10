#pragma once
#include "State.h"
#include "Const.h"
#include "Utils.h"
#include "AttackGlobals.h"

namespace ThreatGen
{
    inline void updateThreats(State& state) {
        state.threatMap[Const::PC_WHITE] = 0ULL;
        state.threatMap[Const::PC_BLACK] = 0ULL;

        uint64_t occ = state.occupancy;

        for (int color = 0; color <= 1; ++color) {
            uint64_t pieces = state.turnOccupancy[color];

            uint64_t temp = pieces;
            while (temp) {
                int sq = Utils::popLSB(temp);
                int pieceIndex = state.squareToPieceIndex[sq];
                int pieceType = Const::getPieceType(pieceIndex);
                bool isWhitePiece = (pieceIndex < 6); // 0–5 are white pieces

                switch (pieceType) {
                    case Const::PT_PAWN:
                        state.threatMap[color] |= AttackGlobals::ad->getPawnTable(sq, isWhitePiece);
                        break;
                    case Const::PT_KNIGHT:
                        state.threatMap[color] |= AttackGlobals::ad->getKnightTable(sq);
                        break;
                    case Const::PT_BISHOP:
                        state.threatMap[color] |= AttackGlobals::ad->getBishopTable(sq, occ);
                        break;
                    case Const::PT_ROOK:
                        state.threatMap[color] |= AttackGlobals::ad->getRookTable(sq, occ);
                        break;
                    case Const::PT_QUEEN:
                        state.threatMap[color] |= AttackGlobals::ad->getBishopTable(sq, occ) | AttackGlobals::ad->getRookTable(sq, occ);
                        break;
                    case Const::PT_KING:
                        state.threatMap[color] |= AttackGlobals::ad->getKingTable(sq);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    inline bool isSquareThreatened(const State& state, int square, int byColor) {
        return state.threatMap[byColor] & (1ULL << square);
    }
}