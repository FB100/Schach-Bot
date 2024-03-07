#ifndef SCHACH_AI_GENERATION_H
#define SCHACH_AI_GENERATION_H

#include "headers.h"

int getPawnMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getKnightMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getBishopMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getRookMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getQueenMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getKingMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index);

int getAllPseudoMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int castlingRights);

#endif //SCHACH_AI_GENERATION_H
