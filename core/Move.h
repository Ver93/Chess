#pragma once

#include "const.h"

struct Move {
    Move() = default;
    Move(int f, int t, int mp, int pt, int mt) : from(f), to(t), movingPiece(mp), pieceType(pt), movingType(mt) {}
    
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

    int score           = Const::NO_VALUE;

    inline static Move quiet(int from, int to, int mp, int pt) {
        return Move{from, to, mp, pt, Const::MT_QUIET};
    }

    inline static Move capture(int from, int to, int mp, int pt, int cp) {
        Move m{from, to, mp, pt, Const::MT_CAPTURE};
        m.capturePiece = cp;
        return m;
    }

    inline static Move doublePush(int from, int to, int mp, int pt) {
        return Move{from, to, mp, pt, Const::MT_DOUBLEPUSH};
    }

    inline static Move enPassant(int from, int to, int mp, int pt, int ept, int eps) {
        Move m{from, to, mp, pt, Const::MT_ENPASSANT};
        m.enPassantCapturePiece = ept;
        m.enPassantTargetSquare = eps;
        return m;
    }

    inline static Move castle(int from, int to, int mp, int pt, int rs, int rd, int rp) {
        Move m{from, to, mp, pt, Const::MT_CASTLE};
        m.rookSquare = rs;
        m.rookDestination = rd;
        m.rookPiece = rp;
        return m;
    }

    inline static Move promotion(int from, int to, int mp, int pt, int pp) {
        Move m{from, to, mp, pt, Const::MT_PROMOTION};
        m.promotionPiece = pp;
        return m;
    }

    inline static Move promotionCapture(int from, int to, int mp, int pt, int pp, int cp) {
        Move m{from, to, mp, pt, Const::MT_PROMOTION_CAPTURE};
        m.promotionPiece = pp;
        m.capturePiece = cp;
        return m;
    }

};