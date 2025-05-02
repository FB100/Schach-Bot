#ifndef VERSION2_STRUCTS_H
#define VERSION2_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

enum PIECES {
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W,
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B
};

//Figur auf a1 = 0, Figur auf b1 = 1, usw
typedef uint64_t Bitboard;

typedef struct {
    Bitboard pawn_W;
    Bitboard knight_W;
    Bitboard bishop_W;
    Bitboard rook_W;
    Bitboard queen_W;
    Bitboard king_W;
    Bitboard pawn_B;
    Bitboard knight_B;
    Bitboard bishop_B;
    Bitboard rook_B;
    Bitboard queen_B;
    Bitboard king_B;
    uint8_t turn;
    uint8_t castling;
    uint8_t epSquare; //En Passant Square
    uint8_t whiteKingSq;
    uint8_t blackKingSq;
    Bitboard occupancy;
    Bitboard occupancyWhite;
    Bitboard occupancyBlack;
    uint64_t hash; //Zobrist hash
    Bitboard attacksWhite; // The attack mask of the other side
    Bitboard attacksBlack; // The attack mask of the other side
} Board;

/**
 * | Bits  | Field     | Description                                     |
 * | ----- | --------- | ----------------------------------------------- |
 * | 0–5   | `to`      | Destination square (0–63)                       |
 * | 6–11  | `from`    | Origin square (0–63)                            |
 * | 12–14 | `promo`   | Promotion piece (0=none, 1=N, 2=B, 3=R, 4=Q)    |
 * | 15    | `capture` | Was a piece captured? (1/0)                     |
 * | 16    | `ep`      | Was this an en passant move?                    |
 * | 17    | `castle`  | Is this castling?                               |
 * | 18–19 | `type`    | Optional: Move type enum (quiet, capture, etc.) |
 * | 20–31 | (unused)  | Free for future (e.g. history, eval, hash)      |
 */

typedef uint32_t Move;

#define MOVE_FROM(m)    (((m) >> 6) & 0x3F)
#define MOVE_TO(m)      ((m) & 0x3F)
#define MOVE_PROMO(m)   (((m) >> 12) & 0x7)
#define MOVE_IS_CAPTURE(m) (((m) >> 15) & 1)
#define MOVE_IS_EP(m)   (((m) >> 16) & 1)
#define MOVE_IS_CASTLE(m) (((m) >> 17) & 1)

#define ENCODE_MOVE(from, to, promo, cap, ep, castle) \
    (((from & 0x3F) << 6) | (to & 0x3F) | \
     ((promo & 0x7) << 12) | ((cap & 1) << 15) | \
     ((ep & 1) << 16) | ((castle & 1) << 17))

enum PromoPiece {
    NO_PROMO = 0,
    PROMO_N = 1,
    PROMO_B = 2,
    PROMO_R = 3,
    PROMO_Q = 4
};


#endif //VERSION2_STRUCTS_H
