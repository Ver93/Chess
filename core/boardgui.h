#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"
#include "state.h"
#include "move.h"
#include "undo.h"
#include "movegen.h"
#include "moveexec.h"
#include "moveval.h"
#include "threatgen.h"
#include "engine.h"
#include "timer.h"

#include "Search.h"

struct Square {
    sf::RectangleShape rect;
    sf::Color original;
};

class BoardGUI {
public:
    BoardGUI() : window(sf::VideoMode(768, 768), "Chess GUI") {}

    void run(State& state) {
        loadFont();
        createBoardGUI();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::MouseButtonPressed:
                        updateSelection(state);
                        break;
                            case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::R) {
                            state = originalState;
                            Utils::print("Board reset via keyboard");
                        }
                        break;

                    default:
                        break;
                }
            }

            window.clear();
            drawRects();
            drawUnicode(state);
            window.display();
        }

        Utils::print("End Reached");
    }

    State originalState;
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<Square> squares;
    std::vector<Move> legalMoves;
    int selectedSquare = -1;

    void loadFont() {
        if (!font.loadFromFile("../font/DejaVuSans.ttf")) {
            Utils::print("Failed to load font");
        } else {
            Utils::print("Font loaded");
        }
    }

    void createBoardGUI() {
        squares.clear();
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Square square;
                square.rect.setSize(sf::Vector2f(96.0f, 96.0f));
                square.rect.setOutlineColor(sf::Color::Black);
                square.rect.setOutlineThickness(2);
                square.rect.setFillColor((x + y) % 2 == 0 ? sf::Color(181, 136, 99) : sf::Color(240, 217, 181));
                square.original = square.rect.getFillColor();
                square.rect.setPosition(x * 96.0f, (7 - y) * 96.0f);
                squares.emplace_back(square);
            }
        }
    }

    void drawRects() {
        for (const auto& square : squares) {
            window.draw(square.rect);
        }
    }

    void highlightSquare(int index, const sf::Color& color) {
        if (index >= 0 && index < 64) {
            squares[index].rect.setFillColor(color);
        }
    }

    sf::Vector2i getMouseGridPosition() {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        int gridX = pixelPos.x / 96;
        int gridY = (window.getSize().y - pixelPos.y) / 96;
        return sf::Vector2i(gridX, gridY);
    }

    int getGUIIndex(sf::Vector2i v) {
        return (v.y * 8 + v.x);
    }

    int selectSquare() {
        sf::Vector2i mousePos = getMouseGridPosition();
        return getGUIIndex(mousePos);
    }

    void resetColors() {
        for (auto& square : squares) {
            square.rect.setFillColor(square.original);
        }
    }

    void drawUnicode(State& state) {
        const float squareSize = 96.0f;

        for (int i = 0; i < 12; i++) {
            uint64_t bb = state.bitboards[i];
            for (int square = 0; square < 64; square++) {
                if (bb & (1ULL << square)) {
                    sf::Text pieceText;
                    pieceText.setFont(font);
                    pieceText.setCharacterSize(squareSize);
                    pieceText.setString(sf::String(getPieceUnicode(i)));
                    pieceText.setFillColor((i < 6) ? sf::Color::White : sf::Color::Black);

                    int file = square % 8;
                    int rank = 7 - (square / 8);
                    float x = file * squareSize;
                    float y = rank * squareSize;

                    sf::FloatRect bounds = pieceText.getLocalBounds();
                    pieceText.setOrigin(bounds.left + bounds.width / 2.0f,
                                        bounds.top + bounds.height / 2.0f);
                    pieceText.setPosition(x + squareSize / 2.0f,
                                          y + squareSize / 2.0f);

                    window.draw(pieceText);
                }
            }
        }
    }

    wchar_t getPieceUnicode(int index) {
        switch(index) {
            case 0: return L'\u265F'; // ♙ white pawn
            case 1: return L'\u265E'; // ♘ white knight
            case 2: return L'\u265D'; // ♗ white bishop
            case 3: return L'\u265C'; // ♖ white rook
            case 4: return L'\u265B'; // ♕ white queen
            case 5: return L'\u265A'; // ♔ white king
            case 6: return L'\u265F'; // ♟ black pawn
            case 7: return L'\u265E'; // ♞ black knight
            case 8: return L'\u265D'; // ♝ black bishop
            case 9: return L'\u265C'; // ♜ black rook
            case 10: return L'\u265B'; // ♛ black queen
            case 11: return L'\u265A'; // ♚ black king
            default: return L' ';
        }
    }

    void updateSelection(State& state) {
        int clickedIndex = selectSquare();

        if (selectedSquare != -1 && clickedIndex != selectedSquare) {
            for (const Move& move : legalMoves) {
                if (move.from == selectedSquare && move.to == clickedIndex) {
                    Undo undo;
                    MoveExec::makeMove(state, move, undo);
                    MoveExec::switchTurn(state);
                    ThreatGen::updateThreats(state);

                    if(state.turn == Const::PC_BLACK){
                        Timer::start();
                        Move bestMove = Search::findBestMove(state, 4);
                        Timer::stop();
                        MoveExec::makeMove(state, bestMove, undo);
                        MoveExec::switchTurn(state);
                        ThreatGen::updateThreats(state);
                        Utils::print(bestMove.score);
                    }

                    selectedSquare = -1;
                    legalMoves.clear();
                    resetColors();

                    uint64_t king = state.kingBitMap[state.turn];
                    int kingSq = Utils::popLSB(king);
                    if (MoveVal::isKingInCheckAfterMove(state)) {
                        highlightSquare(kingSq, sf::Color::Red);
                    }
                    return;
                }
            }
        }

        selectedSquare = clickedIndex;
        legalMoves.clear();
        resetColors();

        int pieceIndex = -1;
        for (int i = 0; i < 12; ++i) {
            if (state.bitboards[i] & (1ULL << clickedIndex)) {
                pieceIndex = i;
                break;
            }
        }

        if (pieceIndex == -1) return;
        int pieceColor = (pieceIndex < 6) ? 0 : 1;
        if (pieceColor != state.turn) return;

        std::vector<Move> moves = Engine::generateLegalMoves(state);
        for (const Move& move : moves) {
            if (move.from == clickedIndex) {
                legalMoves.push_back(move);
                highlightSquare(move.to, sf::Color::Green);
            }
        }

        highlightSquare(clickedIndex, sf::Color::Blue);
    }
};