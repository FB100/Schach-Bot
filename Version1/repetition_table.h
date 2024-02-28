#ifndef VERSION1_REPETITION_TABLE_H
#define VERSION1_REPETITION_TABLE_H

#include "headers.h"


u_int8_t amountInRepetitionTable(ulong h);

void pushRepetitionTable(ulong h);

bool handleRepetitions(Piece board[BOARD_SIZE][BOARD_SIZE]);


#endif //VERSION1_REPETITION_TABLE_H
