#ifndef VERSION2_MOVES_EXTERNAL_H
#define VERSION2_MOVES_EXTERNAL_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "stdbool.h"
#include "structs.h"
#include "util_board.h"

void apply_move_string(Board *board, char *moves);

void set_bit_by_char(Board *board, char c, int rank, int file);

uint8_t square_from_rank_and_file(int rank, int file);

int string_to_square(const char *string);

#endif //VERSION2_MOVES_EXTERNAL_H
