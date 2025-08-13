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
    int alphaBeta(State& state, int depth, int alpha, int beta);
    Move findBestMove(State& state, int depth);
}