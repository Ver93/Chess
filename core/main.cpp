#include "Chess.h"

int main(int argc, char *argv[]){
    Chess chess;
    chess.initialize();
    chess.perft(7);
    chess.visual();
    return 0;
}