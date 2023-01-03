#ifndef SCHACH_AI_GENERATION_H
#define SCHACH_AI_GENERATION_H

#include "headers.h"

int getPawnMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

int getKnightMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

int getBishopMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

int getRookMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

int getQueenMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

int getKingMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index);

#endif //SCHACH_AI_GENERATION_H
