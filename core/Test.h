#pragma once
#include "State.h"
#include "MoveGen.h"
#include "MoveExec.h"
#include "ThreatGen.h"
#include "MoveCounter.h"

namespace Test {
        void countMoveType(const Move& move, MoveCounter& counter) {
            switch (move.movingType) {
                case Const::MT_CAPTURE:
                counter.capture++;   break;
                case Const::MT_ENPASSANT:  
                counter.enpassant++;
                counter.capture++;   break;
                case Const::MT_CASTLE:
                counter.castle++;    break;
                case Const::MT_PROMOTION:
                counter.promotion++; break;
            }
        }

    int perft(int depth, State& state, MoveCounter& counters){
        if(depth == 0) return 1;
        int nodes = 0;
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;
        for (auto & move : pseudo)
        {
            MoveExec::makeMove(state, move, undo);
            ThreatGen::updateThreats(state);
            if(MoveExec::isOpponentKingInCheck(state)){
                MoveExec::undoMove(state, undo);
                continue;
            }

            if (depth == 1) {
                countMoveType(move, counters);
            }

            nodes += perft(depth - 1, state, counters);
            MoveExec::undoMove(state, undo);
        }
        return nodes;
    }

    void dividePerft(int depth, State& state){
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;
        MoveCounter localCounter;
        int totalNodes = 0;
        for (auto & move : pseudo)
        {
            MoveExec::makeMove(state, move, undo);
            ThreatGen::updateThreats(state);
            if(MoveExec::isOpponentKingInCheck(state)){
                MoveExec::undoMove(state, undo);
                continue;
            }

            if (depth == 1) {
                countMoveType(move, localCounter);
            }

            int nodes = perft(depth - 1, state, localCounter);
            totalNodes += nodes;
            MoveExec::undoMove(state, undo);
            Utils::print(nodes);
        }
        Utils::print(totalNodes);
        localCounter.print();
    }
}