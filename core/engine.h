#pragma once

#include <cstdint>

#include "state.h"
#include "movegen.h"
#include "moveexec.h"
#include "moveval.h"
#include "move.h"
#include "undo.h"
#include "threatgen.h"

namespace Engine {
    std::vector<Move> generateLegalMoves(State& state);
}