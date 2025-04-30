#ifndef VERSION2_MOVES_EXTERNAL_H
#define VERSION2_MOVES_EXTERNAL_H

#include <ctype.h>

#include "stdbool.h"
#include "structs.h"

void apply_move_string(Board *b, char *moves);

void set_bit_by_char(Board *board, char c, int rank, int file);

int square_from_rank_and_file(int rank, int file);

#endif //VERSION2_MOVES_EXTERNAL_H
