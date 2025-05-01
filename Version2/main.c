#include "main.h"

void uci_loop();

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
            //TODO search_position();
            printf("TODO\n");
            fflush(stdout);
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "test") == 0) {
            int failed = test_main();
            if (failed) {
                printf("Tests fehlgeschlagen.\n");
            }
            fflush(stdout);
        }
    }
}


