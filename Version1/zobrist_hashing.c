#include "zobrist_hashing.h"
#include "util.h"


uint64_t ZobristTable[8][8][12];
uint64_t rndState = 294260285;

unsigned long long int getZobristTable(int i, int j, int type) {
    return ZobristTable[i][j][type];
}

// Generates from Random number from 0 to 2^64-1
uint64_t randomInt() {
    const uint64_t z = 0x9FB21C651E98DF25;

    rndState ^= ((rndState << 49) | (rndState >> 15)) ^ ((rndState << 24) | (rndState >> 40));
    rndState *= z;
    rndState ^= rndState >> 35;
    rndState *= z;
    rndState ^= rndState >> 28;

    return rndState;
}


// Initializes the table
void initZobristTable() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 12; k++) {
                ZobristTable[i][j][k] = randomInt();
            }
        }
    }
}

// Computes the hash value of from given board
unsigned long long int computeHash(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    unsigned long long int h = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].type == ' ') { continue; }
            int piece = charPieceToEnumPiece(board[i][j].type, board[i][j].white);
            h ^= ZobristTable[i][j][piece];
        }
    }
    return h;
}

