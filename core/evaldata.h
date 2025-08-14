#pragma once
#include <cstdint>

struct EvalData{
    uint64_t singleFileMask[64];
    uint64_t adjacentPawnMask[64];
    uint64_t whitePawnPassedMask[64];
    uint64_t blackPawnPassedMask[64];
};
