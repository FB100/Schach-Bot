#include "init.h"

void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    A  B  C  D  E  F  G  H\n");
    printf("  * * * * * * * * * * * * * *\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d * ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            char c = board[i][j].type;
            if (c == ' ') {
                printf("   ");
            } else {
                if (board[i][j].white) {
                    printf("%c  ", c);
                } else {
                    printf("%c  ", tolower(c));
                }
            }
        }
        printf("* %d\n", BOARD_SIZE - i);
    }
    printf("  * * * * * * * * * * * * * *\n");
    printf("    A  B  C  D  E  F  G  H\n");

}

// Funktion, die einen FEN-String in das interne Schachbrett-Format umwandelt
void fenToBoard(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Alle Felder leer machen
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].type = ' ';
            board[i][j].white = 0;
        }
    }

    // Zeiger auf den aktuellen Zeichen im FEN-String
    const char *ptr = fen;

    // Reihe durchlaufen
    for (int i = 0; i < BOARD_SIZE; i++) {
        int j = 0;
        // Zeichen im FEN-String durchlaufen
        while (*ptr != '\0' && *ptr != '/') {
            // Zahl, die leere Felder anzeigt
            if (*ptr >= '1' && *ptr <= '8') {
                int numEmpty = *ptr - '0';
                j += numEmpty;
            }
                // Schachstein
            else {
                board[i][j].type = toupper(*ptr);
                board[i][j].white = isupper(*ptr);
                j++;
            }
            ptr++;
        }
        if (*ptr == '/') {
            ptr++;
        }
    }
}