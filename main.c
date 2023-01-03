#include "main.h"
#include "init.h"
#include "ai_evaluation.h"
#include "util.h"


int main(int argc, char **argv) {
    return handleArguments(argc, argv);
}

int handleArguments(int argc, char **argv) {
    Piece *board = malloc(sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);
    if (board == NULL) {
        fprintf(stderr, "Malloc of board failed");
    }

    bool white = 1;
    bool aiOnly = 0;
    //const char *programName = argv[0];
    char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    int opt;
    int option_index = 0;

    static struct option longOptions[] = {{"help", no_argument, 0, 'h'}};

    while ((opt = getopt_long(argc, argv, "o:b:f:h", longOptions, &option_index)) != -1) {
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
            case 'h':
                printHelp();
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "Invalid Arguments");
                printHelp();
                return EXIT_FAILURE;
        }
    }
    printf("Begin Game with Parameters: \n");
    printf("-FEN: %s\n", fen);
    printf("-white: %s\n", white ? "true" : "false");
    printf("-AI only: %s\n", aiOnly ? "true" : "false");
    printf("\n");
    fenToBoard(fen, board);
    return runGame(board, white, aiOnly);
}

void printHelp() {
    static const char *help_msg =
            "Optional arguments:\n"
            "  -b         Start as black (wird nicht berücksichtigt in AI-only mode)\n"
            "  -f <str>   Start with custom fen string\n"
            "  -a         Let AI only play\n"
            "  -h|--help  Show help message (this text)\n";

    fprintf(stderr, "\n%s", help_msg);
}


bool isGameEnded(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
    Piece (*tempBoard)[BOARD_SIZE] = malloc(sizeof(Piece) * BOARD_SIZE * BOARD_SIZE);
    // TODO remove (just for debug)
    copyBoard(board, tempBoard);
    if (!whiteTurn) {
        copyBoard(board, tempBoard);
        if (!hasLegalMove(tempBoard, false)) {
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
        }
    }
    if (whiteTurn) {
        copyBoard(board, tempBoard);
        if (!hasLegalMove(tempBoard, true)) {
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
    }
    free(tempBoard);
    return (false);
}

void playHuman(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn) {
    //IO
    printf("Make a move:");
    printf("Where do you want to move from?\n");
    char moveFrom[10];
    if (scanf("%s", moveFrom) >= 10) {
        fprintf(stderr, "Invalid input: ");
        fprintf(stderr, "-%s-\n", moveFrom);
        return;
    }

    if (moveFrom[0] == 'c' && moveFrom[1] == 's') {
        int whiteSize = (7 * whiteTurn);
        board[whiteSize][4].type = ' ';
        board[whiteSize][5].type = 'R';
        board[whiteSize][5].white = whiteTurn;
        board[whiteSize][6].type = 'K';
        board[whiteSize][6].white = whiteTurn;
        board[whiteSize][7].type = ' ';

    } else if (moveFrom[0] == 'c' && moveFrom[1] == 'l') {
        int whiteSize = (7 * whiteTurn);
        board[whiteSize][0].type = ' ';
        board[whiteSize][1].type = ' ';
        board[whiteSize][2].type = 'K';
        board[whiteSize][2].white = whiteTurn;
        board[whiteSize][3].type = 'R';
        board[whiteSize][3].white = whiteTurn;
        board[whiteSize][4].type = ' ';
    } else {

        printf("Where do you want to move to?\n");
        char moveTo[10];
        if (scanf("%s", moveTo) >= 5) {
            fprintf(stderr, "Invalid input: ");
            fprintf(stderr, "-%s-\n", moveFrom);
            return;
        }

        //Translate IO to board
        char tempType = board[8 - (moveFrom[1] - 48)][moveFrom[0] - 97].type;
        bool tempWhite = board[8 - (moveFrom[1] - 48)][moveFrom[0] - 97].white;
        board[8 - (moveFrom[1] - 48)][moveFrom[0] - 97].type = ' ';
        board[8 - (moveTo[1] - 48)][moveTo[0] - 97].type = tempType;
        board[8 - (moveTo[1] - 48)][moveTo[0] - 97].white = tempWhite;
    }
    printf("The new Board looks like this: \n");
}

int playAI(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, int round) {
    int remainingDepth = 7;
    int evaluation = findMovesAndEvaluate(board, whiteTurn, true, remainingDepth, -MAX_ALPHPA_BETA, MAX_ALPHPA_BETA, 1, round);
    //TODO Fick game Endings

    return evaluation;
}

int runGame(Piece board[BOARD_SIZE][BOARD_SIZE], bool whiteTurn, bool aiOnly) {
    // AI vs AI Game
    if (aiOnly) {
        for (int i = 1; i < 200; i++) {
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return EXIT_SUCCESS;
            }
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            printf("AI evaluation: %d\n", playAI(board, whiteTurn, i));
            printBoard(board);
            whiteTurn = 1 - whiteTurn;
        }
        free(board);
        return EXIT_SUCCESS;
    } else {

        //AI vs Human game
        int i = 1;
        if (!whiteTurn) {
            printBoard(board);
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return EXIT_SUCCESS;
            }
            printf("Move: %d\n", 1);
            printf("Black to move\n");
            printf("AI evaluation: %d\n", playAI(board, whiteTurn, i));
            whiteTurn = 1 - whiteTurn;
            i++;
        }
        for (; i < 200; i += 2) {
            printBoard(board);
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return EXIT_SUCCESS;
            }
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            playHuman(board, whiteTurn);
            whiteTurn = 1 - whiteTurn;
            printBoard(board);
            if (isGameEnded(board, whiteTurn)) {
                free(board);
                return EXIT_SUCCESS;
            }
            printf("Move: %d\n", i);
            printf("%s to move\n", whiteTurn ? "White" : "Black");
            printf("AI evaluation: %d\n", playAI(board, whiteTurn, i));
            whiteTurn = 1 - whiteTurn;
        }

        free(board);
        return EXIT_SUCCESS;
    }
}
