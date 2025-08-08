#pragma once
#include <iostream>

struct MoveCounter {

    MoveCounter& operator+=(const MoveCounter& other) {
        capture    += other.capture;
        enpassant  += other.enpassant;
        castle     += other.castle;
        promotion  += other.promotion;
        checks     += other.checks;
        checkMates += other.checkMates;
        staleMates += other.staleMates;
        return *this;
    }

    int capture = 0;
    int enpassant = 0;
    int castle = 0;
    int promotion = 0;
    int checks = 0;
    int checkMates = 0;
    int staleMates = 0;

    void print() const {
        std::cout << "Captures: " << capture << std::endl;
        std::cout << "EnPassant: " << enpassant << std::endl;
        std::cout << "Castle: " << castle << std::endl;
        std::cout << "Promotion: " << promotion << std::endl;
        std::cout << "Checks: " << checks << std::endl;
        std::cout << "CheckMates: " << checkMates << std::endl;
        std::cout << "StaleMates: " << staleMates << std::endl;
    }
};
