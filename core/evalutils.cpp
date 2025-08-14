#include "evalutils.h"

namespace EvalUtils {
    uint64_t generateRelevantAdjacentFileMask(int square) {
        int file = square % 8;

        uint64_t passedMask = Const::FILE_MASKS[file];
        if (file > 0) passedMask |= Const::FILE_MASKS[file - 1];
        if (file < 7) passedMask |= Const::FILE_MASKS[file + 1];

        return passedMask;
    }

    uint64_t generateRelevantSingleFileMask(int square) {
        int file = square % 8;
        uint64_t passedMask = Const::FILE_MASKS[file];
        return passedMask;
    }

    uint64_t generatePassedPawnMask(int square, bool isWhite) {
        int rank = square / 8;
        int file = square % 8;

        uint64_t passedMask = Const::FILE_MASKS[file];
        if (file > 0) passedMask |= Const::FILE_MASKS[file - 1];
        if (file < 7) passedMask |= Const::FILE_MASKS[file + 1];

        if (isWhite && rank < 7) {
            for (int r = 0; r <= rank; ++r) {
                passedMask &= ~Const::RANK_MASKS[r];
            }
        } else if (!isWhite && rank > 0) {
            for (int r = 7; r >= rank; --r) {
                passedMask &= ~Const::RANK_MASKS[r];
            }
        }

        return passedMask;
    }
}