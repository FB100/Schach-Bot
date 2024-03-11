#include "main.h"
#include "ai_evaluation.h"
#include "util.h"
#include "headers.h"
#include "tests.h"
#include "zobrist_hashing.h"
#include "repetition_table.h"
#include "additional_functionality/precompute_bitboards.h"


int main(int argc, char **argv) {
    return handleArgumentsAndInitialiseGame(argc, argv);
}


int handleArgumentsAndInitialiseGame(int argc, char **argv) {

    bool white = 1;
    bool aiOnly = 0;
    //const char *programName = argv[0];
    char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    int opt;
    int option_index = 0;

    static struct option longOptions[] = {
            {"help",   no_argument,       0, 'h'},
            {"test",   no_argument,       0, 't'},
            {"preComp",   no_argument,       0, 'p'},
            {"aionly", no_argument,       0, 'o'},
            {"fen",    required_argument, 0, 'f'}
    };

    while ((opt = getopt_long(argc, argv, "o:b:f:t:p:h", longOptions, &option_index)) != -1) {
        printf("opt: %c\n\n", opt);
        switch (opt) {
            case 'o':
                aiOnly = 1;
                break;
            case 'b':
                white = 0;
                break;
            case 'f':
                fen = optarg;
                break;
            case 't':
                runTests();
                return EXIT_SUCCESS;
            case 'p':
                precomputeKnightMoves();
                return EXIT_SUCCESS;
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "Invalid Arguments");
                printHelp();
                return EXIT_FAILURE;
        }
    }

    //create the board
    Piece (*board)[] = (Piece(*)[]) createBoard(BOARD_SIZE);
    if (board == NULL) {
        fprintf(stderr, "Malloc of board failed");
        return EXIT_FAILURE;
    }
    //initialise Game
    printf("Begin Game with Parameters: \n");
    printf("-FEN: %s\n", fen);
    printf("-white: %s\n", white ? "true" : "false");
    printf("-AI only: %s\n", aiOnly ? "true" : "false");
    printf("\n");
    fenToBoard(fen, board);
    initZobristTable();
    runGame(board, white, aiOnly);
    return EXIT_SUCCESS;
}

void printHelp() {
    static const char *help_msg =
            "Optional arguments:\n"
            "  -b         Start as black (wird nicht berücksichtigt in AI-only mode)\n"
            "  -f <str>   Start with custom fen string\n"
            "  -o         Let AI only play\n"
            "  -h|--help  Show help message (this text)\n";

    fprintf(stderr, "\n%s", help_msg);
}


bool isGameEnded(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
    if (amountInRepetitionTable(computeHash(board)) >= 2) {
        printf("-----------------------\n");
        printf("Remis durch Wiederholung\n");
        printf("-----------------------\n");
        return true;
    }
    Piece (*tempBoard)[BOARD_SIZE] = malloc(sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);
    // TODO remove (just for debug)
    copyBoard(board, tempBoard);
    if (!whiteTurn) {
        copyBoard(board, tempBoard);
        if (hasLegalMove(tempBoard, false)) {
            free(tempBoard);
            return (false);
        }
        if (isKingThreatened(tempBoard, false)) {
            printf("-----------------------\n");
            printf("   Weiß hat Gewonnen   \n");
            printf("-----------------------\n");
        } else {
            printf("------------------------\n");
            printf("Weiß stalemated Schwarz \n");
            printf("------------------------\n");
        }
        free(tempBoard);
        return (true);

    } else {
        copyBoard(board, tempBoard);
        if (hasLegalMove(tempBoard, true)) {
            free(tempBoard);
            return (false);
        }
        if (isKingThreatened(tempBoard, true)) {
            printf("------------------------\n");
            printf("  Schwarz hat Gewonnen  \n");
            printf("------------------------\n");
        } else {
            printf("------------------------\n");
            printf("Schwarz stalemated Weiß \n");
            printf("------------------------\n");
        }
        free(tempBoard);
        return true;

    }
    free(tempBoard);
    return (false);
}

void playHuman(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
    //IO
    printf("Make from move:");
    printf("Where do you want to move from?\n");
    char moveFrom[10];
    if (scanf("%s", moveFrom) >= 10) {
        fprintf(stderr, "Invalid input: ");
        fprintf(stderr, "-%s-\n", moveFrom);
        return;
    }
    Move move;

    if (moveFrom[0] == 'c' && moveFrom[1] == 's') {
        int whiteSize = (7 * whiteTurn);
        move.from = 8 * whiteSize + 4;
        move.to = 8 * whiteSize + 6;
        move.special = 2;

    } else if (moveFrom[0] == 'c' && moveFrom[1] == 'l') {
        int whiteSize = (7 * whiteTurn);
        move.from = 8 * whiteSize + 4;
        move.to = 8 * whiteSize + 2;
        move.special = 3;

    } else {
        printf("Where do you want to move to?\n");
        char moveTo[10];
        if (scanf("%s", moveTo) >= 5) {
            fprintf(stderr, "Invalid input: ");
            fprintf(stderr, "-%s-\n", moveFrom);
            return;
        }

        //Normal moves
        move.from = 8 * (8 - (moveFrom[1] - 48)) + (moveFrom[0] - 97);
        move.to = 8 * (8 - (moveTo[1] - 48)) + (moveTo[0] - 97);
        if (board[move.from / 8][move.from % 8].type == 'P' && (8 * (8 - (moveTo[1] - 48)) == BOARD_SIZE - 1 || 8 - (moveTo[1] - 48) == 0)) {
            move.special = 1; //Promotion
        }else{
            move.special = 0;
        }

    }
    makeMove(move, board);
    printf("The new Board looks like this: \n");
}

int playAI(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int round) {
    int remainingDepth = MAX_AI_DEPTH;
    int evaluation = -MAX_ALPHPA_BETA;
    while (evaluation == -MAX_ALPHPA_BETA) {
        evaluation = findMovesAndEvaluate(board, whiteTurn, true, remainingDepth, -MAX_ALPHPA_BETA, MAX_ALPHPA_BETA, 1, round);
        remainingDepth--;
        if (remainingDepth == 0) {
            break;
        }
    }
    //TODO Fick game Endings

    return evaluation;
}

void runGame(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool aiOnly) {

    // AI vs AI Game
    if (aiOnly) {
        for (int i = 1; i < MAX_MOVES; i++) {
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return;
            }
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            printf("AI evaluation: %d\n", playAI(board, whiteTurn, i));
            printBoard(board);
            whiteTurn = 1 - whiteTurn;
            pushRepetitionTable(computeHash(board));
        }
        free(board);
        return;
    } else {

        //AI vs Human game
        int i = 1;
        int evaluation = 0;
        if (!whiteTurn) {
            printBoard(board);
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return;
            }
            printf("Move: %d\n", 1);
            printf("Black to move\n");
            printf("AI evaluation: %d\n", playAI(board, whiteTurn, i));
            whiteTurn = 1 - whiteTurn;
            i++;
        }
        for (; i < MAX_MOVES; i += 2) {
            printBoard(board);
            pushRepetitionTable(computeHash(board));
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return;
            }
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            playHuman(board, whiteTurn);
            whiteTurn = 1 - whiteTurn;
            printBoard(board);
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return;
            }
            evaluation = playAI(board, whiteTurn, i);
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            printf("AI evaluation: %d\n", evaluation);
            whiteTurn = 1 - whiteTurn;
        }
        free(board);
        return;
    }
}
