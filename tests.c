#include "tests.h"
#include "headers.h"
#include "util.h"

int runTests(){

    //create the board
    Piece (*board)[] =  (Piece(*)[]) createBoard(BOARD_SIZE);
    if (board == NULL) {
        fprintf(stderr, "Malloc of board failed");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
