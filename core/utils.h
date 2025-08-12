#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>
#include <sstream>
#include <fstream>

#include "state.h"

namespace Utils {

    template<typename... Args>
    inline void print(const Args&... args){
        (std::cout << ... << args) << std::endl;
    }

    // inline void print(const std::string& message){
    //     std::cout << message << std::endl;
    // }

    // inline void print(const std::string& first, const std::string& second){
    //     std::cout << first << " " << second << std::endl;
    // }

    // inline void print(const int value){
    //     std::cout << value << std::endl;
    // }

    // inline void print(const long long value){
    //     std::cout << value << std::endl;
    // }

    // inline void print(const std::string& message, const int value){
    //     std::cout << message << value << std::endl;
    // }

    // inline void print(const std::string& message, const long long value){
    //     std::cout << message << value << std::endl;
    // }

    // inline void print(const std::string& message, const long long value){
    //     std::cout << message << value << std::endl;
    // }

    inline void print(const uint64_t mask) {
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

    inline std::string squareName(const int sq) {
        const char files[] = "abcdefgh";
        int file = sq % 8;
        int rank = sq / 8;
        return std::string(1, files[file]) + std::to_string(rank + 1);
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
        std::string position, activeColor, castling, enPassant;

        std::getline(ss, position, ' ');
        std::getline(ss, activeColor, ' ');
        std::getline(ss, castling, ' ');
        std::getline(ss, enPassant, ' ');

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

                    file++;
                }
            }
            rank--;
        }

        state.turn = (activeColor == "w") ? Const::PC_WHITE : Const::PC_BLACK;

        state.kingMoved[Const::PC_WHITE] = !(castling.find('K') != std::string::npos || castling.find('Q') != std::string::npos);
        state.kingMoved[Const::PC_BLACK] = !(castling.find('k') != std::string::npos || castling.find('q') != std::string::npos);

        state.rooksMoved[Const::PC_WHITE][0] = (castling.find('Q') == std::string::npos);
        state.rooksMoved[Const::PC_WHITE][1] = (castling.find('K') == std::string::npos);
        state.rooksMoved[Const::PC_BLACK][0] = (castling.find('q') == std::string::npos);
        state.rooksMoved[Const::PC_BLACK][1] = (castling.find('k') == std::string::npos);

        if(enPassant != "-"){
            int file = enPassant[0] - 'a';
            int rank = enPassant[1] - '1';
            state.enPassantSquare = rank * 8 + file;
            state.enPassantTarget = state.enPassantSquare;
        } else {
            state.enPassantSquare = Const::NO_VALUE;
            state.enPassantTarget = Const::NO_VALUE;
        }

        state.kingBitMap[Const::PC_WHITE] = state.bitboards[Const::W_KING];
        state.kingBitMap[Const::PC_BLACK] = state.bitboards[Const::B_KING];
        refreshOccupancy(state);
        refreshSquareToPieceIndex(state);
        std::cout << "FEN string initialized!" << std::endl;
    }
}