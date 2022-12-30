#ifndef SCHACH_AI_H
#define SCHACH_AI_H


#include "headers.h"


int evaluateBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int round);

void getAllMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int castlingRights);

int findMovesAndEvaluate(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool initialCall, int remainingDepth, int alpha, int beta,
                         int castlingRights, int round);

#endif //SCHACH_AI_H