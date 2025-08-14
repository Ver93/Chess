#include "engine.h"
#include "attackglobals.h"

void Engine::initialize() {
    Magics::initialize();
    Evaluate::initialize();
    Utils::loadFen(state, Const::STARTING_FEN);
    originalState = state;
}

void Engine::run() {
    std::string input;

    while (std::getline(std::cin, input)) {
        std::istringstream ss(input);
        std::string token;
        while (ss >> token) {
            if (token == "uci") {
                std::cout << "id name Botomir\n";
                std::cout << "id author Ver93\n";
                std::cout << "uciok\n";
            } else if (token == "isready") {
                std::cout << "readyok\n";
            } else if (token == "position") {
                std::string sub;
                ss >> sub;
                state = originalState;

                while (ss >> token) {
                    if (token == "moves") {
                        while (ss >> token) {
                            
                            std::pair<int, int> pos = Utils::parseMoveString(token);
                            pseudoMoves = MoveGen::generatePseudoMoves(state);

                            for (auto& lichMove : pseudoMoves) {
                                if (lichMove.from == pos.first && lichMove.to == pos.second) {
                                    bool isWhite = state.turn == Const::PC_WHITE;
                                    if ((lichMove.movingType == Const::MT_PROMOTION || lichMove.movingType == Const::MT_PROMOTION_CAPTURE) &&
                                    (token.length() == 5)) {
                                        char pp = token[4];
                                        int promotionPiece = Utils::charToPieceIndex(pp, isWhite);
                                        lichMove.promotionPiece = promotionPiece;
                                    }
                                    move(lichMove);
                                    break;
                                } 
                            }
                        }
                    }
                }
            } else if (token == "go") {
                const Move bestMove = Search::findBestMove(state, 4);
                if (bestMove.from == -1 || bestMove.to == -1) {
                    std::cout << "bestmove 0000\n";
                    std::cout << "info string Checkmate or stalemate detected.\n";
                    return;
                }

                move(bestMove);
                Utils::printUCIMove(bestMove);
            } else if (token == "reset") {
                state = originalState;
            }else if (token == "quit") {
                return;
            }
        }
    }
}

void Engine::move(const Move& move){
    Undo undo;
    MoveExec::makeMove(state, move, undo);
    MoveExec::switchTurn(state);
    ThreatGen::updateThreats(state);
}