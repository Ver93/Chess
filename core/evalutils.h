#pragma once
#include <cstdint>

#include "const.h"

namespace EvalUtils {
    uint64_t generateRelevantAdjacentFileMask(int square);
    uint64_t generateRelevantSingleFileMask(int square);
    uint64_t generatePassedPawnMask(int square, bool isWhite);
}   