#include "util_board.h"

void calculate_occupancy(Board *board) {
    board->occupancyWhite = board->pawn_W | board->knight_W | board->bishop_W | board->rook_W | board->queen_W | board->king_W;
    board->occupancyBlack = board->pawn_B | board->knight_B | board->bishop_B | board->rook_B | board->queen_B | board->king_B;
    board->occupancy = board->occupancyWhite | board->occupancyBlack;
}

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
        if (MOVE_FROM(moves[i]) == from && MOVE_TO(moves[i]) == to && MOVE_IS_CAPTURE(moves[i]) == capture) {
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

// Hilfsfunktion: Castling-Rechte als String (z.B. KQkq)
void print_castling_rights(int castling) {
    printf("Castling Rights: ");
    if (castling == 0) {
        printf("-");
    } else {
        if (castling & 0x8) printf("K");
        if (castling & 0x4) printf("Q");
        if (castling & 0x2) printf("k");
        if (castling & 0x1) printf("q");
    }
    printf("\n");
}

// Hilfsfunktion: Bitboard als Brett drucken
void print_bitboard(Bitboard bb) {
    printf("  +-----------------+\n");
    for (int rank = 7; rank >= 0; --rank) {
        printf("%d | ", rank + 1);
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            printf("%c ", (bb & (1ULL << sq)) ? 'X' : '.');
        }
        printf("|\n");
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n");
}

void printBoard(const Board *board) {

    printf("\nCurrent Board:\n");
    printf("  +-----------------+\n");
    for (int rank = 7; rank >= 0; --rank) {
        printf("%d | ", rank + 1);
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            Bitboard square = 1ULL << sq;

            if (board->pawn_W & square) printf("P ");
            else if (board->knight_W & square) printf("N ");
            else if (board->bishop_W & square) printf("B ");
            else if (board->rook_W & square) printf("R ");
            else if (board->queen_W & square) printf("Q ");
            else if (board->king_W & square) printf("K ");
            else if (board->pawn_B & square) printf("p ");
            else if (board->knight_B & square) printf("n ");
            else if (board->bishop_B & square) printf("b ");
            else if (board->rook_B & square) printf("r ");
            else if (board->queen_B & square) printf("q ");
            else if (board->king_B & square) printf("k ");
            else printf(". ");
        }
        printf("|\n");
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n\n");

    // Meta Info
    printf("Turn: %s\n", board->turn == 0 ? "White" : "Black");
    print_castling_rights(board->castling);

    if (board->epSquare >= 0 && board->epSquare < 64) {
        int file = board->epSquare % 8;
        int rank = board->epSquare / 8;
        printf("En Passant Square: %c%d\n", 'a' + file, rank + 1);
    } else {
        printf("En Passant Square: -\n");
    }

    printf("White King Square: %c%d\n", 'a' + (board->whiteKingSq % 8), (board->whiteKingSq / 8) + 1);
    printf("Black King Square: %c%d\n", 'a' + (board->blackKingSq % 8), (board->blackKingSq / 8) + 1);

    printf("Zobrist Hash: 0x%016llx\n\n", board->hash);

    // Bitboards
    printf("Occupancy:\n");
    print_bitboard(board->occupancy);
    printf("\nOccupancy White:\n");
    print_bitboard(board->occupancyWhite);
    printf("\nOccupancy Black:\n");
    print_bitboard(board->occupancyBlack);

    printf("\nWhite Attacks:\n");
    print_bitboard(board->attacksWhite);
    printf("\nBlack Attacks:\n");
    print_bitboard(board->attacksBlack);
}