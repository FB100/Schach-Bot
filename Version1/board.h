#ifndef VERSION1_BOARD_H
#define VERSION1_BOARD_H

#include "headers.h"

void makeMove(Move move, Piece board[BOARD_SIZE][BOARD_SIZE], Board *bitBoardBoard);

void unmakeMove(Move move, Piece board[BOARD_SIZE][BOARD_SIZE], Board *bitBoardBoard);

#endif //VERSION1_BOARD_H
