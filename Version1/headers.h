#ifndef SCHACH_HEADERS_H
#define SCHACH_HEADERS_H


// --- Libraries ---
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <immintrin.h>

// --- Definitions ---
// Größen
#define BOARD_SIZE 8 //Don't change or things break
#define MAX_MOVE_ARRAY_SIZE 218

// Wert der Figuren
#define PAWN_PRICE 100
#define KNIGHT_PRICE 305
#define BISHOP_PRICE 313
#define ROOK_PRICE 563
#define QUEEN_PRICE 950

// Tablesizes
#define RT_SIZE 1024 //Repetition-table size
#define CS_SIZE 64 //Capturestack size (Min MAX_AI_DEPTH + 1)

// Sonstiges
#define MAX_MOVES 200 //Wie lange games maximal dauern dürfen. danach wird abgebrochen
#define MAX_AI_DEPTH 5 //Suchtiefe
#define MAX_ALPHA_BETA 10000000



// --- Structs ---
// Struktur für eine Schachfigur
typedef struct {
    char type;
    bool white;
} Piece;

enum PIECES {
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W,
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B
};

// Struktur eines Moves
typedef struct {
    int preEval;
    enum PIECES type;
    uint8_t special;
    uint8_t from;
    uint8_t to;
} Move;
/* spezial:
 * 0: normaler Move
 * 1: En Passant
 * 2: kurze Rochade
 * 3: lange Rochade
 * 4: Promotion Dame
 * 5: Promotion Turm
 * 6: Promotion Läufer
 * 7: Promotion Springer
*/

typedef uint64_t Bitboard;

typedef struct {
    Bitboard pawn_W;
    Bitboard knight_W;
    Bitboard bishop_W;
    Bitboard rook_W;
    Bitboard queen_W;
    Bitboard king_W;
    Bitboard pawn_B;
    Bitboard knight_B;
    Bitboard bishop_B;
    Bitboard rook_B;
    Bitboard queen_B;
    Bitboard king_B;
    int turn;
    int castling;
    int epSquare; //En Passant Square
    u_short whiteKingSq;
    u_short blackKingSq;
    Bitboard occupancy;
    Bitboard occupancyWhite;
    Bitboard occupancyBlack;
    uint64_t hash; //Zobrist hash
    Bitboard attacks; // The attack mask of the other side
} Board;

enum SQUARES {
    H1, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8
};

enum FILES {H, G, F, E, D, C, B, A};

#endif //SCHACH_HEADERS_H