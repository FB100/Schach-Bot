#ifndef VERSION2_SETUP_H
#define VERSION2_SETUP_H

#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "moves_external.h"
#include "generation.h"

void set_position(Board *board, const char *fen);


//For Testing
void position_input_to_fen(const char *input, char *fen_out, uint16_t max_len);
void position_input_to_moves(const char *input, char *moves_out, uint16_t max_len);
void set_position_from_FEN(Board *board, const char *fen);

#endif //VERSION2_SETUP_H
