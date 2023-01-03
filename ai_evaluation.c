#include "ai_evaluation.h"
#include "ai_generation.h"
#include "util.h"


int getPositionModifier(Piece board[BOARD_SIZE][BOARD_SIZE], int round) {
    int positionModifier = 0;

    int modifierPawn[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                            500, 500, 500, 500, 500, 500, 500, 500,
                            100, 100, 200, 300, 300, 200, 100, 100,
                            50, 50, 100, 250, 250, 100, 50, 50,
                            0, 0, 0, 200, 200, 0, 0, 0,
                            50, -50, -100, 0, 0, -100, -50, 50,
                            50, 100, 100, -200, -200, 100, 100, 50,
                            0, 0, 0, 0, 0, 0, 0, 0};

    int modifierKnight[64] = {-500, -400, -300, -300, -300, -300, -400, -500,
                              -400, -200, 0, 0, 0, 0, -200, -400,
                              -300, 0, 100, 150, 150, 100, 0, -300,
                              -300, 50, 150, 200, 200, 150, 50, -300,
                              -300, 0, 150, 200, 200, 150, 0, -300,
                              -300, 50, 100, 150, 150, 100, 50, -300,
                              -400, -200, 0, 50, 50, 0, -200, -400,
                              -500, -400, -300, -300, -300, -300, -400, -500};

    int modifierBishop[64] = {-200, -100, -100, -100, -100, -100, -100, -200,
                              -100, 0, 0, 0, 0, 0, 0, -100,
                              -100, 0, 50, 100, 100, 50, 0, -100,
                              -100, 50, 50, 100, 100, 50, 50, -100,
                              -100, 0, 100, 100, 100, 100, 0, -100,
                              -100, 100, 100, 100, 100, 100, 100, -100,
                              -100, 50, 0, 0, 0, 0, 50, -100,
                              -200, -100, -100, -100, -100, -100, -100, -200};

    int modifierRook[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                            50, 100, 100, 100, 100, 100, 100, 50,
                            -50, 0, 0, 0, 0, 0, 0, -50,
                            -50, 0, 0, 0, 0, 0, 0, -50,
                            -50, 0, 0, 0, 0, 0, 0, -50,
                            -50, 0, 0, 0, 0, 0, 0, -50,
                            -50, 0, 0, 0, 0, 0, 0, -50,
                            0, 0, 0, 50, 50, 0, 0, 0};

    int modifierQueen[64] = {-200, -100, -100, -50, -50, -100, -100, -200,
                             -100, 0, 0, 0, 0, 0, 0, -100,
                             -100, 0, 50, 50, 50, 50, 0, -100,
                             -50, 0, 50, 50, 50, 50, 0, -50,
                             0, 0, 50, 50, 50, 50, 0, -50,
                             -100, 50, 50, 50, 50, 50, 0, -100,
                             -100, 0, 50, 0, 0, 0, 0, -100,
                             -200, -100, -100, -50, -50, -100, -100, -200};

    int modifierKingMid[64] = {-300, -400, -400, -500, -500, -400, -400, -300,
                               -300, -400, -400, -500, -500, -400, -400, -300,
                               -300, -400, -400, -500, -500, -400, -400, -300,
                               -300, -400, -400, -500, -500, -400, -400, -300,
                               -200, -300, -300, -400, -400, -300, -300, -200,
                               -100, -200, -200, -200, -200, -200, -200, -100,
                               200, 200, 0, 0, 0, 0, 200, 200,
                               200, 300, 100, 0, 0, 100, 300, 200};

    int modifierKingEnd[64] = {-500, -400, -300, -200, -200, -300, -400, -500,
                               -300, -200, -100, 0, 0, -100, -200, -300,
                               -300, -100, 200, 300, 300, 200, -100, -300,
                               -300, -100, 300, 400, 400, 300, -100, -300,
                               -300, -100, 300, 400, 400, 300, -100, -300,
                               -300, -100, 200, 300, 300, 200, -100, -300,
                               -300, -300, 0, 0, 0, 0, -300, -300,
                               -500, -300, -300, -300, -300, -300, -300, -500};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].type != ' ') {
                switch (board[i][j].type) {
                    case 'P':
                        if (board[i][j].white) {
                            positionModifier += modifierPawn[(i * BOARD_SIZE) + j];
                        } else {
                            positionModifier -= modifierPawn[(63 - (i * BOARD_SIZE) + j)];
                        }
                        break;
                    case 'N':
                        if (board[i][j].white) {
                            positionModifier += modifierKnight[(i * BOARD_SIZE) + j];
                        } else {
                            positionModifier -= modifierKnight[(63 - (i * BOARD_SIZE) + j)];
                        }
                        break;
                    case 'B':
                        if (board[i][j].white) {
                            positionModifier += modifierBishop[(i * BOARD_SIZE) + j];
                        } else {
                            positionModifier -= modifierBishop[(63 - (i * BOARD_SIZE) + j)];
                        }
                        break;
                    case 'R':
                        if (board[i][j].white) {
                            positionModifier += modifierRook[(i * BOARD_SIZE) + j];
                        } else {
                            positionModifier -= modifierRook[(63 - (i * BOARD_SIZE) + j)];
                        }
                        break;
                    case 'Q':
                        if (board[i][j].white) {
                            positionModifier += modifierQueen[(i * BOARD_SIZE) + j];
                        } else {
                            positionModifier -= modifierQueen[(63 - (i * BOARD_SIZE) + j)];
                        }
                        break;

                    case 'K':
                        if (round < 30) {
                            if (board[i][j].white) {
                                positionModifier += modifierKingMid[(i * BOARD_SIZE) + j];
                            } else {
                                positionModifier -= modifierKingMid[(63 - (i * BOARD_SIZE) + j)];
                            }
                        } else {
                            if (board[i][j].white) {
                                positionModifier += modifierKingEnd[(i * BOARD_SIZE) + j];
                            } else {
                                positionModifier -= modifierKingEnd[(63 - (i * BOARD_SIZE) + j)];
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return positionModifier;
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
                    counterWhite += piece.white ? 100 : -100;
                    break;
                case 'R':
                    counterWhite += piece.white ? 563 : -563;
                    break;
                case 'N':
                    counterWhite += piece.white ? 305 : -305;
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
                    counterWhite += piece.white ? 333 : -333;
                    break;
                case 'Q':
                    counterWhite += piece.white ? 950 : -950;
                    break;
                case 'K':
                    counterWhite += piece.white ? 100000 : -100000;
                    break;
                default:
                    break;
            }
        }
    }
    counterWhite += getPositionModifier(board, round);
    return counterWhite;
}


