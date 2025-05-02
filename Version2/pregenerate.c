#include "pregenerate.h"


Bitboard knight_attacks_from(int sq) {
    Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // All 8 possible knight jumps
    int dr[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int df[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; i++) {
        int r = rank + dr[i];
        int f = file + df[i];
        if (r >= 0 && r < 8 && f >= 0 && f < 8) {
            int target_sq = r * 8 + f;
            attacks |= (1ULL << target_sq);
        }
    }

    return attacks;
}

void generate_knight_attack_table() {
    printf("Bitboard knight_attacks_from[64] = {\n");
    for (int sq = 0; sq < 64; sq++) {
        printf("%luULL, ", knight_attacks_from(sq));
    }
    printf("\n};\n\n");
}

void pregenerate_everything(){
    generate_knight_attack_table();
}
