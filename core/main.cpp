#include "Chess.h"

int main(int argc, char *argv[]){
    Chess chess;
    chess.initialize();
    chess.Perft(5);
    return 0;
}