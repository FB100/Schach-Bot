#include "ai_evaluation.h"
#include "ai_generation.h"
#include "util.h"
#include "repetition_table.h"
#include "zobrist_hashing.h"



int getPositionModifier(int i, int j, Piece piece, int round){
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

    int modifierBishop[64] = {-200, -100, -120, -120, -120, -120, -100, -200,
                              -100, 0, 0, 0, 0, 0, 0, -100,
                              -100, 0, 50, 100, 100, 50, 0, -100,
                              -100, 50, 50, 100, 100, 50, 50, -100,
                              -100, 0, 100, 100, 100, 100, 0, -100,
                              -100, 100, 100, 100, 100, 100, 100, -100,
                              -100, 50, 0, 0, 0, 0, 50, -100,
                              -200, -100, -120, -120, -120, -120, -100, -200};

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
                               200, 100, -100, -100, -100, -100, 100, 200,
                               200, 350, 100, 0, 0, 100, 350, 200};

    int modifierKingEnd[64] = {-500, -400, -300, -200, -200, -300, -400, -500,
                               -300, -200, -100, 0, 0, -100, -200, -300,
                               -300, -100, 200, 300, 300, 200, -100, -300,
                               -300, -100, 300, 400, 400, 300, -100, -300,
                               -300, -100, 300, 400, 400, 300, -100, -300,
                               -300, -100, 200, 300, 300, 200, -100, -300,
                               -300, -300, 0, 0, 0, 0, -300, -300,
                               -500, -300, -300, -300, -300, -300, -300, -500};

    switch (piece.type) {
        case 'P':
            if (piece.white){
                return modifierPawn[i*BOARD_SIZE+j];
            } else{
                return modifierPawn[63-(i*BOARD_SIZE+j)];
            }

        case 'N':
            if (piece.white){
                return modifierKnight[i*BOARD_SIZE+j];
            } else{
                return modifierKnight[63-(i*BOARD_SIZE+j)];
            }

        case 'B':
            if (piece.white){
                return modifierBishop[i*BOARD_SIZE+j];
            } else{
                return modifierBishop[63-(i*BOARD_SIZE+j)];
            }

        case 'R':
            if (piece.white){
                return modifierRook[i*BOARD_SIZE+j];
            } else{
                return modifierRook[63-(i*BOARD_SIZE+j)];
            }

        case 'Q':
            if (piece.white){
                return modifierQueen[i*BOARD_SIZE+j];
            } else{
                return modifierQueen[63-(i*BOARD_SIZE+j)];
            }


        case 'K':
            if (round < 30) {
                if (piece.white){
                    return modifierKingMid[i*BOARD_SIZE+j];
                } else{
                    return modifierKingMid[63-(i*BOARD_SIZE+j)];
                }
            } else {
                if (piece.white){
                    return modifierKingEnd[i*BOARD_SIZE+j];
                } else{
                    return modifierKingEnd[63-(i*BOARD_SIZE+j)];
                }
            }
        default:
            return 0;
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
                    break;
                case 'R':
                    counterWhite += piece.white ? ROOK_PRICE : -ROOK_PRICE;
                    break;
                case 'N':
                    counterWhite += piece.white ? KNIGHT_PRICE : -KNIGHT_PRICE;
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
                    break;
                case 'Q':
                    counterWhite += piece.white ? QUEEN_PRICE : -QUEEN_PRICE;
                    break;
                case 'K':
                    counterWhite += piece.white ? 100000 : -100000;
                    break;
                default:
                    break;
            }
            counterWhite += getPositionModifier(i,j,piece,round);
        }
    }
    return counterWhite;
}


void evaluateAllCaptures(Piece board[BOARD_SIZE][BOARD_SIZE], Piece moves[100][BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
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
    Move *moveArray = calloc(BOARD_SIZE * BOARD_SIZE * 100, sizeof(Piece));
    getAllPseudoMoves(board, moveArray, whiteTurn, castlingRights);

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
    for (int i = 0; i < 100; ++i) {
        Move move = moveArray[i];

        if (move.from == 0 && move.to == 0) {
            break;
        }

        int whiteSize = (7 * whiteTurn);

        switch (move.special) {
            case 0:
                //normal Moves
                p = board[move.from / 8][move.from % 8];
                p2 = board[move.to / 8][move.to % 8];
                board[move.from / 8][move.from % 8].type = ' ';
                board[move.to / 8][move.to % 8] = p;
                if (evaluateAndDoSingleMove(board, tempBoard, maxBoard, moveArray, whiteTurn, initialCall, remainingDepth, &alpha, beta,
                                            castlingRights, round)) {
                    return beta;
                }
                board[move.to / 8][move.to % 8] = p2;
                board[move.from / 8][move.from % 8] = p;
                break;
            case 1:
                // Promotion
                p = board[moveArray[i].from / 8][moveArray[i].from % 8];
                p2 = board[moveArray[i].to / 8][moveArray[i].to % 8];
                board[moveArray[i].from / 8][moveArray[i].from % 8].type = ' ';
                board[moveArray[i].to / 8][moveArray[i].to % 8] = p;
                board[moveArray[i].to / 8][moveArray[i].to % 8].type = 'Q';
                if (evaluateAndDoSingleMove(board, tempBoard, maxBoard, moveArray, whiteTurn, initialCall, remainingDepth, &alpha, beta,
                                            castlingRights, round)) {
                    return beta;
                }
                board[moveArray[i].to / 8][moveArray[i].to % 8] = p2;
                board[moveArray[i].from / 8][moveArray[i].from % 8] = p;
                break;
            case 2:
                // kurze Rochade
                board[whiteSize][4].type = ' ';
                board[whiteSize][5].type = 'R';
                board[whiteSize][5].white = whiteTurn;
                board[whiteSize][6].type = 'K';
                board[whiteSize][6].white = whiteTurn;
                board[whiteSize][7].type = ' ';
                if (evaluateAndDoSingleMove(board, tempBoard, maxBoard, moveArray, whiteTurn, initialCall, remainingDepth, &alpha, beta,
                                            castlingRights, round)) {
                    return beta;
                }
                board[whiteSize][4].type = 'K';
                board[whiteSize][4].white = whiteTurn;
                board[whiteSize][5].type = ' ';
                board[whiteSize][6].type = ' ';
                board[whiteSize][7].type = 'R';
                board[whiteSize][7].white = whiteTurn;
                break;
            case 3:
                // Lange Rochade
                board[whiteSize][0].type = ' ';
                board[whiteSize][2].type = 'K';
                board[whiteSize][2].white = whiteTurn;
                board[whiteSize][3].type = 'R';
                board[whiteSize][3].white = whiteTurn;
                board[whiteSize][4].type = ' ';
                if (evaluateAndDoSingleMove(board, tempBoard, maxBoard, moveArray, whiteTurn, initialCall, remainingDepth, &alpha, beta,
                                            castlingRights, round)) {
                    return beta;
                }
                board[whiteSize][0].type = 'R';
                board[whiteSize][0].white = whiteTurn;
                board[whiteSize][2].type = ' ';
                board[whiteSize][3].type = ' ';
                board[whiteSize][4].type = 'K';
                board[whiteSize][4].white = whiteTurn;
                break;
            default:
                break;
        }
    }

    free(moveArray);
    free(tempBoard);

    if (initialCall) {
        copyBoard(maxBoard, board);
    }
    free(maxBoard);
    return alpha;
}