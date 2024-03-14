#include "precompute_bitboards.h"
#include "../util.h"

void precomputeKnightMoves() {
    uint64_t moves[64] = {0};
    //Relative Position der Felder, auf die ein Night ziehen kann
    int8_t knightMoves[8][2] = {{2,  1},
                                {2,  -1},
                                {-2, 1},
                                {-2, -1},
                                {1,  2},
                                {1,  -2},
                                {-1, 2},
                                {-1, -2}};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Alle 8 möglichen Züge für einen Springer
            uint64_t bitboard = moves[8*i+j];
            for (int k = 0; k < 8; k++) {
                int di = (int) knightMoves[k][0];
                int dj = (int) knightMoves[k][1];
                if (i + di < 0 || i + di >= BOARD_SIZE || j + dj < 0 || j + dj >= BOARD_SIZE)
                    continue; // Zug außerhalb des Bretts
                bitboard ^= 1ULL << (8*(i+di)+(j+dj));
            }
            moves[8*i+j] = bitboard;
            printBitBoard(bitboard);
        }
    }
}