#pragma once

#include <vector>

#include "move.h"
#include "undo.h"
#include "state.h"
#include "movegen.h"
#include "moveval.h"
#include "moveexec.h"
#include "threatgen.h"
#include "evaluate.h"
#include "engine.h"
#include "moveorder.h"

namespace Search {
    inline int alphaBeta(State& state, int depth, int alpha, int beta){
        if(depth == 0) return Evalaute::evaluate(state);
        std::vector<Move> moves = Engine::generateLegalMoves(state);

        if (moves.empty()) {
            if (MoveVal::isKingInCheckAfterMove(state)) return -Const::INF + depth;
            else return 0;
        }

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);

        Undo undo;
        for (auto & move : moves)
        {
            MoveExec::makeMove(state, move, undo);
            ThreatGen::updateThreats(state);
            int score = -alphaBeta(state, depth - 1, -beta, -alpha);   
            MoveExec::undoMove(state, undo);

            if(score >= beta){
                return beta;
            }

            if(score > alpha){
                alpha = score;
            }
        }

        return alpha;
    }

    inline Move findBestMove(State& state, int depth){
        Move bestMove;
        int bestScore = -Const::INF;
        std::vector<Move> moves = Engine::generateLegalMoves(state);

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);

        Undo undo;
        for (auto& move : moves)
        {
            MoveExec::makeMove(state, move, undo);
            int score = -alphaBeta(state, depth - 1, -Const::INF, Const::INF);
            MoveExec::undoMove(state, undo);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                bestMove.score = score;
            }
        }
        return bestMove;
    }
}