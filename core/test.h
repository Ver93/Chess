#pragma once

#include <future>
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <mutex>

#include "state.h"
#include "movecounter.h"
#include "threatgen.h"
#include "movegen.h"
#include "moveexec.h"
#include "MoveVal.h"
#include "move.h"
#include "undo.h"
#include "const.h"
#include "utils.h"

using PerftOutput = std::pair<long long, MoveCounter>;

namespace Test {
    void countMoveType(const Move& move, MoveCounter& counter);
    void countCheckAndCheckmate(State& state, MoveCounter& counter);
    void dividePerft(int depth, State& state);
    int perft(int depth, State& state, MoveCounter& counters);
    PerftOutput perftTask(int depth, State& originalState, const Move& move);
}