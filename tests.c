#include "tests.h"
#include "headers.h"
#include "util.h"

int runTests() {
    uint16_t testsFailed = 0;
    uint16_t overallTestsFailed = 0;
    const uint16_t numberTests = 4;

    //create the board
    Piece (*board)[] = (Piece(*)[]) createBoard(BOARD_SIZE);
    if (board == NULL) {
        fprintf(stderr, "Malloc of board failed");
        return EXIT_FAILURE;
    }

    printf("Running Tests for FEN Strings: \n");
    testsFailed = runAllFenStringTests(board);
    overallTestsFailed += testsFailed;
    printf("Tests failed: %i/4\n\n", testsFailed);


    printf("Overall Tests failed: %i/%i / Tests succeeded: %i/%i\n\n", overallTestsFailed, numberTests, numberTests - overallTestsFailed,
           numberTests);
    return EXIT_SUCCESS;
}

uint16_t testFenStrings(const char *fen, Piece board[8][8], bool whitPlays, int castlingRights) {
    bool testPassed = 0;
    fenToBoard(fen, board);
    char *newFen = boardToFEN(board, whitPlays, 0);

    testPassed = strcmp(fen,newFen);
    if(!testPassed){
        printf("Test FEN failed:");
        printf("Original FEN: %s\n", fen);
        printf("New FEN:      %s\n", newFen);
    }
    return !testPassed;
}


uint16_t runAllFenStringTests(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    uint16_t testsFailed = 0;

    testsFailed += testFenStrings("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -", board, true, 0);

    testsFailed += testFenStrings("8/8/8/8/8/8/8/8 b -", board, false, 0);

    testsFailed += testFenStrings("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -", board, true, 0);

    testsFailed += testFenStrings("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -", board, true, 0);


    return testsFailed;
}

