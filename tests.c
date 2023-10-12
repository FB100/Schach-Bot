#include "tests.h"
#include "headers.h"
#include "util.h"

int runTests() {
    uint16_t testsFailed = 0;
    uint16_t overallTestsFailed = 0;
    const uint16_t numberTests = 0;

    //create the board
    Piece (*board)[] = (Piece(*)[]) createBoard(BOARD_SIZE);
    if (board == NULL) {
        fprintf(stderr, "Malloc of board failed");
        return EXIT_FAILURE;
    }

    printf("Running Tests for FEN Strings: \n");
    testsFailed = runAllFenStringTests(board);
    overallTestsFailed += testsFailed;
    printf("Tests failed: %i/0\n\n", testsFailed);


    printf("Overall Tests failed: %i/%i / Tests succeeded: %i/%i\n\n", overallTestsFailed, numberTests, numberTests - overallTestsFailed, numberTests);
    return EXIT_SUCCESS;
}

uint16_t testFenStrings(const char *fen, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    uint16_t testPassed = 0;
    fenToBoard(fen, board);

    //TODO board überprüfen

    return testPassed;
}


uint16_t runAllFenStringTests(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    uint16_t testsPassed = 0;

    testsPassed += testFenStrings("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", board);


    return testsPassed;
}

