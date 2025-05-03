#ifndef VERSION2_GENERATION_H
#define VERSION2_GENERATION_H

#include "structs.h"
#include "pregenerate.h"

int generate_moves(Board *board, Move *legal_moves);
void make_move(Board *board, Move move);

//For Testing
void generate_knight_moves(Board *board, Move *moves, int *count);

#endif //VERSION2_GENERATION_H
