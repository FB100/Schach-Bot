#include <stdio.h>
#include <string.h>
#include "moves_external.h"

// Hilfsfunktion, um einen Square-namen in einen internen Square umzuwandeln
int squarename_to_square(const char *square) {
    // z.B. "g4" → File g, Rank 4
    if (!isalpha(square[0]) || !isdigit(square[1])) return -1;
    return (square[1] - '1') * 8 + (square[0] - 'a');
}

uint8_t is_piece_white(enum PIECES piece) {
    if (piece <= KING_W) return true;
    return false;
}

enum PIECES get_piece_on_square(Board *board, int square) {
    Bitboard mask = 1ULL << square;
    if (board->pawn_W & mask) return PAWN_W;
    if (board->knight_W & mask) return KNIGHT_W;
    if (board->bishop_W & mask) return BISHOP_W;
    if (board->rook_W & mask) return ROOK_W;
    if (board->queen_W & mask) return QUEEN_W;
    if (board->king_W & mask) return KING_W;
    if (board->pawn_B & mask) return PAWN_B;
    if (board->knight_B & mask) return KNIGHT_B;
    if (board->bishop_B & mask) return BISHOP_B;
    if (board->rook_B & mask) return ROOK_B;
    if (board->queen_B & mask) return QUEEN_B;
    if (board->king_B & mask) return KING_B;
    return -1;
}

void place_piece_at(Board *board, enum PIECES piece, int square) {
    Bitboard mask = 1ULL << square;
    switch (piece) {
        case PAWN_W:
            board->pawn_W |= mask;
            break;
        case KNIGHT_W:
            board->knight_W |= mask;
            break;
        case BISHOP_W:
            board->bishop_W |= mask;
            break;
        case ROOK_W:
            board->rook_W |= mask;
            break;
        case QUEEN_W:
            board->queen_W |= mask;
            break;
        case KING_W:
            board->king_W |= mask;
            board->whiteKingSq = square;
            break;
        case PAWN_B:
            board->pawn_B |= mask;
            break;
        case KNIGHT_B:
            board->knight_B |= mask;
            break;
        case BISHOP_B:
            board->bishop_B |= mask;
            break;
        case ROOK_B:
            board->rook_B |= mask;
            break;
        case QUEEN_B:
            board->queen_B |= mask;
            break;
        case KING_B:
            board->king_B |= mask;
            board->blackKingSq = square;
            break;
    }
}

void remove_piece_at(Board *board, int square) {
    Bitboard mask = ~(1ULL << square);
    board->pawn_W &= mask;
    board->knight_W &= mask;
    board->bishop_W &= mask;
    board->rook_W &= mask;
    board->queen_W &= mask;
    board->king_W &= mask;
    board->pawn_B &= mask;
    board->knight_B &= mask;
    board->bishop_B &= mask;
    board->rook_B &= mask;
    board->queen_B &= mask;
    board->king_B &= mask;
}


// Hilfsfunktion zum Setzen eines Bits im Bitboard
void set_bit(Bitboard *board, int square) {
    *board |= (1ULL << square);
}

// Hilfsfunktion zur Berechnung des Index eines Schachbrettfeldes
int square_from_rank_and_file(int rank, int file) {
    return (rank * 8) + file;
}

// Hilfsfunktion, die anhand eines Chars das Bitboard updated
void set_bit_by_char(Board *board, char c, int rank, int file) {
    int square = square_from_rank_and_file(rank, file);

    switch (c) {
        case 'P':
            set_bit(&board->pawn_W, square);
            break;
        case 'N':
            set_bit(&board->knight_W, square);
            break;
        case 'B':
            set_bit(&board->bishop_W, square);
            break;
        case 'R':
            set_bit(&board->rook_W, square);
            break;
        case 'Q':
            set_bit(&board->queen_W, square);
            break;
        case 'K':
            set_bit(&board->king_W, square);
            board->whiteKingSq = square;
            break;
        case 'p':
            set_bit(&board->pawn_B, square);
            break;
        case 'n':
            set_bit(&board->knight_B, square);
            break;
        case 'b':
            set_bit(&board->bishop_B, square);
            break;
        case 'r':
            set_bit(&board->rook_B, square);
            break;
        case 'q':
            set_bit(&board->queen_B, square);
            break;
        case 'k':
            set_bit(&board->king_B, square);
            board->blackKingSq = square;
            break;
        default:
            break;
    }
}


