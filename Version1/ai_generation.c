#include "ai_generation.h"
#include "ai_evaluation.h"
#include "util.h"
#include "additional_functionality/precomputed_data.h"


int getPawnMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    int direction = whiteTurn ? -1 : 1;

    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];
    Piece p2;

    // Ein Feld vorwärts
    if (board[i + direction][j].type == ' ') {
        if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
            //Alle 4 Promotionen
            for (int k = 0; k < 3; k++) {
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (i + direction) + j;
                moves[index].special = 4 + k;
                moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                moves[index].preEval = getPiecePrice('Q') - getPiecePrice('P'); //TODO hier richtiges Piece nicht immer Queen
                index++;
            }
        } else {
            moves[index].from = 8 * i + j;
            moves[index].to = 8 * (i + direction) + j;
            moves[index].special = 0;
            moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
            moves[index].preEval =
                    getPositionModifierPawn(i + direction, j, whiteTurn, 0) - getPositionModifierPawn(i, j, whiteTurn, 0);
            index++;
        }
        // Zwei Felder vorwärts (nur möglich von Startposition)
        if ((whiteTurn && i == 6) || (!whiteTurn && i == 1)) {
            if (board[i + 2 * direction][j].type == ' ') {
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (i + 2 * direction) + j;
                moves[index].special = 0;
                moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                moves[index].preEval =
                        getPositionModifierPawn(i + 2 * direction, j, whiteTurn, 0) - getPositionModifierPawn(i, j, whiteTurn, 0);
                index++;
            }
        }
    }
    // Diagonal schlagen (nur möglich bei gegnerischem piece)
    if (j + 1 < BOARD_SIZE) {
        if (board[i + direction][j + 1].white != whiteTurn && board[i + direction][j + 1].type != ' ') {
            p2 = board[i + direction][j + 1];
            if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
                //Alle 4 Promotionen
                for (int k = 0; k < 3; k++) {
                    moves[index].from = 8 * i + j;
                    moves[index].to = 8 * (i + direction) + j + 1;
                    moves[index].special = 4 + k;
                    moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                    moves[index].preEval = getPiecePrice(p2.type) - getPiecePrice('P') + getPiecePrice('Q');
                    index++;
                }
            } else {
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (i + direction) + j + 1;
                moves[index].special = 0;
                moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                moves[index].preEval = getPiecePrice(p2.type) - getPiecePrice('P');
                moves[index].preEval +=
                        getPositionModifierPawn(i + direction, j + 1, whiteTurn, 0) - getPositionModifierPawn(i, j, whiteTurn, 0);
                index++;
            }
        }
    }
    if (j - 1 >= 0) {
        if (board[i + direction][j - 1].white != whiteTurn && board[i + direction][j - 1].type != ' ') {
            p2 = board[i + direction][j - 1];
            if (i + direction == BOARD_SIZE - 1 || i + direction == 0) {
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (i + direction) + j - 1;
                moves[index].special = 1;
                moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                moves[index].preEval = getPiecePrice(p2.type) - getPiecePrice('P') + getPiecePrice('Q');
                index++;
            } else {
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (i + direction) + j - 1;
                moves[index].special = 0;
                moves[index].type = whiteTurn ? PAWN_W : PAWN_B;
                moves[index].preEval = getPiecePrice(p2.type) - getPiecePrice('P');
                moves[index].preEval +=
                        getPositionModifierPawn(i + direction, j - 1, whiteTurn, 0) - getPositionModifierPawn(i, j, whiteTurn, 0);

                index++;
            }
        }
    }
    return index;
}

int getKnightMoves(Board bitBoardBoard, Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    int preEvalWhite = 0;
    unsigned long x;
    uint64_t friendlyOccupancy = whiteTurn ? bitBoardBoard.occupancyWhite : bitBoardBoard.occupancyBlack;
    uint64_t knightSquares = PRE_KNIGHT_MOVE[BOARD_SIZE * i + j] & ~friendlyOccupancy;
    while (knightSquares) {
        x = popLSB(&knightSquares);
        moves[index].from = BOARD_SIZE * i + j;
        moves[index].to = x;
        moves[index].type = whiteTurn ? KNIGHT_W : KNIGHT_B;
        moves[index].special = 0;
        preEvalWhite = getPositionModifierKnight(x / 8ULL, x % 8ULL, whiteTurn, 0) - getPositionModifierKnight(i, j, whiteTurn, 0);
        moves[index].preEval = whiteTurn ? preEvalWhite : -preEvalWhite;
        index++;
    }
    return index;
}

int getBishopMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];
    Piece p2;
    int preEvalWhite = 0;

    // Bewege den Läufer in alle 4 Richtungen, solange kein Stein im Weg ist
    for (int di = -1; di <= 1; di += 2) {
        for (int dj = -1; dj <= 1; dj += 2) {
            int ni = i + di;
            int nj = j + dj;
            while (ni >= 0 && ni < BOARD_SIZE && nj >= 0 && nj < BOARD_SIZE) { // Zug innerhalb des Bretts
                if (board[ni][nj].type != ' ') { // Stein im Weg
                    if (board[ni][nj].white != whiteTurn && board[ni][nj].type != ' ') { // Gegnerischer Stein
                        p2 = board[ni][nj];
                        moves[index].from = 8 * i + j;
                        moves[index].to = 8 * (ni) + nj;
                        moves[index].special = 0;
                        moves[index].type = whiteTurn ? BISHOP_W : BISHOP_B;
                        moves[index].preEval = getPiecePrice(p2.type);
                        preEvalWhite = getPositionModifierBishop(ni, nj, whiteTurn, 0) - getPositionModifierBishop(i, j, whiteTurn, 0);
                        moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;
                        index++;
                    }
                    break; // Beende Schleife
                }
                moves[index].from = 8 * i + j;
                moves[index].to = 8 * (ni) + nj;
                moves[index].special = 0;
                moves[index].type = whiteTurn ? BISHOP_W : BISHOP_B;
                preEvalWhite = getPositionModifierBishop(ni, nj, whiteTurn, 0) - getPositionModifierBishop(i, j, whiteTurn, 0);
                moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;
                index++;

                ni += di;
                nj += dj;
            }
        }
    }
    return index;
}

int getRookMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p = board[i][j];
    Piece p2;
    int preEvalWhite = 0;

    // Bewege den Turm in alle 4 Richtungen, solange kein Stein im Weg ist
    for (int di = -1; di <= 1; di += 2) {
        int ni = i + di;
        while (ni >= 0 && ni < BOARD_SIZE) { // Zug innerhalb des Bretts
            if (board[ni][j].type != ' ') { // Stein im Weg
                if (board[ni][j].white != whiteTurn && board[ni][j].type != ' ') { // Gegnerischer Stein
                    p2 = board[ni][j];
                    moves[index].from = 8 * i + j;
                    moves[index].to = 8 * (ni) + j;
                    moves[index].special = 0;
                    moves[index].type = whiteTurn ? ROOK_W : ROOK_B;
                    moves[index].preEval = getPiecePrice(p2.type);
                    preEvalWhite = getPositionModifierRook(ni, j, whiteTurn, 0) - getPositionModifierRook(i, j, whiteTurn, 0);
                    moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;
                    index++;
                }
                break; // Beende Schleife
            }
            moves[index].from = 8 * i + j;
            moves[index].to = 8 * (ni) + j;
            moves[index].special = 0;
            moves[index].type = whiteTurn ? ROOK_W : ROOK_B;
            preEvalWhite = getPositionModifierRook(ni, j, whiteTurn, 0) - getPositionModifierRook(i, j, whiteTurn, 0);
            moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;
            index++;
            ni += di;
        }
    }
    for (int dj = -1; dj <= 1; dj += 2) {
        int nj = j + dj;
        while (nj >= 0 && nj < BOARD_SIZE) { // Zug innerhalb des Bretts
            if (board[i][nj].type != ' ') { // Stein im Weg
                if (board[i][nj].white != whiteTurn && board[i][nj].type != ' ') { // Gegnerischer Stein
                    p2 = board[i][nj];

                    moves[index].from = 8 * i + j;
                    moves[index].to = 8 * i + nj;
                    moves[index].special = 0;
                    moves[index].type = whiteTurn ? ROOK_W : ROOK_B;
                    moves[index].preEval = getPiecePrice(p2.type);
                    preEvalWhite = getPositionModifierRook(i, nj, whiteTurn, 0) - getPositionModifierRook(i, j, whiteTurn, 0);
                    moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;

                    index++;
                }
                break; // Beende Schleife
            }
            moves[index].from = 8 * i + j;
            moves[index].to = 8 * i + nj;
            moves[index].special = 0;
            moves[index].type = whiteTurn ? ROOK_W : ROOK_B;
            moves[index].preEval = 0;
            preEvalWhite = getPositionModifierRook(i, nj, whiteTurn, 0) - getPositionModifierRook(i, j, whiteTurn, 0);
            moves[index].preEval += whiteTurn ? preEvalWhite : -preEvalWhite;            index++;
            nj += dj;
        }
    }
    return index;
}

int getQueenMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    int index_old = index;
    index = getRookMoves(board, moves, whiteTurn, i, j, index);
    index = getBishopMoves(board, moves, whiteTurn, i, j, index);
    for (int k = index_old; k < index; k++) {
        moves[k].type = whiteTurn ? QUEEN_W : QUEEN_B;
    }
    return index;
}

int getKingMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int i, int j, int index) {
    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p2;

    // Alle 8 möglichen Züge um den König herum
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) { continue; } // Kein Zug auf die aktuelle Position
            if (i + di < 0 || i + di >= BOARD_SIZE || j + dj < 0 || j + dj >= BOARD_SIZE) { continue; }  // Zug außerhalb des Bretts
            if (board[i + di][j + dj].white == whiteTurn && board[i + di][j + dj].type != ' ') { continue; } // Zug auf eigenen Stein
            p2 = board[i + di][j + dj];
            moves[index].from = 8 * i + j;
            moves[index].to = 8 * (i + di) + j + dj;
            moves[index].special = 0;
            moves[index].type = whiteTurn ? KNIGHT_W : KNIGHT_B;
            moves[index].preEval = getPiecePrice(p2.type);
            index++;
        }
    }
    return index;
}

int getAllPseudoMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Board bitBoardBoard, Move moves[MAX_MOVE_ARRAY_SIZE], bool whiteTurn, int castlingRights) {

    int index = 0; //Index für moves Array


    // wer darf rochieren
    // white king
    if (castlingRights % 6 && (board[BOARD_SIZE - 1][4].type != 'K' || !board[BOARD_SIZE - 1][4].white)) {
        castlingRights *= 6;
    }
    // white from = 2
    if (castlingRights % 2 && (board[BOARD_SIZE - 1][0].type != 'R' || !board[BOARD_SIZE - 1][0].white)) {
        castlingRights *= 2;
    }
    // white h = 3
    if (castlingRights % 3 && (board[BOARD_SIZE - 1][BOARD_SIZE - 1].type != 'R' || !board[BOARD_SIZE - 1][BOARD_SIZE - 1].white)) {
        castlingRights *= 3;
    }
    // black king
    if (castlingRights % 35 && (board[0][4].type != 'K' || board[0][4].white)) {
        castlingRights *= 35;
    }
    // black from = 5
    if (castlingRights % 5 && (board[0][0].type != 'R' || board[0][0].white)) {
        castlingRights *= 5;
    }
    // black h = 7
    if (castlingRights % 7 && (board[0][BOARD_SIZE - 1].type != 'R' || board[0][BOARD_SIZE - 1].white)) {
        castlingRights *= 7;
    }

    // lange Rochade
    int whiteSize = (7 * whiteTurn);
    if (((castlingRights % 2 && whiteTurn) || (castlingRights % 5 && !whiteTurn)) && board[whiteSize][1].type == ' ' &&
        board[whiteSize][2].type == ' ' && board[whiteSize][3].type == ' ') {
        if (!isKingThreatened(board, whiteTurn)) {
            moves[index].from = 8 * whiteSize + 4;
            moves[index].to = 8 * whiteSize + 2;
            moves[index].special = 3;
            moves[index].type = whiteTurn ? KING_W : KING_B;
            moves[index].preEval = 0;
            index++;
        }
    }
    // kurze Rochade
    if (((castlingRights % 3 && whiteTurn) || (castlingRights % 7 && !whiteTurn)) && board[whiteSize][5].type == ' ' &&
        board[whiteSize][6].type == ' ') {
        if (!isKingThreatened(board, whiteTurn)) {
            moves[index].from = 8 * whiteSize + 4;
            moves[index].to = 8 * whiteSize + 6;
            moves[index].special = 2;
            moves[index].type = whiteTurn ? KING_W : KING_B;
            moves[index].preEval = 0;
            index++;
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].white == whiteTurn) {
                switch (board[i][j].type) {
                    case 'P':
                        index = getPawnMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    case 'N':
                        index = getKnightMoves(bitBoardBoard, moves, whiteTurn, i, j, index);
                        break;

                    case 'B':
                        index = getBishopMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    case 'R':
                        index = getRookMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    case 'Q':
                        index = getQueenMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    case 'K':
                        index = getKingMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    default:
                        break;
                }
            }
        }
    }
    if(index == 0){
        return index;
    }
    moves = realloc(moves, index * sizeof(Move));
    sortMoves(index - 1, moves);
    return index;
}