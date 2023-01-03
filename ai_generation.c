#include "ai_generation.h"
#include "util.h"


int getPawnMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    int direction = whiteTurn ? -1 : 1;

    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

    // Ein Feld vorwärts
    if (board[i + direction][j].type == ' ') {
        board[i][j].type = ' ';
        board[i + direction][j] = p;
        if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
            board[i + direction][j].type = 'Q';
        }
        if (!isKingThreatened(board, whiteTurn)) {
            addBoardToArray(board, moves, index);
            index++;
        }
        board[i + direction][j].type = ' ';
        board[i][j] = p;
        // Zwei Felder vorwärts (nur möglich von Startposition)
        if ((whiteTurn && i == 6) || (!whiteTurn && i == 1)) {
            if (board[i + 2 * direction][j].type == ' ') {
                board[i][j].type = ' ';
                board[i + 2 * direction][j] = p;
                if (!isKingThreatened(board, whiteTurn)) {
                    addBoardToArray(board, moves, index);
                    index++;
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
                addBoardToArray(board, moves, index);
                index++;
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
                addBoardToArray(board, moves, index);
                index++;
            }
            board[i + direction][j - 1] = p2;
            board[i][j] = p;
        }
    }
    return index;
}

int getKnightMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

    //Relative Position der Felder, au die ein Night ziehen kann
    int8_t knightMoves[8][2] = {{2,  1},
                                {2,  -1},
                                {-2, 1},
                                {-2, -1},
                                {1,  2},
                                {1,  -2},
                                {-1, 2},
                                {-1, -2}};

    // Alle 8 möglichen Züge für einen Springer
    for (int k = 0; k < 8; k++) {
        int di = knightMoves[k][0];
        int dj = knightMoves[k][1];
        if (i + di < 0 || i + di >= BOARD_SIZE || j + dj < 0 || j + dj >= BOARD_SIZE)
            continue; // Zug außerhalb des Bretts
        if (board[i + di][j + dj].white == whiteTurn && board[i + di][j + dj].type != ' ') continue; // Zug auf eigenen Stein
        p2 = board[i + di][j + dj];
        board[i][j].type = ' ';
        board[i + di][j + dj] = p;
        if (!isKingThreatened(board, whiteTurn)) {
            addBoardToArray(board, moves, index);
            index++;
        }
        board[i + di][j + dj] = p2;
        board[i][j] = p;
    }
    return index;
}

int getBishopMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

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
                            addBoardToArray(board, moves, index);
                            index++;
                        }
                        board[ni][nj] = p2;
                        board[i][j] = p;
                    }
                    break; // Beende Schleife
                }
                board[i][j].type = ' ';
                board[ni][nj] = p;
                if (!isKingThreatened(board, whiteTurn)) {
                    addBoardToArray(board, moves, index);
                    index++;
                }
                board[ni][nj].type = ' ';
                board[i][j] = p;
                ni += di;
                nj += dj;
            }
        }
    }
    return index;
}

int getRookMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

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
                        addBoardToArray(board, moves, index);
                        index++;
                    }
                    board[ni][j] = p2;
                    board[i][j] = p;
                }
                break; // Beende Schleife
            }
            board[i][j].type = ' ';
            board[ni][j] = p;
            if (!isKingThreatened(board, whiteTurn)) {
                addBoardToArray(board, moves, index);
                index++;
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
                        addBoardToArray(board, moves, index);
                        index++;
                    }
                    board[i][nj] = p2;
                    board[i][j] = p;
                    nj += dj;
                }
                break; // Beende Schleife
            }
            board[i][j].type = ' ';
            board[i][nj] = p;
            if (!isKingThreatened(board, whiteTurn)) {
                addBoardToArray(board, moves, index);
                index++;
            }
            board[i][nj].type = ' ';
            board[i][j] = p;
            nj += dj;
        }
    }
    return index;
}

int getQueenMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

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
                            addBoardToArray(board, moves, index);
                            index++;
                        }
                        board[ni][nj] = p2;
                        board[i][j] = p;
                    }
                    break; // Beende Schleife
                }
                board[i][j].type = ' ';
                board[ni][nj] = p;
                if (!isKingThreatened(board, whiteTurn)) {
                    addBoardToArray(board, moves, index);
                    index++;
                }
                board[ni][nj].type = ' ';
                board[i][j] = p;
                ni += di;
                nj += dj;
            }
        }
    }
    return index;
}

int getKingMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];;
    Piece p2;

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
                addBoardToArray(board, moves, index);
                index++;
            }
            board[i + di][j + dj] = p2;
            board[i][j] = p;
        }
    }
    return index;
}