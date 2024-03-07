#include "ai_evaluation.h"
#include "ai_generation.h"
#include "util.h"
#include "repetition_table.h"
#include "zobrist_hashing.h"

int getPositionModifierPawn(int i, int j, bool white, int round) {
    const int modifierPawnMid[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            98, 134, 61, 95, 68, 126, 34, -11,
            -6, 7, 26, 31, 65, 56, 25, -20,
            -14, 13, 6, 21, 23, 12, 17, -23,
            -27, -2, -5, 12, 17, 6, 10, -25,
            -26, -4, -4, -10, 3, 3, 33, -12,
            -35, -1, -20, -23, -15, 24, 38, -22,
            0, 0, 0, 0, 0, 0, 0, 0,};

    const int modifierPawnEnd[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            178, 173, 158, 134, 147, 132, 165, 187,
            94, 100, 85, 67, 56, 53, 82, 84,
            32, 24, 13, 5, -2, 4, 17, 17,
            13, 9, -3, -7, -7, -8, 3, -1,
            4, 7, -6, 1, 0, -5, -1, -8,
            13, 8, 8, 10, 13, 0, 2, -7,
            0, 0, 0, 0, 0, 0, 0, 0,};

    if (round < 30) {
        if (white) {
            return modifierPawnMid[i * BOARD_SIZE + j];
        } else {
            return -modifierPawnMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierPawnEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierPawnEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

int getPositionModifierKnight(int i, int j, bool white, int round) {
    const int modifierKnightMid[64] = {
            -167, -89, -34, -49, 61, -97, -15, -107,
            -73, -41, 72, 36, 23, 62, 7, -17,
            -47, 60, 37, 65, 84, 129, 73, 44,
            -9, 17, 19, 53, 37, 69, 18, 22,
            -13, 4, 16, 13, 28, 19, 21, -8,
            -23, -9, 12, 10, 19, 17, 25, -16,
            -29, -53, -12, -3, -1, 18, -14, -19,
            -105, -21, -58, -33, -17, -28, -19, -23,};
    const int modifierKnightEnd[64] = {
            -58, -38, -13, -28, -31, -27, -63, -99,
            -25, -8, -25, -2, -9, -25, -24, -52,
            -24, -20, 10, 9, -1, -9, -19, -41,
            -17, 3, 22, 22, 22, 11, 8, -18,
            -18, -6, 16, 25, 16, 17, 4, -18,
            -23, -3, -1, 15, 10, -3, -20, -22,
            -42, -20, -10, -5, -2, -20, -23, -44,
            -29, -51, -23, -15, -22, -18, -50, -64,};

    if (round < 30) {
        if (white) {
            return modifierKnightMid[i * BOARD_SIZE + j];
        } else {
            return -modifierKnightMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierKnightEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierKnightEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

int getPositionModifierRook(int i, int j, bool white, int round) {
    const int modifierRookMid[64] = {
            32, 42, 32, 51, 63, 9, 31, 43,
            27, 32, 58, 62, 80, 67, 26, 44,
            -5, 19, 26, 36, 17, 45, 61, 16,
            -24, -11, 7, 26, 24, 35, -8, -20,
            -36, -26, -12, -1, 9, -7, 6, -23,
            -45, -25, -16, -17, 3, 0, -5, -33,
            -44, -16, -20, -9, -1, 11, -6, -71,
            -19, -13, 1, 17, 16, 7, -37, -26,};

    const int modifierRookEnd[64] = {
            13, 10, 18, 15, 12, 12, 8, 5,
            11, 13, 13, 11, -3, 3, 8, 3,
            7, 7, 7, 5, 4, -3, -5, -3,
            4, 3, 13, 1, 2, 1, -1, 2,
            3, 5, 8, 4, -5, -6, -8, -11,
            -4, 0, -5, -1, -7, -12, -8, -16,
            -6, -6, 0, 2, -9, -9, -11, -3,
            -9, 2, 3, -1, -5, -13, 4, -20,};

    if (round < 30) {
        if (white) {
            return modifierRookMid[i * BOARD_SIZE + j];
        } else {
            return -modifierRookMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierRookEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierRookEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

int getPositionModifierBishop(int i, int j, bool white, int round) {
    const int modifierBishopMid[64] = {
            -29, 4, -82, -37, -25, -42, 7, -8,
            -26, 16, -18, -13, 30, 59, 18, -47,
            -16, 37, 43, 40, 35, 50, 37, -2,
            -4, 5, 19, 50, 37, 37, 7, -2,
            -6, 13, 13, 26, 34, 12, 10, 4,
            0, 15, 15, 15, 14, 27, 18, 10,
            4, 15, 16, 0, 7, 21, 33, 1,
            -33, -3, -14, -21, -13, -12, -39, -21,};

    const int modifierBishopEnd[64] = {
            -14, -21, -11, -8, -7, -9, -17, -24,
            -8, -4, 7, -12, -3, -13, -4, -14,
            2, -8, 0, -1, -2, 6, 0, 4,
            -3, 9, 12, 9, 14, 10, 3, 2,
            -6, 3, 13, 19, 7, 10, -3, -9,
            -12, -3, 8, 10, 13, 3, -7, -15,
            -14, -18, -7, -1, 4, -9, -15, -27,
            -23, -9, -23, -5, -9, -16, -5, -17,};

    if (round < 30) {
        if (white) {
            return modifierBishopMid[i * BOARD_SIZE + j];
        } else {
            return -modifierBishopMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierBishopEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierBishopEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

int getPositionModifierQueen(int i, int j, bool white, int round) {
    const int modifierQueenMid[64] = {
            -28, 0, 29, 12, 59, 44, 43, 45,
            -24, -39, -5, 1, -16, 57, 28, 54,
            -13, -17, 7, 8, 29, 56, 47, 57,
            -27, -27, -16, -16, -1, 17, -2, 1,
            -9, -26, -9, -10, -2, -4, 3, -3,
            -14, 2, -11, -2, -5, 2, 14, 5,
            -35, -8, 11, 2, 8, 15, -3, 1,
            -1, -18, -9, 10, -15, -25, -31, -50,};

    const int modifierQueenEnd[64] = {
            -9, 22, 22, 27, 27, 19, 10, 20,
            -17, 20, 32, 41, 58, 25, 30, 0,
            -20, 6, 9, 49, 47, 35, 19, 9,
            3, 22, 24, 45, 57, 40, 57, 36,
            -18, 28, 19, 47, 31, 34, 39, 23,
            -16, -27, 15, 6, 9, 17, 10, 5,
            -22, -23, -30, -16, -16, -23, -36, -32,
            -33, -28, -22, -43, -5, -32, -20, -41,};

    if (round < 30) {
        if (white) {
            return modifierQueenMid[i * BOARD_SIZE + j];
        } else {
            return -modifierQueenMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierQueenEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierQueenEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

int getPositionModifierKing(int i, int j, bool white, int round) {
    const int modifierKingMid[64] = {
            -65, 23, 16, -15, -56, -34, 2, 13,
            29, -1, -20, -7, -8, -4, -38, -29,
            -9, 24, 2, -16, -20, 6, 22, -22,
            -17, -20, -12, -27, -30, -25, -14, -36,
            -49, -1, -27, -39, -46, -44, -33, -51,
            -14, -14, -22, -46, -44, -30, -15, -27,
            1, 7, -8, -64, -43, -16, 9, 8,
            -15, 36, 12, -54, 8, -28, 24, 14,};

    const int modifierKingEnd[64] = {
            -74, -35, -18, -18, -11, 15, 4, -17,
            -12, 17, 14, 17, 17, 38, 23, 11,
            10, 17, 23, 15, 20, 45, 44, 13,
            -8, 22, 24, 27, 26, 33, 26, 3,
            -18, -4, 21, 24, 27, 23, 9, -11,
            -19, -3, 11, 21, 23, 16, 7, -9,
            -27, -11, 4, 13, 14, 4, -5, -17,
            -53, -34, -21, -11, -28, -14, -24, -43};

    if (round < 30) {
        if (white) {
            return modifierKingMid[i * BOARD_SIZE + j];
        } else {
            return -modifierKingMid[63 - (i * BOARD_SIZE + j)];
        }
    } else {
        if (white) {
            return modifierKingEnd[i * BOARD_SIZE + j];
        } else {
            return -modifierKingEnd[63 - (i * BOARD_SIZE + j)];
        }
    }
}

//counts the number of pieces on the board, first place - white pieces, second - black pieces
int evaluateBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int round) {
    int counterWhite = 0;
    // Speichert, ob wir schon einen Läufer gesehen haben für den Läuferpaar Bonus
    bool bishopBlack = false;
    bool bishopWhite = false;
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            Piece piece = board[i][j];
            switch (piece.type) {
                case 'P':
                    counterWhite += piece.white ? PAWN_PRICE : -PAWN_PRICE;
                    counterWhite += getPositionModifierPawn(i, j, piece.white, round);
                    break;
                case 'N':
                    counterWhite += piece.white ? KNIGHT_PRICE : -KNIGHT_PRICE;
                    counterWhite += getPositionModifierKnight(i, j, piece.white, round);
                    break;
                case 'R':
                    counterWhite += piece.white ? ROOK_PRICE : -ROOK_PRICE;
                    counterWhite += getPositionModifierRook(i, j, piece.white, round);
                    break;
                case 'B':
                    //Läuferpaar bonus
                    if (piece.white) {
                        if (bishopWhite) {
                            counterWhite += 50;
                        }
                        bishopWhite = true;
                    }
                    if (!piece.white) {
                        if (bishopBlack) {
                            counterWhite -= 50;
                        }
                        bishopBlack = true;
                    }
                    counterWhite += piece.white ? BISHOP_PRICE : -BISHOP_PRICE;
                    counterWhite += getPositionModifierBishop(i, j, piece.white, round);
                    break;
                case 'Q':
                    counterWhite += piece.white ? QUEEN_PRICE : -QUEEN_PRICE;
                    counterWhite += getPositionModifierQueen(i, j, piece.white, round);
                    break;
                case 'K':
                    counterWhite += piece.white ? 100000 : -100000;
                    counterWhite += getPositionModifierKing(i, j, piece.white, round);
                    break;
                default:
                    break;
            }
        }
    }
    return counterWhite;
}


void evaluateAllCaptures(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[MAX_MOVE_ARRAY_SIZE][BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
// TODO am Ende vor der Evaluation über diese Methode nur ruhige Positionen bewerten
}


bool evaluateAndDoSingleMove(Piece board[BOARD_SIZE][BOARD_SIZE], Piece *tempBoard, Piece *maxBoard, Move *moveArray,
                             bool whiteTurn, bool initialCall, int remainingDepth, int *alpha, int beta, int castlingRights, int round) {
    int evaluation;

    //TODO Das ist ein placeholder bevor ich richtige Stellungswiederholung mache
    if (amountInRepetitionTable(computeHash(board)) > 1) {
        return 0;
    }

    if (isKingThreatened(board, whiteTurn)) {
        return false;
    }

    copyBoard(board, tempBoard);
    evaluation = 0;
    if (amountInRepetitionTable(computeHash(board)) < 2) {
        evaluation = -findMovesAndEvaluate(tempBoard, 1 - whiteTurn, false, remainingDepth - 1, -beta, -*alpha, castlingRights, round);
    }
    if (evaluation >= beta) {
        if (initialCall) {
            copyBoard(tempBoard, board);
        }
        free(moveArray);
        free(maxBoard);
        free(tempBoard);
        return true;
    } else if (evaluation > *alpha) {
        *alpha = evaluation;
        copyBoard(board, maxBoard);
    }
    return false;
}


// TODO Stellungswiederholung vermeiden, wenn vorne
// TODO Move ordering
int findMovesAndEvaluate(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool initialCall, int remainingDepth, int alpha, int beta,
                         int castlingRights, int round) {
    int evaluation;
    Piece p;
    Piece p2;

    // Ende der Rekursion
    if (!remainingDepth) {
        evaluation = evaluateBoard(board, round);
        if (whiteTurn) {
            return evaluation;
        }
        return -evaluation;
    }

    //
    Piece *maxBoard = malloc(sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);
    Piece *tempBoard = malloc(sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);
    if (maxBoard == NULL) {
        fprintf(stderr, "Malloc of maxBoard failed");
    }
    copyBoard(board, maxBoard);
    Move *moveArray = calloc(MAX_MOVE_ARRAY_SIZE, sizeof(Move));
    int moveArraySize = getAllPseudoMoves(board, moveArray, whiteTurn, castlingRights);

    // TODO: falsch das sind nur pseudo-legal moves => wenn leer problem
    // Checkmate or Stalemate
    if (moveArray[0].from == 0 && moveArray[0].to == 0) {
        if (isKingThreatened(board, whiteTurn)) {
            free(maxBoard);
            free(moveArray);
            free(tempBoard);
            return -MAX_ALPHPA_BETA;
        } else {
            free(maxBoard);
            free(moveArray);
            free(tempBoard);
            return 0;
        }
    }

    // Iterate over all moves and evaluate
    for (int i = 0; i < moveArraySize; i++) {
        Move move = moveArray[i];

        if (move.from == 0 && move.to == 0) {
            break;
        }

        int whiteSize = (7 * whiteTurn);

        makeMove(move, board);
        if (evaluateAndDoSingleMove(board, tempBoard, maxBoard, moveArray, whiteTurn, initialCall, remainingDepth, &alpha, beta,
                                    castlingRights, round)) {
            return beta;
        }
        unmakeMove(move, board);
    }

    free(moveArray);
    free(tempBoard);

    if (initialCall) {
        copyBoard(maxBoard, board);
    }
    free(maxBoard);
    return alpha;
}