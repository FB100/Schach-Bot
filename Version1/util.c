#include "util.h"
#include "board.h"
#include "headers.h"

void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    printf("      A   B   C   D   E   F   G   H\n");
    printf("  *   *   *   *   *   *   *   *   *   *\n");
    printf("  * --------------------------------- *\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d * | ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            char c = board[i][j].type;
            if (c == ' ') {
                printf("  | ");
            } else {
                if (board[i][j].white) {
                    printf("%c | ", c);
                } else {
                    printf("%c | ", tolower(c));
                }
            }
        }
        printf("* %d\n", BOARD_SIZE - i);
        printf("  * --------------------------------- *\n");
    }
    printf("  *   *   *   *   *   *   *   *   *   *\n");
    printf("      A   B   C   D   E   F   G   H\n");
}

void printBitBoard(uint64_t bitboard) {
    printf("%lu\n", bitboard);
    char printBoard[64] = {0};

    unsigned long x;
    do {
        x = popLSB(&bitboard);
        printBoard[x] = 1;
    } while (bitboard);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (printBoard[i * BOARD_SIZE + j] == 1) {
                printf("1 ");
            } else {
                printf("0 ");
            }

        }
        printf("\n");
    }
    printf("\n");
}

// Funktion, die einen FEN-String in das interne Schachbrett-Format umwandelt
void fenToBoard(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Alle Felder leer machen
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].type = ' ';
            board[i][j].white = 0;
        }
    }

    // Zeiger auf den aktuellen Zeichen im FEN-String
    const char *ptr = fen;

    // Reihe durchlaufen
    for (int i = 0; i < BOARD_SIZE; i++) {
        int j = 0;
        // Zeichen im FEN-String durchlaufen
        while (*ptr != '\0' && *ptr != '/') {
            // Zahl, die leere Felder anzeigt
            if (*ptr >= '1' && *ptr <= '8') {
                int numEmpty = *ptr - '0';
                j += numEmpty;
            } else {
                // Schachstein
                board[i][j].type = (char) toupper(*ptr);
                board[i][j].white = isupper(*ptr);
                j++;
            }
            ptr++;
        }
        if (*ptr == '/') {
            ptr++;
        }
    }
}

// Funktion, die einen FEN-String in das interne Schachbrett-Format umwandelt
void fenToBitBoardBoard(const char *fen, Board *bitboard) {
    // Alle Felder leer machen
    bitboard->pawn_W = 0;
    bitboard->knight_W = 0;
    bitboard->bishop_W = 0;
    bitboard->rook_W = 0;
    bitboard->queen_W = 0;
    bitboard->king_W = 0;
    bitboard->pawn_B = 0;
    bitboard->knight_B = 0;
    bitboard->bishop_B = 0;
    bitboard->rook_B = 0;
    bitboard->queen_B = 0;
    bitboard->king_B = 0;
    bitboard->turn = 0;
    bitboard->castling = 0;
    bitboard->epSquare = 0; //En Passant Square
    bitboard->whiteKingSq = 0;
    bitboard->blackKingSq = 0;
    bitboard->occupancy = 0;
    bitboard->occupancyWhite = 0;
    bitboard->occupancyBlack = 0;
    bitboard->hash = 0; //Zobrist hash
    bitboard->attacks = 0; // The attack mask of the other side

    //TODO rest der Felder ausfüllen

    // Zeiger auf den aktuellen Zeichen im FEN-String
    const char *ptr = fen;

    // Reihe durchlaufen
    for (int i = 0; i < BOARD_SIZE; i++) {
        int j = 0;
        // Zeichen im FEN-String durchlaufen
        while (*ptr != '\0' && *ptr != '/') {
            // Zahl, die leere Felder anzeigt
            if (*ptr >= '1' && *ptr <= '8') {
                int numEmpty = *ptr - '0';
                j += numEmpty;
            } else {
                // Schachstein
                switch (*ptr) {
                    case 'p':
                        bitboard->pawn_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'n':
                        bitboard->knight_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'b':
                        bitboard->bishop_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'r':
                        bitboard->rook_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'q':
                        bitboard->queen_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'k':
                        bitboard->king_B ^= 1ULL << (8 * i + j);
                        break;
                    case 'P':
                        bitboard->pawn_W ^= 1ULL << (8 * i + j);
                        break;
                    case 'N':
                        bitboard->knight_W ^= 1ULL << (8 * i + j);
                        break;
                    case 'B':
                        bitboard->bishop_W ^= 1ULL << (8 * i + j);
                        break;
                    case 'R':
                        bitboard->rook_W ^= 1ULL << (8 * i + j);
                        break;
                    case 'Q':
                        bitboard->queen_W ^= 1ULL << (8 * i + j);
                        break;
                    case 'K':
                        bitboard->king_W ^= 1ULL << (8 * i + j);
                        break;
                    default:
                        break;
                }
                j++;
            }
            ptr++;
        }
        if (*ptr == '/') {
            ptr++;
        }
    }
    computeOccupancyMasks(bitboard);
}

