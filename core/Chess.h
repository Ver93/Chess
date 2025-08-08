#pragma once
#include <iostream>
#include "Magics.h"
#include "State.h"
#include "Const.h"
#include "MoveGen.h"
#include "Test.h"

class Chess {
public:
    inline void initialize(){
        Magics::initialize();
        Utils::loadFen(state, Const::STARTING_FEN);
    }

    inline void Perft(int depth){
        Test::dividePerft(depth, state);
    }

private:
    State state;
    std::vector<Move> pseudoMoves;
};