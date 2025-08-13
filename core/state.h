#pragma once

#include <cstdint>

#include "const.h"

struct State{
    int turn                    = 0;
    int squareToPieceIndex[64]  = {};
    int enPassantSquare         = Const::NO_VALUE;
    int enPassantTarget         = Const::NO_VALUE;
    uint64_t bitboards[12]      = { Const::NO_MASK, Const::NO_MASK,
                                    Const::NO_MASK, Const::NO_MASK,
                                    Const::NO_MASK, Const::NO_MASK,
                                    Const::NO_MASK, Const::NO_MASK,
                                    Const::NO_MASK, Const::NO_MASK,
                                    Const::NO_MASK, Const::NO_MASK,
                                };

    uint64_t turnOccupancy[2]   = { Const::NO_MASK, Const::NO_MASK };
    uint64_t kingBitMap[2]      = { Const::NO_MASK, Const::NO_MASK };
    uint64_t threatMap[2]       = { Const::NO_MASK, Const::NO_MASK };
    uint64_t occupancy          = Const::NO_MASK;

    bool kingMoved[2]           = { false, false };
    bool rooksMoved[2][2]       = { { false, false }, { false, false } };
};
