#pragma once

#include "state.h"
#include "move.h"
#include "undo.h"
#include "const.h"
#include "utils.h"

namespace MoveExec {
    void setPiece(State& state, int square, int movingPiece);
    void clearPiece(State& state, int square, int movingPiece);
    void switchTurn(State& state);
    void makeMove(State& state, const Move& move, Undo& undo);
    void undoMove(State& state, const Undo& undo);
}