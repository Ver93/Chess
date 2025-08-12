#include <iostream>
#include <string>
#include <vector>

#include "magics.h"
#include "state.h"
#include "utils.h"
#include "test.h"
#include "boardgui.h"
#include "timer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        Utils::print("Usage:");
        Utils::print("  ./Engine perft [set] depth [debug]");
        Utils::print("  ./Engine debug");
        return 1;
    }

    Magics::initialize();
    std::vector<std::string> args(argv + 1, argv + argc);
    std::string mode = args[0];
    bool debug = false;
    State state;

    if (mode == "perft") {
        std::string set = "start";
        int depth = -1;

        if (args.size() == 2) {
            depth = std::stoi(args[1]);
        } else if (args.size() >= 3) {
            set = args[1];
            depth = std::stoi(args[2]);
        }

        if (args.size() == 4 && args[3] == "debug") {
            debug = true;
        }

        if (set == "start") {
            Utils::loadFen(state, Const::STARTING_FEN);
        } else if (Const::PRESET_FENS.count(set)) {
            Utils::loadFen(state, Const::PRESET_FENS.at(set));
        } else {
            Utils::loadFen(state, set);
        }

        Timer::start();
        Test::dividePerft(depth, state);
        Timer::stop();

        if (debug) {
            Utils::print("Launching Visual Debugger...");
            BoardGUI vb;
            vb.run(state);
        }

    } else if (mode == "debug") {
        Utils::loadFen(state, Const::STARTING_FEN);
        BoardGUI vb;
        vb.originalState = state;
        vb.run(state);

    } else {
        Utils::print("Unknown mode: ", mode);
        return 1;
    }

    return 0;
}