char *boardToFEN(const Piece board[BOARD_SIZE][BOARD_SIZE], bool whitePlays, int castlingRights) {
    char fen[(BOARD_SIZE * BOARD_SIZE) + 20];
    int index = 0;

    // Piece placement data
    for (int rank = BOARD_SIZE - 1; rank >= 0; rank--) {
        int emptySquares = 0;
        for (int file = 0; file < BOARD_SIZE; file++) {
            char type = board[rank][file].type;
            bool white = board[rank][file].white;
            if (type == ' ') {
                emptySquares++;
            } else {
                if (emptySquares > 0) {
                    fen[index++] = emptySquares + '0';
                    emptySquares = 0;
                }
                if (white) {
                    fen[index++] = type;
                } else {
                    fen[index++] = (char) toupper(type);
                }
            }
        }
        if (emptySquares > 0) {
            fen[index++] = emptySquares + '0';
        }
        if (rank > 0) {
            fen[index++] = '/';
        }
    }
    fen[index++] = ' ';

    // Active color
    fen[index++] = whitePlays ? 'w' : 'b';
    fen[index++] = ' ';

    // Castling availability (assuming no castling available)
    fen[index++] = '-';  // Replace with actual castling availability if needed
    fen[index++] = ' ';


    char *result = strdup(fen);
    return result;
}




// Funktion, die überprüft, ob der König eines Spielers bedroht wird
bool isKingThreatened(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
    // Finde den König
    int ki = -1;
    int kj = -1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].type == 'K' && board[i][j].white == whiteTurn) {
                ki = i;
                kj = j;
                break;
            }
        }
    }
    if (ki == -1 || kj == -1) {
        return true;
    }
    // Überprüfe, ob der König von einem gegnerischen Bauern bedroht wird
    if (whiteTurn) {
        if (ki > 0 && kj > 0 && board[ki - 1][kj - 1].type == 'P' && !board[ki - 1][kj - 1].white) return true;
        if (ki > 0 && kj < BOARD_SIZE - 1 && board[ki - 1][kj + 1].type == 'P' && !board[ki - 1][kj + 1].white) return true;
    } else {
        if (ki < BOARD_SIZE - 1 && kj > 0 && board[ki + 1][kj - 1].type == 'P' && board[ki + 1][kj - 1].white) return true;
        if (ki < BOARD_SIZE - 1 && kj < BOARD_SIZE - 1 && board[ki + 1][kj + 1].type == 'P' && board[ki + 1][kj + 1].white) return true;
    }
    // Überprüfe, ob der König von einem gegnerischen Springer bedroht wird
    for (int di = -2; di <= 2; di++) {
        for (int dj = -2; dj <= 2; dj++) {
            if (abs(di) + abs(dj) != 3) continue; // Keine Läuferbewegung
            int ni = ki + di;
            int nj = kj + dj;
            if (ni < 0 || ni >= BOARD_SIZE || nj < 0 || nj >= BOARD_SIZE) continue; // Zug außerhalb des Bretts
            if (board[ni][nj].type == 'N' && board[ni][nj].white != whiteTurn) return true; // Gegnerischer Springer
        }
    }
    // Überprüfe, ob der König von einem gegnerischen Läufer oder Dame bedroht wird
    // Prüfe alle 4 Diagonalen
    for (int di = -1; di <= 1; di += 2) {
        for (int dj = -1; dj <= 1; dj += 2) {
            for (int k = 1; k < BOARD_SIZE; k++) {
                int ni = ki + di * k;
                int nj = kj + dj * k;
                if (ni < 0 || ni >= BOARD_SIZE || nj < 0 || nj >= BOARD_SIZE) break; // Zug außerhalb des Bretts
                if (board[ni][nj].type == 'B' || board[ni][nj].type == 'Q') { // Gegnerischer Läufer oder Dame
                    if (board[ni][nj].white != whiteTurn) return true;
                    break;
                } else if (board[ni][nj].type != ' ') { // Anderer Stein im Weg
                    break;
                }
            }
        }
    }
    // Überprüfe, ob der König von einem gegnerischen Turm oder Dame bedroht wird
    // Horizontal
    for (int di = -1; di <= 1; di += 2) {
        for (int k = 1; k < BOARD_SIZE; k++) {
            int ni = ki + di * k;
            int nj = kj;
            if (ni < 0 || ni >= BOARD_SIZE) break; // Zug außerhalb des Bretts
            if (board[ni][nj].type == 'R' || board[ni][nj].type == 'Q') { // Gegnerischer Turm oder Dame
                if (board[ni][nj].white != whiteTurn) return true;
                break;
            } else if (board[ni][nj].type != ' ') { // Anderer Stein im Weg
                break;
            }
        }
    }
    // Vertikal
    for (int dj = -1; dj <= 1; dj++) {
        for (int k = 1; k < BOARD_SIZE; k++) {
            int ni = ki;
            int nj = kj + dj * k;
            if (nj < 0 || nj >= BOARD_SIZE) break; // Zug außerhalb des Bretts
            if (board[ni][nj].type == 'R' || board[ni][nj].type == 'Q') { // Gegnerischer Turm oder Dame
                if (board[ni][nj].white != whiteTurn) return true;
                break;
            } else if (board[ni][nj].type != ' ') { // Anderer Stein im Weg
                break;
            }
        }
    }
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (ki + di >= 0 && ki + di < BOARD_SIZE && kj + dj >= 0 && kj + dj < BOARD_SIZE) {
                if (board[ki + di][kj + dj].type == 'K' && !(di == 0 && dj == 0)) { // Gegnerischer König
                    return true;
                }
            }
        }
    }
    // König wird von keiner Figur bedroht
    return false;
}


