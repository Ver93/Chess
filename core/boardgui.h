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
    BoardGUI();
    void run(State& state);
    State originalState;
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<Square> squares;
    std::vector<Move> legalMoves;
    int selectedSquare = -1;

    void loadFont();
    void createBoardGUI();
    void drawRects();
    void highlightSquare(int index, const sf::Color& color);
    sf::Vector2i getMouseGridPosition();
    int getGUIIndex(sf::Vector2i v);
    int selectSquare();
    void resetColors();
    void drawUnicode(State& state);
    wchar_t getPieceUnicode(int index);
    void updateSelection(State& state);
};