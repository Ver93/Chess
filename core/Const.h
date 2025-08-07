#pragma once
#include <cstdint>

namespace Const {
    constexpr int MAX_SQUARES                   = 1ULL << 6;    // 64
    constexpr int MAX_BISHOP_ENTRIES            = 1ULL << 13;   // 8192
    constexpr int MAX_ROOK_ENTRIES              = 1ULL << 14;   // 16384
    constexpr int TRIVIAL_MAGIC                 = 0xFF;         // 255 
    constexpr int NO_VALUE                      = -1;

    //PieceType
    constexpr int PT_PAWN                       = 0;
    constexpr int PT_KNIGHT                     = 1;
    constexpr int PT_BISHOP                     = 2;
    constexpr int PT_ROOK                       = 3;
    constexpr int PT_QUEEN                      = 4;
    constexpr int PT_KING                       = 5;

    constexpr int SLIDING_MOVES[8][2]           { {-1, +0}, {+1, +0}, {+0, -1}, {+0, +1}, {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };

    constexpr uint64_t EXCLUDE_A_FILE           = 0xfefefefefefefefeULL;
    constexpr uint64_t EXCLUDE_B_FILE           = 0xfdfdfdfdfdfdfdfdULL;
    constexpr uint64_t EXCLUDE_G_FILE           = 0xbfbfbfbfbfbfbfbfULL;
    constexpr uint64_t EXCLUDE_H_FILE           = 0x7f7f7f7f7f7f7f7fULL;

    constexpr uint64_t EXCLUDE_AB_FILE          = EXCLUDE_A_FILE & EXCLUDE_B_FILE;
    constexpr uint64_t EXCLUDE_GH_FILE          = EXCLUDE_G_FILE & EXCLUDE_H_FILE;

    constexpr uint64_t RANK_1                   = 0x00000000000000FFULL;
    constexpr uint64_t RANK_2                   = 0x000000000000FF00ULL;
    constexpr uint64_t RANK_7                   = 0x00FF000000000000ULL;
    constexpr uint64_t RANK_8                   = 0xFF00000000000000ULL;

    constexpr uint64_t NO_MASK                  = 0x00ULL;
}