Piece *createBoard(int boardSize) {
    Piece (*board)[] = malloc(sizeof(Piece) * boardSize * boardSize);
    return (Piece *) board;
}


bool hasLegalMove(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {

    int nightMoves[8][2] = {{2,  1},
                            {2,  -1},
                            {-2, 1},
                            {-2, -1},
                            {1,  2},
                            {1,  -2},
                            {-1, 2},
                            {-1, -2}};

    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p;
    Piece p2;


    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            p = board[i][j];
            int direction = whiteTurn ? -1 : 1;
            if (p.white == whiteTurn) {
                switch (p.type) {
                    case 'P':
                        // Bauern

                        // Ein Feld vorwärts
                        if (board[i + direction][j].type == ' ') {
                            board[i][j].type = ' ';
                            board[i + direction][j] = p;
                            if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
                                board[i + direction][j].type = 'Q';
                            }
                            if (!isKingThreatened(board, whiteTurn)) {
                                return true;
                            }
                            board[i + direction][j].type = ' ';
                            board[i][j] = p;
                            // Zwei Felder vorwärts (nur möglich von Startposition)
                            if ((whiteTurn && i == 6) || (!whiteTurn && i == 1)) {
                                if (board[i + 2 * direction][j].type == ' ') {
                                    board[i][j].type = ' ';
                                    board[i + 2 * direction][j] = p;
                                    if (!isKingThreatened(board, whiteTurn)) {
                                        return true;
                                    }
                                    board[i + 2 * direction][j].type = ' ';
                                    board[i][j] = p;
                                }
                            }
                        }
                        // Diagonal schlagen (nur möglich bei gegnerischem piece)
                        if (j + 1 < BOARD_SIZE) {
                            if (board[i + direction][j + 1].white != whiteTurn &&
                                board[i + direction][j + 1].type != ' ') {
                                p2 = board[i + direction][j + 1];
                                board[i][j].type = ' ';
                                board[i + direction][j + 1] = p;
                                if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
                                    board[i + direction][j + 1].type = 'Q';
                                }
                                if (!isKingThreatened(board, whiteTurn)) {
                                    return true;
                                }
                                board[i + direction][j + 1] = p2;
                                board[i][j] = p;
                            }
                        }
                        if (j - 1 >= 0) {
                            if (board[i + direction][j - 1].white != whiteTurn &&
                                board[i + direction][j - 1].type != ' ') {
                                p2 = board[i + direction][j - 1];
                                board[i][j].type = ' ';
                                board[i + direction][j - 1] = p;
                                if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
                                    board[i + direction][j - 1].type = 'Q';
                                }
                                if (!isKingThreatened(board, whiteTurn)) {
                                    return true;
                                }
                                board[i + direction][j - 1] = p2;
                                board[i][j] = p;
                            }
                        }
                        break;

                    case 'N':
                        // Alle 8 möglichen Züge für einen Springer
                        for (int k = 0; k < 8; k++) {
                            int di = nightMoves[k][0];
                            int dj = nightMoves[k][1];
                            if (i + di < 0 || i + di >= BOARD_SIZE || j + dj < 0 || j + dj >= BOARD_SIZE)
                                continue; // Zug außerhalb des Bretts
                            if (board[i + di][j + dj].white == whiteTurn && board[i + di][j + dj].type != ' ') continue; // Zug auf eigenen Stein
                            p2 = board[i + di][j + dj];
                            board[i][j].type = ' ';
                            board[i + di][j + dj] = p;
                            if (!isKingThreatened(board, whiteTurn)) {
                                return true;
                            }
                            board[i + di][j + dj] = p2;
                            board[i][j] = p;
                        }
                        break;

                    case 'B':
                        // Bewege den Läufer in alle 4 Richtungen, solange kein Stein im Weg ist
                        for (int di = -1; di <= 1; di += 2) {
                            for (int dj = -1; dj <= 1; dj += 2) {
                                int ni = i + di;
                                int nj = j + dj;
                                while (ni >= 0 && ni < BOARD_SIZE && nj >= 0 &&
                                       nj < BOARD_SIZE) { // Zug innerhalb des Bretts
                                    if (board[ni][nj].type != ' ') { // Stein im Weg
                                        if (board[ni][nj].white != whiteTurn && board[ni][nj].type != ' ') { // Gegnerischer Stein
                                            p2 = board[ni][nj];
                                            board[i][j].type = ' ';
                                            board[ni][nj] = p;
                                            if (!isKingThreatened(board, whiteTurn)) {
                                                return true;
                                            }
                                            board[ni][nj] = p2;
                                            board[i][j] = p;
                                        }
                                        break; // Beende Schleife
                                    }
                                    board[i][j].type = ' ';
                                    board[ni][nj] = p;
                                    if (!isKingThreatened(board, whiteTurn)) {
                                        return true;
                                    }
                                    board[ni][nj].type = ' ';
                                    board[i][j] = p;
                                    ni += di;
                                    nj += dj;
                                }
                            }
                        }
                        break;

                    case 'R':
                        // Bewege den Turm in alle 4 Richtungen, solange kein Stein im Weg ist
                        for (int di = -1; di <= 1; di += 2) {
                            int ni = i + di;
                            while (ni >= 0 && ni < BOARD_SIZE) { // Zug innerhalb des Bretts
                                if (board[ni][j].type != ' ') { // Stein im Weg
                                    if (board[ni][j].white != whiteTurn && board[ni][j].type != ' ') { // Gegnerischer Stein
                                        p2 = board[ni][j];
                                        board[i][j].type = ' ';
                                        board[ni][j] = p;
                                        if (!isKingThreatened(board, whiteTurn)) {
                                            return true;
                                        }
                                        board[ni][j] = p2;
                                        board[i][j] = p;
                                    }
                                    break; // Beende Schleife
                                }
                                board[i][j].type = ' ';
                                board[ni][j] = p;
                                if (!isKingThreatened(board, whiteTurn)) {
                                    return true;
                                }
                                board[ni][j].type = ' ';
                                board[i][j] = p;
                                ni += di;
                            }
                        }
                        for (int dj = -1; dj <= 1; dj += 2) {
                            int nj = j + dj;
                            while (nj >= 0 && nj < BOARD_SIZE) { // Zug innerhalb des Bretts
                                if (board[i][nj].type != ' ') { // Stein im Weg
                                    if (board[i][nj].white != whiteTurn && board[i][nj].type != ' ') { // Gegnerischer Stein
                                        p2 = board[i][nj];
                                        board[i][j].type = ' ';
                                        board[i][nj] = p;
                                        if (!isKingThreatened(board, whiteTurn)) {
                                            return true;
                                        }
                                        board[i][nj] = p2;
                                        board[i][j] = p;
                                    }
                                    break; // Beende Schleife
                                }
                                board[i][j].type = ' ';
                                board[i][nj] = p;
                                if (!isKingThreatened(board, whiteTurn)) {
                                    return true;
                                }
                                board[i][nj].type = ' ';
                                board[i][j] = p;
                                nj += dj;
                            }
                        }
                        break;

                    case 'Q':
                        // Bewege die Dame in alle 4 Richtungen, solange kein Stein im Weg ist
                        for (int di = -1; di <= 1; di++) {
                            for (int dj = -1; dj <= 1; dj++) {
                                if (di == 0 && dj == 0)
                                    continue; // Kein Zug auf die aktuelle Position
                                int ni = i + di;
                                int nj = j + dj;
                                while (ni >= 0 && ni < BOARD_SIZE && nj >= 0 &&
                                       nj < BOARD_SIZE) { // Zug innerhalb des Bretts
                                    if (board[ni][nj].type != ' ') { // Stein im Weg
                                        if (board[ni][nj].white != whiteTurn && board[ni][nj].type != ' ') { // Gegnerischer Stein
                                            p2 = board[ni][nj];
                                            board[i][j].type = ' ';
                                            board[ni][nj] = p;
                                            if (!isKingThreatened(board, whiteTurn)) {
                                                return true;
                                            }
                                            board[ni][nj] = p2;
                                            board[i][j] = p;
                                        }
                                        break; // Beende Schleife
                                    }
                                    board[i][j].type = ' ';
                                    board[ni][nj] = p;
                                    if (!isKingThreatened(board, whiteTurn)) {
                                        return true;
                                    }
                                    board[ni][nj].type = ' ';
                                    board[i][j] = p;
                                    ni += di;
                                    nj += dj;
                                }
                            }
                        }
                        break;

                    case 'K':
                        // Alle 8 möglichen Züge um den König herum
                        for (int di = -1; di <= 1; di++) {
                            for (int dj = -1; dj <= 1; dj++) {
                                if (di == 0 && dj == 0)
                                    continue; // Kein Zug auf die aktuelle Position
                                if (i + di < 0 || i + di >= BOARD_SIZE || j + dj < 0 ||
                                    j + dj >= BOARD_SIZE)
                                    continue; // Zug außerhalb des Bretts
                                if (board[i + di][j + dj].white == whiteTurn && board[i + di][j + dj].type != ' ')
                                    continue; // Zug auf eigenen Stein
                                p2 = board[i + di][j + dj];
                                board[i][j].type = ' ';
                                board[i + di][j + dj] = p;
                                if (!isKingThreatened(board, whiteTurn)) {
                                    return true;
                                }
                                board[i + di][j + dj] = p2;
                                board[i][j] = p;
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }
    return false;
}

int getPiecePrice(const char pieceType) {
    switch (pieceType) {
        case 'P':
            return PAWN_PRICE;
        case 'R':
            return ROOK_PRICE;
        case 'N':
            return KNIGHT_PRICE;
        case 'B':
            return BISHOP_PRICE;
        case 'Q':
            return QUEEN_PRICE;
        default:
            return 0;
    }
}

// This function associates each piece with
// from number
int charPieceToEnumPiece(char piece, bool white) {
    switch (piece) {
        case 'P':
            return white ? PAWN_W : PAWN_B;
        case 'N':
            return white ? KNIGHT_W : KNIGHT_B;
        case 'B':
            return white ? BISHOP_W : BISHOP_B;
        case 'R':
            return white ? ROOK_W : ROOK_B;
        case 'Q':
            return white ? QUEEN_W : QUEEN_B;
        case 'K':
            return white ? KING_W : KING_B;
        default:
            return -1;
    }
}

void copyBoard(Piece oldBoard[BOARD_SIZE][BOARD_SIZE], Piece newBoard[BOARD_SIZE][BOARD_SIZE]) {
    memcpy(newBoard, oldBoard, BOARD_SIZE * BOARD_SIZE * sizeof(Piece));
}

//sort Movearray acording to preEval for more efficient AlphaBeta Pruning
int moveComperator(const void *p, const void *q) {
    return ((Move *) q)->preEval - ((Move *) p)->preEval;
}

void sortMoves(int size, Move arr[size]) {
    qsort(arr, size, sizeof(Move), moveComperator);
}

//pops the Least Significant Bit off an Int and returns its Position
uint64_t popLSB(uint64_t *num) {
    uint64_t pos = 0;
    _BitScanForward64(&pos, *num);
    *num ^= 1ULL << (pos);
    return pos;
}


