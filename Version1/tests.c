#include "tests.h"
#include "headers.h"
#include "util.h"
#include "ai_generation.h"

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


    printf("Overall:\nTests failed: %i/%i \nTests succeeded: %i/%i\n\n", overallTestsFailed, numberTests, numberTests - overallTestsFailed,
           numberTests);
    return EXIT_SUCCESS;
}

uint16_t testFenStrings(const char *fen, Piece board[8][8], bool whitePlays, int castlingRights) {
    bool testFailed;
    fenToBoard(fen, board);
    char *newFen = boardToFEN(board, whitePlays, castlingRights);

    testFailed = !strcmp(fen, newFen);
    if (testFailed) {
        printf("Test FEN failed:");
        printf("Original FEN: %s\n", fen);
        printf("New FEN:      %s\n", newFen);
    }
    return testFailed;
}


uint16_t runAllFenStringTests(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    uint16_t testsFailed = 0;

    testsFailed += testFenStrings("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -", board, true, 0);

    testsFailed += testFenStrings("8/8/8/8/8/8/8/8 b -", board, false, 0);

    testsFailed += testFenStrings("pppppppp/pppppppp/ppppKppp/pppppppp/pppppppp/pppppppp/PPPPPPPP/PPPPPPPP w -", board, true, 0);

    testsFailed += testFenStrings("r4k1r/pp3ppp/2p2n2/3pP3/8/1Pq1PN2/P3BPPP/RQ3RK1 w -", board, true, 0);


    return testsFailed;
}

uint16_t testMoveGeneration() {
    bool testFailed = 0;


    return testFailed;
}

uint16_t testPawnMoveGeneration(Piece board[BOARD_SIZE][BOARD_SIZE], const char *fen, Move rightMoves[100]) {
    bool testFailed = 0;
    Move moves[100] = {0};

    fenToBoard(fen, board);
    //moves = getPawnMoves(board,moves);

    return testFailed;
}

uint16_t runAllPawnMoveGenerationTests(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Move moves[100] = {0};
    uint16_t testsFailed = 0;


    testsFailed += testPawnMoveGeneration( board,"8/8/8/3pP3/8/8/8/8 w -", moves);

    return testsFailed;
}
