#pragma once
#include <cstdint>
#include "Const.h"

struct AttackData{
    uint64_t bishopAttackTable[64][Const::MAX_BISHOP_ENTRIES];
    uint64_t rookAttackTable[64][Const::MAX_ROOK_ENTRIES];

    uint64_t bishopBlockerMasks[64];
    uint64_t bishopShiftAmount[64];
    uint64_t bishopMagics[64];
    int bishopBits[64];

    uint64_t rookBlockerMasks[64];
    uint64_t rookShiftAmount[64];
    uint64_t rookMagics[64];
    int rookBits[64];

    uint64_t whitePawnMoveTable[64];
    uint64_t blackPawnMoveTable[64];
    uint64_t whitePawnAttackTable[64];
    uint64_t blackPawnAttackTable[64];
    uint64_t knightAttackTable[64];
    uint64_t kingAttackTable[64];

    inline uint64_t getPawnTable(int square, bool isWhite){ return (isWhite) ? whitePawnAttackTable[square] : blackPawnAttackTable[square]; };
    inline uint64_t getKnightTable(int square){ return knightAttackTable[square]; };
    inline uint64_t getKingTable(int square){ return kingAttackTable[square]; };

    inline uint64_t getBishopTable(int square, uint64_t occ){
        uint64_t blockers = occ & bishopBlockerMasks[square];
        uint64_t index = (blockers * bishopMagics[square]) >> bishopShiftAmount[square];
        return bishopAttackTable[square][index];
    }

    inline uint64_t getRookTable(int square, uint64_t occ){
        uint64_t blockers = occ & rookBlockerMasks[square];
        uint64_t index = (blockers * rookMagics[square]) >> rookShiftAmount[square];
        return rookAttackTable[square][index];
    }
};