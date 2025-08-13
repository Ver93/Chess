#include "engine.h"

namespace Engine {
    std::vector<Move> generateLegalMoves(State& state){
        std::vector<Move> legalMoves;
        std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
        Undo undo;

        for (const auto& move : pseudo) {
            if (move.movingType == Const::MT_CASTLE) {
                if (MoveVal::isKingInCheckBeforeMove(state)) continue;
                if (!MoveVal::isCastlingPathSafe(state, move.to, state.threatMap[state.turn ^ 1])) continue;
            }

            MoveExec::makeMove(state, move, undo);
            MoveExec::switchTurn(state);
            ThreatGen::updateThreats(state);

            if (!MoveVal::isKingInCheckAfterMove(state)) {
                legalMoves.emplace_back(move);
            }
            MoveExec::undoMove(state, undo);
        }
        return legalMoves;
    }
}