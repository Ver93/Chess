#pragma once
#include <cstdint>
#include "Const.h"

struct State
{
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
    uint64_t threatMap          = Const::NO_MASK;
    uint64_t occupancy          = Const::NO_MASK;
};
