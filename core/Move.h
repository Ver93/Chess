#pragma once
#include "Const.h"

struct Move {
    Move() = default;
    Move(int f, int t, int mp, int pt, int mt) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt) {}
    Move(int f, int t, int mp, int pt, int mt, int cp) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt), capturePiece(cp) {}
    Move(int f, int t, int mp, int pt, int mt, int ept, int eps) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt), enPassantCapturePiece(ept), enPassantTargetSquare(eps) {}
    Move(int f, int t, int mp, int pt, int mt, int rs, int rd, int rp) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt), rookSquare(rs), rookDestination(rd), rookPiece(rp) {}
    Move(int f, int t, int mp, int pt, int mt, int rs, int rd, int rp, int pp, int cp) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt), rookSquare(rs), rookDestination(rd), rookPiece(rp), promotionPiece(pp), capturePiece(cp) {}

    int from            = Const::NO_VALUE;
    int to              = Const::NO_VALUE;
    int movingPiece     = Const::NO_VALUE;
    int pieceType       = Const::NO_VALUE;
    int movingType      = Const::NO_VALUE;
    int capturePiece    = Const::NO_VALUE;

    int enPassantCapturePiece = Const::NO_VALUE;
    int enPassantTargetSquare = Const::NO_VALUE;

    int rookSquare      = Const::NO_VALUE;
    int rookDestination = Const::NO_VALUE;
    int rookPiece       = Const::NO_VALUE;

    int promotionPiece  = Const::NO_VALUE;
};