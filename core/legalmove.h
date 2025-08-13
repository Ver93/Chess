#pragma once
#include <vector>
#include "movegen.h"
#include "state.h"
#include "move.h"
#include "utils.h"
#include "undo.h"
#include "const.h"
#include "threatgen.h"
#include "moveval.h"
#include "moveexec.h"


namespace LegalMove {
    std::vector<Move> generate(State& state);
}