void evaluateAllCaptures(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
// TODO am Ende vor der Evaluation über diese Methode nur ruhige Positionen bewerten
}

void getAllMoves(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int castlingRights) {

    int index = 0; //Index für moves Array

    // 2 Pieces, damit ich die nicht immer neu initialisieren muss. P1 ist das aktuelle Piece. P2 ein eventuell geschlagenes
    Piece p;
    Piece p2;

    // wer darf rochieren
    // white king
    if (castlingRights % 6 && (board[BOARD_SIZE - 1][4].type != 'K' || !board[BOARD_SIZE - 1][4].white)) {
        castlingRights *= 6;
    }
    // white a = 2
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
    // black a = 5
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
        board[whiteSize][0].type = ' ';
        board[whiteSize][2].type = 'K';
        board[whiteSize][2].white = whiteTurn;
        board[whiteSize][3].type = 'R';
        board[whiteSize][3].white = whiteTurn;
        board[whiteSize][4].type = ' ';
        addBoardToArray(board, moves, index);
        index++;
        board[whiteSize][0].type = 'R';
        board[whiteSize][0].white = whiteTurn;
        board[whiteSize][2].type = ' ';
        board[whiteSize][3].type = ' ';
        board[whiteSize][4].type = 'K';
        board[whiteSize][4].white = whiteTurn;
    }
    // kurze Rochade
    if (((castlingRights % 3 && whiteTurn) || (castlingRights % 7 && !whiteTurn)) && board[whiteSize][5].type == ' ' &&
        board[whiteSize][6].type == ' ') {
        board[whiteSize][4].type = ' ';
        board[whiteSize][5].type = 'R';
        board[whiteSize][5].white = whiteTurn;
        board[whiteSize][6].type = 'K';
        board[whiteSize][6].white = whiteTurn;
        board[whiteSize][7].type = ' ';
        addBoardToArray(board, moves, index);
        index++;
        board[whiteSize][4].type = 'K';
        board[whiteSize][4].white = whiteTurn;
        board[whiteSize][5].type = ' ';
        board[whiteSize][6].type = ' ';
        board[whiteSize][7].type = 'R';
        board[whiteSize][7].white = whiteTurn;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            p = board[i][j];

            if (p.white == whiteTurn) {
                switch (p.type) {
                    case 'P':
                        index = getPawnMoves(board, moves, whiteTurn, i, j, index);
                        break;

                    case 'N':
                        index = getKnightMoves(board, moves, whiteTurn, i, j, index);
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
}


// TODO Stellungswiederholung vermeiden, wenn vorne
// TODO Move ordering
int findMovesAndEvaluate(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool initialCall, int remainingDepth, int alpha, int beta,
                         int castlingRights, int round) {
    int evaluation;

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
    if (maxBoard == NULL) {
        fprintf(stderr, "Malloc of maxBoard failed");
    }
    copyBoard(board, maxBoard);
    Piece (*moveArray)[BOARD_SIZE][BOARD_SIZE] = calloc(BOARD_SIZE * BOARD_SIZE * 100, sizeof(Piece));
    getAllMoves(board, moveArray, whiteTurn, castlingRights);

    // Checkmate or Stalemate
    if (moveArray[0][0][0].type == 0) {
        if (isKingThreatened(board, whiteTurn)) {
            free(maxBoard);
            free(moveArray);
            return -MAX_ALPHPA_BETA;
        } else {
            free(maxBoard);
            free(moveArray);
            return 0;
        }
    }
    // Iterate over all moves and evaluate
    for (int i = 0; i < 100; ++i) {
        if (moveArray[i][0][0].type == 0) {
            break;
        }
        evaluation = -findMovesAndEvaluate(moveArray[i], 1 - whiteTurn, false, remainingDepth - 1, -beta, -alpha, castlingRights, round);
        if (evaluation >= beta) {

            if (initialCall) {
                copyBoard(moveArray[i], board);
            }
            free(moveArray);
            free(maxBoard);
            return beta;
        } else if (evaluation > alpha) {
            alpha = evaluation;
            copyBoard(moveArray[i], maxBoard);
        }
    }
    free(moveArray);

    if (initialCall) {
        copyBoard(maxBoard, board);
    }
    free(maxBoard);
    return alpha;
}