#ifndef SCHACH_AI_EVALUATION_H
#define SCHACH_AI_EVALUATION_H

#include "headers.h"

int getPositionModifierPawn(int i, int j, bool white, int round);

int getPositionModifierKnight(int i, int j, bool white, int round);

int getPositionModifierRook(int i, int j, bool white, int round);

int getPositionModifierBishop(int i, int j, bool white, int round);

int getPositionModifierQueen(int i, int j, bool white, int round);

int getPositionModifierKing(int i, int j, bool white, int round);

int evaluateBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int round, bool whiteturn);

int findMovesAndEvaluate(Board *bitBoardBoard, Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool initialCall, int remainingDepth, int alpha, int beta,
                         int castlingRights, int round);


#endif //SCHACH_AI_EVALUATION_H