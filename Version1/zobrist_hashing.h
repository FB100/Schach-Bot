#ifndef VERSION1_ZOBRIST_HASHING_H
#define VERSION1_ZOBRIST_HASHING_H

#include "headers.h"

void initZobristTable();

unsigned long long int getZobristTable(int i, int j, int type);

unsigned long long int computeHash(Piece board[BOARD_SIZE][BOARD_SIZE]);


#endif //VERSION1_ZOBRIST_HASHING_H
