#pragma once
#include <cstdint>
#include <string>

namespace Const {

    /* Values For Magics */
    constexpr int MAX_SQUARES                   = 1ULL << 6;
    constexpr int MAX_BISHOP_ENTRIES            = 1ULL << 13;
    constexpr int MAX_ROOK_ENTRIES              = 1ULL << 14;
    constexpr int TRIVIAL_MAGIC                 = 0xFF;
    constexpr int NO_VALUE                      = -1;

    /* Sliding Piece Direction For Magics */
    constexpr int SLIDING_MOVES[8][2]           { {-1, +0}, {+1, +0}, {+0, -1}, {+0, +1}, {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };

    /* Piece Types */
    constexpr int PT_PAWN                       = 0;
    constexpr int PT_KNIGHT                     = 1;
    constexpr int PT_BISHOP                     = 2;
    constexpr int PT_ROOK                       = 3;
    constexpr int PT_QUEEN                      = 4;
    constexpr int PT_KING                       = 5;

    /* Turn Color */
    constexpr int PC_WHITE                      = 0;
    constexpr int PC_BLACK                      = 1;

    /* Piece Index */
    constexpr int W_PAWN                        = PT_PAWN;
    constexpr int W_KNIGHT                      = PT_KNIGHT;
    constexpr int W_BISHOP                      = PT_BISHOP;
    constexpr int W_ROOK                        = PT_ROOK;
    constexpr int W_QUEEN                       = PT_QUEEN;
    constexpr int W_KING                        = PT_KING;
    constexpr int B_PAWN                        = PT_PAWN + 6;
    constexpr int B_KNIGHT                      = PT_KNIGHT + 6;
    constexpr int B_BISHOP                      = PT_BISHOP + 6;
    constexpr int B_ROOK                        = PT_ROOK + 6;
    constexpr int B_QUEEN                       = PT_QUEEN + 6;
    constexpr int B_KING                        = PT_KING + 6;

    /* Movement Types */
    constexpr int MT_QUIET                      = 0;
    constexpr int MT_CAPTURE                    = 1;
    constexpr int MT_DOUBLEPUSH                 = 2;
    constexpr int MT_ENPASSANT                  = 3;
    constexpr int MT_CASTLE                     = 4;
    constexpr int MT_PROMOTION                  = 5;
    constexpr int MT_PROMOTION_CAPTURE          = 6;

    /* Pawn Pushes */
    constexpr int SINGLE_PUSH                   = 8;
    constexpr int DOUBLE_PUSH                   = 16;

    /* Castle Values */
    constexpr int C_QUEEN_SIDE                  = 0;
    constexpr int C_KING_SIDE                   = 1;

    constexpr int SQ_A1                         = 0;
    constexpr int SQ_B1                         = 1;
    constexpr int SQ_C1                         = 2;
    constexpr int SQ_D1                         = 3;

    constexpr int SQ_E1                         = 4;
    constexpr int SQ_F1                         = 5;
    constexpr int SQ_G1                         = 6;
    constexpr int SQ_H1                         = 7;

    constexpr int SQ_A8                         = 56;
    constexpr int SQ_B8                         = 57;
    constexpr int SQ_C8                         = 58;
    constexpr int SQ_D8                         = 59;

    constexpr int SQ_E8                         = 60;
    constexpr int SQ_F8                         = 61;
    constexpr int SQ_G8                         = 62;
    constexpr int SQ_H8                         = 63;

    /* Bitmap file exclusions */
    constexpr uint64_t EXCLUDE_A_FILE           = 0xfefefefefefefefeULL;
    constexpr uint64_t EXCLUDE_B_FILE           = 0xfdfdfdfdfdfdfdfdULL;
    constexpr uint64_t EXCLUDE_G_FILE           = 0xbfbfbfbfbfbfbfbfULL;
    constexpr uint64_t EXCLUDE_H_FILE           = 0x7f7f7f7f7f7f7f7fULL;

    constexpr uint64_t EXCLUDE_AB_FILE          = EXCLUDE_A_FILE & EXCLUDE_B_FILE;
    constexpr uint64_t EXCLUDE_GH_FILE          = EXCLUDE_G_FILE & EXCLUDE_H_FILE;

    /* Bitmap rank inclusion */
    constexpr uint64_t RANK_1                   = 0x00000000000000FFULL;
    constexpr uint64_t RANK_2                   = 0x000000000000FF00ULL;
    constexpr uint64_t RANK_7                   = 0x00FF000000000000ULL;
    constexpr uint64_t RANK_8                   = 0xFF00000000000000ULL;

    constexpr uint64_t NO_MASK                  = 0x00ULL;

    /* FEN-string constants */
    const std::string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string CASTLE_FEN = "r2qk2r/8/8/8/8/8/8/R2QK2R w KQkq - 0 1";
    const std::string CASTLE_FEN_KIWI = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";

    /* Helpers */
    constexpr int getPieceType(int pieceIndex) { return (pieceIndex >= 6) ? pieceIndex - 6 : pieceIndex; }
    constexpr int getPieceIndex(int pieceType) { return (pieceType >= 6) ? pieceType + 6 : pieceType; }
}