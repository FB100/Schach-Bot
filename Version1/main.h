#ifndef SCHACH_MAIN_H
#define SCHACH_MAIN_H


#include "headers.h"


int handleArguments(int argc, char **argv);

void printHelp();

void runGame(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool aiOnly);

#endif //SCHACH_MAIN_H