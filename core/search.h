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
    int alphaBeta(State& state, int depth, int alpha, int beta){
        if(depth == 0) return Evalaute::evaluate(state);
        std::vector<Move> moves = MoveGen::generatePseudoMoves(state);

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);

        Undo undo;
        for (auto & move : moves)
        {
            if (move.movingType == Const::MT_CASTLE) {
                if (MoveVal::isKingInCheckBeforeMove(state)) continue;
                if (!MoveVal::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

            if (MoveVal::isKingInCheckAfterMove(state)) {
                MoveExec::undoMove(state, undo);
                continue;
            }

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

    Move findBestMove(State& state, int depth){
        Move bestMove;
        int bestScore = -Const::INF;
        std::vector<Move> moves = MoveGen::generatePseudoMoves(state);

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);

        Undo undo;
        for (auto& move : moves)
        {
            if (move.movingType == Const::MT_CASTLE) {
                if (MoveVal::isKingInCheckBeforeMove(state)) continue;
                if (!MoveVal::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

            if (MoveVal::isKingInCheckAfterMove(state)) {
                MoveExec::undoMove(state, undo);
                continue;
            }

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