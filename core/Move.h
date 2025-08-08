#pragma once
#include "Const.h"

struct Move {
    Move() = default;
    Move(int f, int t, int mp, int pt, int mt) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt) {}
    Move(int f, int t, int mp, int pt, int mt, int cp) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt), capturePiece(cp) {}

    int from            = Const::NO_VALUE;
    int to              = Const::NO_VALUE;
    int movingPiece     = Const::NO_VALUE;
    int pieceType       = Const::NO_VALUE;
    int movingType      = Const::NO_VALUE;
    int capturePiece    = Const::NO_VALUE;
};