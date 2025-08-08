#pragma once
#include <iostream>
#include "Magics.h"
#include "State.h"
#include "Const.h"
#include "MoveGen.h"
#include "Test.h"
#include "BoardGUI.h"

class Chess {
public:
    inline void initialize(){
        Magics::initialize();
        Utils::loadFen(state, Const::STARTING_FEN);
    }

    inline void perft(int depth){
        Test::dividePerft(depth, state);
    }

    inline void visual(){
        bg.run(state);
    }

private:
    BoardGUI bg;
    State state;
    std::vector<Move> pseudoMoves;
};