#include "legalmove.h"


namespace LegalMove {
    std::vector<Move> generate(State& state){
        std::vector<Move> moves = MoveGen::generatePseudoMoves(state);
        std::vector<Move> legal;
        Undo undo;
        for (auto& move : moves)
        {
            if (move.movingType == Const::MT_CASTLE) {
                if (MoveVal::isKingInCheckBeforeMove(state)) continue;
                if (!MoveVal::isCastlingPathSafe(move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

            if (!MoveVal::isKingInCheckAfterMove(state)) {
                legal.emplace_back(move);
            }


            MoveExec::undoMove(state, undo);
        }

        return legal;
    }
}