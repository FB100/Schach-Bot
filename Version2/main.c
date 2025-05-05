#include "main.h"

void uci_loop();
void uci_play_move(Move move);

Board *board;

int main() {
    board = malloc(sizeof(Board));
    if (!board) {
        fprintf(stderr, "Speicher konnte nicht allokiert werden\n");
        return 1;
    }
    uci_loop();
    free(board);
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
            printf("id author Fabian Boehm\n");
            printf("uciok\n");
            fflush(stdout);
        } else if (strcmp(input, "isready") == 0) {
            printf("readyok\n");
            fflush(stdout);
        } else if (strncmp(input, "position", 8) == 0) {
            set_position(board, input);
        } else if (strncmp(input, "go", 2) == 0) {
            uci_play_move(get_best_move(board, 4));
            fflush(stdout);
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "test") == 0) {
            int failed = test_main();
            if (failed) {
                printf("Tests fehlgeschlagen.\n");
            }
            fflush(stdout);
        } else if (strcmp(input, "pregen") == 0) {
            pregenerate_everything();
            fflush(stdout);
        }

    }
}

void uci_play_move(Move move) {
    char from_str[3], to_str[3];
    square_to_string(MOVE_FROM(move), from_str);
    square_to_string(MOVE_TO(move), to_str);

    printf("bestmove %s%s", from_str, to_str);

    int promo = MOVE_PROMO(move);
    if (promo != 0) {
        char pchar = promo_to_char(promo);
        if (pchar != '\0') {
            putchar(pchar);
        }
    }

    putchar('\n'); // End the UCI command with newline
}


