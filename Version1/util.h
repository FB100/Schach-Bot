#ifndef SCHACH_UTIL_H
#define SCHACH_UTIL_H

#include "headers.h"

void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);

void fenToBoard(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]);

char *boardToFEN(const Piece board[BOARD_SIZE][BOARD_SIZE], bool whitePlays, int castlingRights);

bool isKingThreatened(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

Piece *createBoard(int boardSize);

bool hasLegalMove(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

int getPiecePrice(char pieceType);

void copyBoard(Piece oldBoard[BOARD_SIZE][BOARD_SIZE], Piece newBoard[BOARD_SIZE][BOARD_SIZE]);

void sortMoves(Move arr[100], int size);

#endif //SCHACH_UTIL_H
