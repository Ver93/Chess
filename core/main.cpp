#include <iostream>
#include "Magics.h"
#include "State.h"
#include "Const.h"
#include "MoveGen.h"

int main(){
    Magics::initialize();
    State state;
    Utils::loadFen(state, Const::STARTING_FEN);
    std::vector<Move> pseudo = MoveGen::generatePseudoMoves(state);
    Utils::print((int)pseudo.size());
    return 0;
}