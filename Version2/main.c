#include "main.h"

void uci_loop();


int main() {
    uci_loop();
    return 0;
}

// Das UCI Interface
void uci_loop() {
    char input[2048];

    while (1) {
        if (!fgets(input, 2048, stdin)) {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "uci") == 0) {
            printf("id name FabiBot\n");
            printf("id author Fabian Böhm\n");
            printf("uciok\n");
        } else if (strcmp(input, "isready") == 0) {
            printf("readyok\n");
        } else if (strncmp(input, "position", 8) == 0) {
            //TODO set_position_from_FEN(input);
        } else if (strncmp(input, "go", 2) == 0) {
            //TODO search_position();
        } else if (strcmp(input, "quit") == 0) {
            break;
        }
    }
}

// Hilfsfunktion zum Setzen eines Bits im Bitboard
void set_bit(Bitboard *board, int square) {
    *board |= (1ULL << square);
}

// Hilfsfunktion zur Berechnung des Indexes eines Schachbrettfeldes
int square_index(int rank, int file) {
    return (rank * 8) + file;
}

// Hilfsfunktion, die anhand eines Chars das Bitboard updated
void set_bit_by_char(Board *board, char c, int rank, int file) {
    int square = square_index(rank, file);

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

void calculate_occupancy(Board *board){
    board->occupancyWhite = board->pawn_W | board->knight_W | board->bishop_W |
                            board->rook_W | board->queen_W | board->king_W;
    board->occupancyBlack = board->pawn_B | board->knight_B | board->bishop_B |
                            board->rook_B | board->queen_B | board->king_B;
    board->occupancy = board->occupancyWhite | board->occupancyBlack;
}

// Funktion zum Konvertieren eines FEN-Zeichens in ein Bitboard
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
        board->epSquare = square_index(rank, file);
    } else {
        board->epSquare = -1;
    }

    calculate_occupancy(board);

    // TODO Zobrist Hash
    board->hash = 0;

    // TODO Attackbitboard for legal move checking
    board->attacks = 0;
}
