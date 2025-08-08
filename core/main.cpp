#include "Chess.h"

int main(int argc, char *argv[]){
    Chess chess;
    chess.initialize();
    chess.Perft(3);
    return 0;
}