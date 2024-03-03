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

// --- Definitions ---
// Größe des Schachbretts
#define BOARD_SIZE 8
#define MAX_ALPHPA_BETA 10000000

// Wert der Figuren
#define PAWN_PRICE 100
#define KNIGHT_PRICE 305
#define BISHOP_PRICE 323
#define ROOK_PRICE 563
#define QUEEN_PRICE 950

// Tablesizes
#define RT_SIZE 512 //Repetition-table size

// Sonstiges
#define AI_ONLY_MAX_MOVES 200 //Wie lange aionly games maximal dauern dürfen. danach wird abgebrochen


// --- Structs ---
// Struktur für eine Schachfigur
typedef struct {
    char type;
    bool white;
} Piece;

// Struktur eines Moves
typedef struct {
    int preEval;
    uint8_t special;
    uint8_t from;
    uint8_t to;

} Move;

#endif //SCHACH_HEADERS_H