#pragma once
#include "State.h"
#include "MoveGen.h"
#include "MoveExec.h"

namespace Test {
    int perft(int depth, State& state){
        if(depth == 0) return 1;
        int nodes = 0;

        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;
        for (auto & move : pseudo)
        {
            MoveExec::makeMove(state, move, undo);
            nodes += perft(depth - 1, state);
            MoveExec::undoMove(state, undo);
        }
        return nodes;
    }

    void dividePerft(int depth, State& state){
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;
        int totalNodes = 0;
        for (auto & move : pseudo)
        {
            MoveExec::makeMove(state, move, undo);
            int nodes = perft(depth - 1, state);
            totalNodes += nodes;
            MoveExec::undoMove(state, undo);
            Utils::print(nodes);
        }
        Utils::print(totalNodes);
    }
}