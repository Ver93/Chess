#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <fstream>

#include "bitintrinsics.h"
#include "state.h"
#include "move.h"

namespace Utils {

    template<typename... Args>
    void print(const Args&... args) {
        (std::cout << ... << args) << std::endl;
    }

    void print(const uint64_t mask);
    void printBoard(State& state);
    void refreshOccupancy(State& state);
    void refreshSquareToPieceIndex(State& state);
    void loadFen(State& state, const std::string& fen);

    int mirror(int sq);
    int popLSB(uint64_t& bb);
    int countBits(uint64_t bb);

    std::string squareToString(const int sq);
    int stringToSquare(const std::string& str);
    std::pair<int, int> parseMoveString(const std::string& moveStr);
    void print(const Move& move);
    int charToPieceIndex(char c, bool isWhite);
}