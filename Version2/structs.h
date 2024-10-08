#ifndef VERSION2_STRUCTS_H
#define VERSION2_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

enum PIECES {
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W,
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B
};

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
    int turn;
    int castling;
    int epSquare; //En Passant Square
    u_short whiteKingSq;
    u_short blackKingSq;
    Bitboard occupancy;
    Bitboard occupancyWhite;
    Bitboard occupancyBlack;
    uint64_t hash; //Zobrist hash
    Bitboard attacks; // The attack mask of the other side
} Board;



#endif //VERSION2_STRUCTS_H
