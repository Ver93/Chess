#include "engine.h"

void Engine::initialize() {
    Magics::initialize();
    std::string startFEN = Const::STARTING_FEN;
    Utils::loadFen(state, startFEN);
    originalState = state;
}

void Engine::run() {
    std::string input;

    while (std::getline(std::cin, input)) {
        if (input == "uci") {
            std::cout << "id name Rookie\n";
            std::cout << "Rookie By Name, Grandmaster By Nature.\n";
            std::cout << "id author Ver93\n";
            std::cout << "uciok\n";
        } else if (input == "isready") {
            std::cout << "readyok\n";
        } else if (input.rfind("position", 0) == 0) {
        } else if (input.rfind("perft", 0) == 0) {
            std::istringstream ss(input);
            std::string command;
            int depth;

            ss >> command >> depth;

            if (ss.fail() || depth <= 0) {
                Utils::print("Invalid Depth For Perft ", depth);
                continue;
            }

            Utils::print("Perft For Depth: ", depth, " Started.");
            Timer::start();
            Test::dividePerft(depth, state);
            Timer::stop();
        } else if (input.rfind("go", 0) == 0) {
            const Move bestMove = Search::findBestMove(state, 4);
            move(bestMove);
        } else if (input == "quit") {
            break;
        }
    }
}

void Engine::move(const Move& move){
    Undo undo;
    MoveExec::makeMove(state, move, undo);
    MoveExec::switchTurn(state);
    ThreatGen::updateThreats(state);
    Utils::print("Move Executed:");
    Utils::print(move);
}