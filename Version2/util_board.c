#include "util_board.h"


// Hilfsfunktion: Konvertiert Feld-Index in String
void square_to_string(uint8_t square, char *out) {
    uint8_t file = square % 8;
    uint8_t rank = square / 8;
    out[0] = 'a' + file;
    out[1] = '1' + rank;
    out[2] = '\0';
}

// Hilfsfunktion, um einen Square-namen in einen internen Square umzuwandeln
int string_to_square(const char *string) {
    // z.B. "g4" → File g, Rank 4
    if (!isalpha(string[0]) || !isdigit(string[1])) return -1;
    return (string[1] - '1') * 8 + (string[0] - 'a');
}

// Hilfsfunktion: Konvertiert Promotion-Zahl zu Buchstaben
char promo_to_char(int promo) {
    switch (promo) {
        case 1:
            return 'n';
        case 2:
            return 'b';
        case 3:
            return 'r';
        case 4:
            return 'q';
        default:
            return '\0';
    }
}

// Hilfsfunktion: Prüft, ob Figur auf bestimmtem Feld steht
int is_piece_on_square(Bitboard piece_bb, const char *square) {
    int index = (square[1] - '1') * 8 + (square[0] - 'a');
    if (index < 0) return 0;
    return ((piece_bb >> index) & 1ULL) == 1;
}

// Hilfsfunktion: Setzt ein einzelnes Bit im Bitboard
Bitboard square_to_bit(int square) {
    return 1ULL << square;
}

// Hilfsfunktion: Gibt zurück, ob ein Move in der Liste existiert
int move_exists(const Move *moves, int count, int from, int to, int capture) {
    for (int i = 0; i < count; ++i) {
        if (MOVE_FROM(moves[i]) == from &&
            MOVE_TO(moves[i]) == to &&
            MOVE_IS_CAPTURE(moves[i]) == capture) {
            return 1;
        }
    }
    return 0;
}

// Hilfsfunktion: Gibt an, ob ein Piece Weiß ist
uint8_t is_piece_white(enum PIECES piece) {
    if (piece <= KING_W) return true;
    return false;
}