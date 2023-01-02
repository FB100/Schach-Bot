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

// --- Structs ---
// Struktur für eine Schachfigur
typedef struct {
    char type;
    bool white;
} Piece;

#endif //SCHACH_HEADERS_H