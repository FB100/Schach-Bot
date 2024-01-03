#ifndef SCHACH_TESTS_H
#define SCHACH_TESTS_H

#include "headers.h"

int runTests();

uint16_t runAllFenStringTests(Piece board[BOARD_SIZE][BOARD_SIZE]);

uint16_t runAllPawnMoveGenerationTests(Piece board[BOARD_SIZE][BOARD_SIZE]);

#endif //SCHACH_TESTS_H
