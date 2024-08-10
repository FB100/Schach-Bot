#include "main.h"


int main() {

}


void uci_loop() {
    char input[2048];

    while (1) {
        if (!fgets(input, 2048, stdin)) {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "uci") == 0) {
            printf("id name FabiBot\n");
            printf("id author Fabian Böhm\n");
            printf("uciok\n");
        } else if (strcmp(input, "isready") == 0) {
            printf("readyok\n");
        } else if (strncmp(input, "position", 8) == 0) {
            //TODO set_position(input);
        } else if (strncmp(input, "go", 2) == 0) {
            //TODO search_position();
        } else if (strcmp(input, "quit") == 0) {
            break;
        }
    }
}