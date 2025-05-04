#ifndef TEST_BOT_PY_UTIL_BOARD_H
#define TEST_BOT_PY_UTIL_BOARD_H


#include <ctype.h>
#include <stdbool.h>

#include "structs.h"


void square_to_string(uint8_t square, char *out);

int string_to_square(const char *string);

char promo_to_char(int promo);

int is_piece_on_square(Bitboard piece_bb, const char *square);

Bitboard square_to_bit(int square);

int move_exists(const Move *moves, int count, int from, int to, int capture);

uint8_t is_piece_white(enum PIECES piece);

#endif //TEST_BOT_PY_UTIL_BOARD_H
