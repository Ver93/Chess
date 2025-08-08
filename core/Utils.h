#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <fstream>

#include "State.h"

namespace Utils {
    inline void print(const std::string& message){
        std::cout << message << std::endl;
    }

    inline void print(int value){
        std::cout << value << std::endl;
    }

    inline void print(std::string& message, int value){
        std::cout << message << value << std::endl;
    }

    inline void print(uint64_t mask) {
        std::bitset<64> bm(mask);
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                int square = rank * 8 + file;
                std::cout << (bm[square] ? "1 " : ". ");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    
    inline int popLSB(uint64_t& bb){
        int square = __builtin_ctzll(bb);
        bb &= bb - 1;
        return square;
    }

    inline void refreshOccupancy(State& state){
        state.turnOccupancy[Const::PC_WHITE] =
        state.bitboards[Const::W_PAWN] |
        state.bitboards[Const::W_KNIGHT] |
        state.bitboards[Const::W_BISHOP] |
        state.bitboards[Const::W_ROOK] |
        state.bitboards[Const::W_QUEEN] |
        state.bitboards[Const::W_KING]; 

        state.turnOccupancy[Const::PC_BLACK] =
        state.bitboards[Const::B_PAWN] |
        state.bitboards[Const::B_KNIGHT] |
        state.bitboards[Const::B_BISHOP] |
        state.bitboards[Const::B_ROOK] |
        state.bitboards[Const::B_QUEEN] |
        state.bitboards[Const::B_KING]; 

        state.occupancy = state.turnOccupancy[Const::PC_WHITE] | state.turnOccupancy[Const::PC_BLACK];
    }

    inline void refreshSquareToPieceIndex(State& state) {
        std::fill(std::begin(state.squareToPieceIndex), std::end(state.squareToPieceIndex), Const::NO_VALUE);

        for (int pieceIndex = 0; pieceIndex < 12; ++pieceIndex) {
            uint64_t bb = state.bitboards[pieceIndex];
            while (bb) {
                int square = popLSB(bb);
                state.squareToPieceIndex[square] = pieceIndex;
            }
        }
    }

    inline void loadFen(State& state, const std::string& fen){
        std::cout << "Initializing FEN string.." << std::endl;
        std::stringstream ss(fen);
        std::string position;

        std::getline(ss, position, ' ');
        std::stringstream posStream(position);
        std::string row;

        int rank = 7;

        while(std::getline(posStream, row, '/')){
            int file = 0;
            for(char c : row){
                if(std::isdigit(c)){
                    file += c - '0';
                } else {
                    if(file >= 8 || rank < 0) return;
                    int square = rank * 8 + file;
                    int pieceIndex = Const::NO_VALUE;
                    
                    switch (c) {
                        case 'P': pieceIndex = Const::W_PAWN; break;
                        case 'N': pieceIndex = Const::W_KNIGHT; break;
                        case 'B': pieceIndex = Const::W_BISHOP; break;
                        case 'R': pieceIndex = Const::W_ROOK; break;
                        case 'Q': pieceIndex = Const::W_QUEEN; break;
                        case 'K': pieceIndex = Const::W_KING; break;
                        case 'p': pieceIndex = Const::B_PAWN; break;
                        case 'n': pieceIndex = Const::B_KNIGHT; break;
                        case 'b': pieceIndex = Const::B_BISHOP; break;
                        case 'r': pieceIndex = Const::B_ROOK; break;
                        case 'q': pieceIndex = Const::B_QUEEN; break;
                        case 'k': pieceIndex = Const::B_KING; break;
                    }

                    if(pieceIndex != Const::NO_VALUE){
                        state.bitboards[pieceIndex] |= 1ULL << square;
                    }

                    file ++;
                }
            }
            rank --;
        }
        state.kingBitMap[Const::PC_WHITE] = state.bitboards[Const::W_KING];
        state.kingBitMap[Const::PC_BLACK] = state.bitboards[Const::B_KING];
        refreshOccupancy(state);
        refreshSquareToPieceIndex(state);
        std::cout << "FEN string initialized!" << std::endl;
    }
}