#include "setup.h"

void calculate_occupancy(Board *board) {
    board->occupancyWhite = board->pawn_W | board->knight_W | board->bishop_W |
                            board->rook_W | board->queen_W | board->king_W;
    board->occupancyBlack = board->pawn_B | board->knight_B | board->bishop_B |
                            board->rook_B | board->queen_B | board->king_B;
    board->occupancy = board->occupancyWhite | board->occupancyBlack;
}

// Filtert einen Fen string aus einem UCI Position input
// Input format: position [fen <fenstring> | startpos ] moves <move1> .... <movei>
void position_input_to_fen(const char *input, char *fen_out, uint16_t max_len) {
    const char *start = strstr(input, "position fen ");
    if (!start) {
        fen_out[0] = '\0'; // keine Züge gefunden
        return;
    }

    start += strlen("position fen ");
    const char *end = strstr(start, " moves");

    size_t len = end ? (size_t) (end - start) : strlen(start);
    if (len >= max_len) len = max_len - 1;

    strncpy(fen_out, start, len);
    fen_out[len] = '\0';
}

// Filtert die moves aus einem UCI Position input.
// Input format: position [fen <fenstring> | startpos ] moves <move1> .... <movei>
void position_input_to_moves(const char *input, char *moves_out, uint16_t max_len) {
    const char *start = strstr(input, "moves ");
    if (!start) {
        moves_out[0] = '\0'; // keine Züge gefunden
        return;
    }

    start += strlen("moves ");
    size_t len = strlen(start);
    if (len >= max_len) len = max_len - 1;

    strncpy(moves_out, start, len);
    moves_out[len] = '\0';
}

// Funktion zum Konvertieren eines FEN-Zeichens in ein Bitboard Board
void set_position_from_FEN(Board *board, const char *fen) {
    memset(board, 0, sizeof(Board)); // Initialisiert das Board mit 0

    int rank = 7, file = 0;

    // Verarbeite die FEN-Zeichenkette
    while (*fen && *fen != ' ') {
        char c = *fen;

        if (isdigit(c)) {
            file += c - '0';
        } else if (c == '/') {
            rank--;
            file = 0;
        } else {
            set_bit_by_char(board, c, rank, file);
            file++;
        }
        fen++;
    }

    fen++; // Überspringe das Leerzeichen
    board->turn = (*fen == 'w') ? 0 : 1; // Setze die Zugseite

    // Verarbeite die Rochaderechte
    fen += 2; // Überspringe das Leerzeichen und 'w'/'b'
    board->castling = 0;
    while (*fen && *fen != ' ') {
        switch (*fen) {
            case 'K':
                board->castling |= 1;
                break;
            case 'Q':
                board->castling |= 2;
                break;
            case 'k':
                board->castling |= 4;
                break;
            case 'q':
                board->castling |= 8;
                break;
        }
        fen++;
    }

    // Verarbeite En-Passant
    fen++; // Überspringe das Leerzeichen
    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';
        board->epSquare = square_from_rank_and_file(rank, file);
    } else {
        board->epSquare = -1;
    }

    calculate_occupancy(board);

    board->hash = 0;

    // TODO Attackbitboard for legal move checking
    board->attacksWhite = 0;
    board->attacksBlack = 0;

}

void set_position(Board *board, const char *input) {
    uint16_t max_len = strlen(input);
    if (strncmp(input + strlen("position "), "startpos", 8) == 0) {
        set_position_from_FEN(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    } else {
        char *fen = malloc(max_len);
        if (!fen) {
            fprintf(stderr, "Speicher konnte nicht allokiert werden\n");
            return;
        }
        position_input_to_fen(input, fen, max_len);
        set_position_from_FEN(board, fen);
        free(fen);
    }
    char *moves = malloc(max_len);
    if (!moves) {
        fprintf(stderr, "Speicher konnte nicht allokiert werden\n");
        return;
    }
    position_input_to_moves(input, moves, max_len);
    apply_move_string(board, moves);
    free(moves);

    calculate_occupancy(board);
    update_attackmaps(board);
    // TODO Zobrist Hash
}