#pragma once

#include <cstdint>
#include <sstream>

#include "magics.h"
#include "state.h"
#include "movegen.h"
#include "moveexec.h"
#include "moveval.h"
#include "move.h"
#include "undo.h"
#include "threatgen.h"
#include "test.h"
#include "search.h"
#include "timer.h"

class Engine {
public:
    Engine() = default;
    void initialize();
    void run();

private:
    State state;
    State originalState;
    void move(const Move& move);
};