#ifndef SCHACH_UTIL_H
#define SCHACH_UTIL_H

#include "headers.h"

#ifdef _MSC_VER
#include <intrin.h>
#ifdef _WIN64
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#define USING_INTRINSICS
#endif
#elif defined(__GNUC__) && defined(__LP64__)

static inline unsigned char _BitScanForward64(unsigned long *Index, uint64_t Mask) {
    uint64_t Ret;
    __asm__
            (
            "bsfq %[Mask], %[Ret]"
            :[Ret] "=r"(Ret)
    :[Mask] "mr"(Mask)
    );
    *Index = (unsigned long) Ret;
    return Mask ? 1 : 0;
}

static inline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask) {
    uint64_t Ret;
    __asm__(
            "bsrq %[Mask], %[Ret]"
            :[Ret] "=r"(Ret)
    :[Mask] "mr"(Mask)
    );
    *Index = (unsigned long) Ret;
    return Mask ? 1 : 0;
}

#define USING_INTRINSICS
#endif


void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);

void printBitBoard(uint64_t bitboard);

void fenToBoard(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]);

void fenToBitBoardBoard(const char *fen, Board *bitboard);

char *boardToFEN(const Piece board[BOARD_SIZE][BOARD_SIZE], bool whitePlays, int castlingRights);

bool isKingThreatened(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

Piece *createBoard(int boardSize);

bool hasLegalMove(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn);

int getPiecePrice(char pieceType);

void copyBoard(Piece oldBoard[BOARD_SIZE][BOARD_SIZE], Piece newBoard[BOARD_SIZE][BOARD_SIZE]);

void sortMoves(int size, Move arr[size]);

uint64_t popLSB(uint64_t *num);


#endif //SCHACH_UTIL_H
