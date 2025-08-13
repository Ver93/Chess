#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <string>

#include "const.h"
#include "bitintrinsics.h"
#include "attackdata.h"

namespace MagicUtils {
    uint64_t generateSlidingMoves(int square, uint64_t blockerConfig, int pieceType);
    uint64_t generateJumpingMoves(int square, int pieceType, bool isWhite);
    bool testMagic(std::vector<uint64_t>& permutations, std::vector<uint64_t>& used, uint64_t magic, int relevantBits, int pt, int square, int arraySize);
    std::vector<uint64_t> calcTotalPermutations(uint64_t mask);
    uint64_t rand64();
    uint64_t randomMagic();
    bool filterHeuristics(uint64_t magic, uint64_t mask, int shift);
    bool fileExists(const std::string& path);
    uint64_t findMagic(int square, int relevantBits, int pt, uint64_t& mask, std::vector<uint64_t> permutations);
    void saveMagicData(std::unique_ptr<AttackData>& dataPtr, const std::string& saveString);
    std::unique_ptr<AttackData> loadMagicData(const std::string& loadString);
}