#ifndef SCHACH_MAIN_H
#define SCHACH_MAIN_H

#include "headers.h"


int handleArgumentsAndInitialiseGame(int argc, char **argv);

void printHelp();

void runGame(Piece board[BOARD_SIZE][BOARD_SIZE], Board *bitBoardBoard, bool whiteTurn, bool aiOnly);

#endif //SCHACH_MAIN_H