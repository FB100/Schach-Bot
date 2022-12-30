#ifndef SCHACH_INIT_H
#define SCHACH_INIT_H


#include "headers.h"


void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);

void fenToBoard(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]);


#endif //SCHACH_INIT_H