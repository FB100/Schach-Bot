#ifndef VERSION2_GENERATION_H
#define VERSION2_GENERATION_H

#include "structs.h"
#include "pregenerate.h"
#include "util_board.h"

int generate_moves(Board *board, Move *legal_moves);

void make_move(Board *board, Move move);

//For Testing
void generate_knight_moves(Board *board, Move *moves, int *count);

void generate_pawn_moves(Board *board, Move *moves, int *count);

void generate_bishop_moves(Board *board, Move *moves, int *count);

void generate_rook_moves(Board *board, Move *moves, int *count);

void generate_queen_moves(Board *board, Move *moves, int *count);

void generate_king_moves(Board *board, Move *moves, int *count);

void update_attackmaps(Board *board);

#endif //VERSION2_GENERATION_H