void move_piece_with_rules(Board *board, int from, int to, char promo) {
    enum PIECES piece = get_piece_on_square(board, from);
    if (piece == -1) return;

    int is_white = is_piece_white(piece);
    int direction = is_white ? 8 : -8;

    // En Passant
    if ((piece == PAWN_W || piece == PAWN_B) && to == board->epSquare) {
        int captured_sq = to - direction;
        remove_piece_at(board, captured_sq);
    }

    // Rochade
    if ((piece == KING_W || piece == KING_B) && abs(from - to) == 2) {
        int kingside = to > from;
        int rook_from = kingside ? from + 3 : from - 4;
        int rook_to = kingside ? from + 1 : from - 1;
        char rook = is_white ? ROOK_W : ROOK_B;
        remove_piece_at(board, rook_from);
        place_piece_at(board, rook, rook_to);
    }

    // Normale Bewegung (inklusive Schlagen)
    remove_piece_at(board, to);  // Gegner eventuell schlagen
    remove_piece_at(board, from);

    // Umwandlung
    if ((piece == PAWN_W && to / 8 == 7) || (piece == PAWN_B && to / 8 == 0)) {
        char promo_queen = is_white ? 'Q' : 'q';
        char promo_piece = promo ? promo : promo_queen;
        set_bit_by_char(board, promo_piece, to / 8, to % 8);
        board->turn ^= 1; // Zugrecht wechseln
        return;
    }

    place_piece_at(board, piece, to); //


    // En Passant-Ziel aktualisieren
    if ((piece == PAWN_W || piece == PAWN_B) && abs(from - to) == 16) {
        board->epSquare = (from + to) / 2;
    } else {
        board->epSquare = -1;
    }

    // Zugrecht wechseln
    board->turn ^= 1;
}

void remove_all_chars(char *string, char characterToRemove) {
    char *readPointer = string, *writePointer = string;
    while (*readPointer) {
        *writePointer = *readPointer;
        readPointer++;
        writePointer += (*writePointer != characterToRemove);
    }
    *writePointer = '\0';
}

void apply_move_string(Board *board, char *moves) {

    char move[6];
    int from, to;
    remove_all_chars(moves, 'x');
    remove_all_chars(moves, '+');

    while (*moves) {
        if (strncmp(moves, "0-0-0", 5) == 0) {
            from = board->turn ? board->blackKingSq : board->whiteKingSq;
            to = board->turn ? 58 : 3;
            move_piece_with_rules(board, from, to, 0);
            while (*moves && !isspace(*moves)) ++moves; // bearbeiteten Move überspringen
            while (*moves && isspace(*moves)) ++moves; // Lehrzeichen überspringen
            continue;
        } else if (strncmp(moves, "0-0", 3) == 0) {
            from = board->turn ? board->blackKingSq : board->whiteKingSq;
            to = board->turn ? 62 : 6;
            move_piece_with_rules(board, from, to, 0);
            while (*moves && !isspace(*moves)) ++moves; // bearbeiteten Move überspringen
            while (*moves && isspace(*moves)) ++moves; // Lehrzeichen überspringen
            continue;
        }

        if (sscanf(moves, "%5s", move) != 1) break;
        from = squarename_to_square(move);
        to = squarename_to_square(move + 2);
        char promo = 0;
        if (board->turn) {
            promo = move[4] ? tolower(move[4]) : 0;
        } else {
            promo = move[4] ? toupper(move[4]) : 0;
        }
        move_piece_with_rules(board, from, to, promo);
        while (*moves && !isspace(*moves)) ++moves; // bearbeiteten Move überspringen
        while (*moves && isspace(*moves)) ++moves; // Lehrzeichen überspringen
    }
}