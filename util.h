#ifndef SCHACH_UTIL_H
#define SCHACH_UTIL_H

#include "headers.h"


bool isKingThreatened(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

void addBoardToArray(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], int index);

bool hasLegalMove(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

int getPiecePrice(char pieceType);

void copyBoard(Piece oldBoard[BOARD_SIZE][BOARD_SIZE], Piece newBoard[BOARD_SIZE][BOARD_SIZE]);

#endif //SCHACH_UTIL_H
