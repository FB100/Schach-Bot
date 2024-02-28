#ifndef SCHACH_AI_EVALUATION_H
#define SCHACH_AI_EVALUATION_H

#include "headers.h"


int evaluateBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int round);

int findMovesAndEvaluate(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool initialCall, int remainingDepth, int alpha, int beta,
                         int castlingRights, int round);


#endif //SCHACH_AI_EVALUATION_H