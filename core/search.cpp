#include "search.h"

namespace Search {
    int alphaBeta(State& state, int depth, int alpha, int beta){
        if(depth == 0) return Evaluate::evaluate(state);
        std::vector<Move> moves = LegalMove::generate(state);

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);

        if(moves.empty()){
            if(MoveVal::isKingInCheckBeforeMove(state)){
                return -Const::INF + (depth);
            } else {
                return 0;
            }
        }

        Undo undo;
        for (auto & move : moves)
        {
            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
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

    Move findBestMove(State& state, int depth){
        Move bestMove;
        int bestScore = -Const::INF;
        std::vector<Move> moves = LegalMove::generate(state);
        if(moves.size() == 1) return moves[0];

        MoveOrder::scoreMoves(state, moves);
        MoveOrder::sortMoves(moves);
        Undo undo;
        for (auto& move : moves)
        {
            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

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