#include "generation.h"


int generate_moves(Board *board, Move *legal_moves) {
    Move pseudo_moves[MAX_MOVES];
    int pseudo_count = 0;

    // Call piece-specific pseudo-legal generators
    generate_pawn_moves(board, pseudo_moves, &pseudo_count);
    generate_knight_moves(board, pseudo_moves, &pseudo_count);
    generate_bishop_moves(board, pseudo_moves, &pseudo_count);
    generate_rook_moves(board, pseudo_moves, &pseudo_count);
    generate_queen_moves(board, pseudo_moves, &pseudo_count);
    generate_king_moves(board, pseudo_moves, &pseudo_count);

    int legal_count = 0;

    // Filter pseudo-legal moves by checking legality (i.e., king not left in check)
    for (int i = 0; i < pseudo_count; i++) {
        Board temp = *board;
        make_move(&temp, pseudo_moves[i]);

        uint64_t is_king_in_check = temp.turn ? temp.king_W & temp.attacksBlack : temp.king_B & temp.attacksWhite;

        if (!is_king_in_check) {
            legal_moves[legal_count++] = pseudo_moves[i];
        }

        // No need to undo: temp was copied
    }

    return legal_count;
}

void make_move(Board *board, Move move) {
    uint8_t from = MOVE_FROM(move);
    uint8_t to = MOVE_TO(move);
    uint8_t is_capture = MOVE_IS_CAPTURE(move);
    uint8_t is_ep = MOVE_IS_EP(move);
    uint8_t is_castle = MOVE_IS_CASTLE(move);
    uint8_t promo = MOVE_PROMO(move);

    Bitboard from_bb = 1ULL << from;
    Bitboard to_bb = 1ULL << to;

    int is_white = board->turn == 0;

    // 1. Figur und Ziel identifizieren
    Bitboard *pieces[] = {is_white ? &board->pawn_W : &board->pawn_B, is_white ? &board->knight_W : &board->knight_B,
                          is_white ? &board->bishop_W : &board->bishop_B, is_white ? &board->rook_W : &board->rook_B,
                          is_white ? &board->queen_W : &board->queen_B, is_white ? &board->king_W : &board->king_B};

    int moved_piece = -1;
    for (int i = 0; i < 6; i++) {
        if (*pieces[i] & from_bb) {
            *pieces[i] &= ~from_bb;
            moved_piece = i;
            break;
        }
    }

    // 2. Gegnerische Figur schlagen (außer En Passant)
    if (is_capture && !is_ep) {
        Bitboard *targets[] = {is_white ? &board->pawn_B : &board->pawn_W, is_white ? &board->knight_B : &board->knight_W,
                               is_white ? &board->bishop_B : &board->bishop_W, is_white ? &board->rook_B : &board->rook_W,
                               is_white ? &board->queen_B : &board->queen_W, is_white ? &board->king_B : &board->king_W};

        for (int i = 0; i < 6; i++) {
            if (*targets[i] & to_bb) {
                *targets[i] &= ~to_bb;
                break;
            }
        }
    }

    // 3. En Passant Schlag
    if (is_ep) {
        int ep_pawn_sq = to + (is_white ? -8 : 8);
        Bitboard ep_bb = 1ULL << ep_pawn_sq;
        if (is_white)
            board->pawn_B &= ~ep_bb;
        else
            board->pawn_W &= ~ep_bb;
    }

    // 4. Rochade
    if (is_castle && moved_piece == 5 /* king */) {
        if (to == 6 || to == 62) { // short
            int rook_from = to + 1;
            int rook_to = to - 1;
            Bitboard rook = 1ULL << rook_from;
            Bitboard dest = 1ULL << rook_to;

            Bitboard *rook_bb = is_white ? &board->rook_W : &board->rook_B;
            *rook_bb &= ~rook;
            *rook_bb |= dest;
        } else if (to == 2 || to == 58) { // long
            int rook_from = to - 2;
            int rook_to = to + 1;
            Bitboard rook = 1ULL << rook_from;
            Bitboard dest = 1ULL << rook_to;

            Bitboard *rook_bb = is_white ? &board->rook_W : &board->rook_B;
            *rook_bb &= ~rook;
            *rook_bb |= dest;
        }
    }

    // 5. Umwandlung oder normaler Zug
    if (promo && moved_piece == 0 /* pawn */) {
        Bitboard *promo_bb = NULL;
        switch (promo) {
            case 1:
                promo_bb = is_white ? &board->knight_W : &board->knight_B;
                break;
            case 2:
                promo_bb = is_white ? &board->bishop_W : &board->bishop_B;
                break;
            case 3:
                promo_bb = is_white ? &board->rook_W : &board->rook_B;
                break;
            case 4:
                promo_bb = is_white ? &board->queen_W : &board->queen_B;
                break;
            default:
                break;
        }
        if (promo_bb) *promo_bb |= to_bb;
    } else {
        pieces[moved_piece][0] |= to_bb;
    }

    // 6. En Passant Ziel setzen
    board->epSquare = -1;
    if (moved_piece == 0 /* pawn */ && (to - from == 16 || from - to == 16)) {
        board->epSquare = is_white ? (to - 8) : (to + 8);
    }

    // 7. Rochaderechte aktualisieren
    // Weißer König oder weißer kurzer Turm gezogen/geschlagen
    if (!(board->king_W & (1ULL << 4)) || !(board->rook_W & (1ULL << 7)))
        board->castling &= ~0b0001; // Weiß kurz (K)

    // Weißer König oder weißer langer Turm gezogen/geschlagen
    if (!(board->king_W & (1ULL << 4)) || !(board->rook_W & (1ULL << 0)))
        board->castling &= ~0b0010; // Weiß lang (Q)

    // Schwarzer König oder schwarzer kurzer Turm gezogen/geschlagen
    if (!(board->king_B & (1ULL << 63)) || !(board->rook_B & (1ULL << 63)))
        board->castling &= ~0b0100; // Schwarz kurz (k)

    // Schwarzer König oder schwarzer langer Turm gezogen/geschlagen
    if (!(board->king_B & (1ULL << 63)) || !(board->rook_B & (1ULL << 56)))
        board->castling &= ~0b1000; // Schwarz lang (q)

    // 8. King-Position aktualisieren
    if (moved_piece == 5 /* king */) {
        if (is_white)
            board->whiteKingSq = to;
        else
            board->blackKingSq = to;
    }

    // 10. Spielerwechsel
    board->turn ^= 1;

    update_attackmaps(board);
    calculate_occupancy(board);
    // TODO Zobrist Hash
}

void generate_pawn_moves(Board *board, Move *moves, int *count) {
    Bitboard pawns;
    Bitboard empty_squares = ~board->occupancy;

    if (board->turn) {
        // Schwarze Bauern
        pawns = board->pawn_B;

        // 1. Einzelzüge nach vorne
        Bitboard single_push = (pawns >> 8) & empty_squares;
        Bitboard double_push = ((single_push & RANK_6) >> 8) & empty_squares;

        // Normale Einzelzüge + Promotion
        while (single_push) {
            int to = __builtin_ctzll(single_push);
            int from = to + 8;
            if ((1ULL << to) & RANK_1) {
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 0, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 0, 0, 0);
            }
            single_push &= single_push - 1;
        }

        // Doppelschritt
        Bitboard double_push_copy = double_push;
        while (double_push_copy) {
            int to = __builtin_ctzll(double_push_copy);
            int from = to + 16;
            moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 0, 0, 0);
            double_push_copy &= double_push_copy - 1;
        }

        // 2. Schlagzüge
        Bitboard left_attacks = (pawns & ~FILE_A) >> 9 & board->occupancyWhite;
        Bitboard right_attacks = (pawns & ~FILE_H) >> 7 & board->occupancyWhite;

        Bitboard left_copy = left_attacks;
        while (left_copy) {
            int to = __builtin_ctzll(left_copy);
            int from = to + 9;
            if ((1ULL << to) & RANK_1) {
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 1, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 1, 0, 0);
            }
            left_copy &= left_copy - 1;
        }

        Bitboard right_copy = right_attacks;
        while (right_copy) {
            int to = __builtin_ctzll(right_copy);
            int from = to + 7;
            if ((1ULL << to) & RANK_1) {
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 1, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 1, 0, 0);
            }
            right_copy &= right_copy - 1;
        }

        // En Passant
        if (board->epSquare != 0xFF) {
            Bitboard epSquareBB = 1ULL << board->epSquare;
            Bitboard epLeft = (pawns & ~FILE_A) >> 9;
            Bitboard epRight = (pawns & ~FILE_H) >> 7;
            if (epLeft & epSquareBB) {
                int from = board->epSquare + 9;
                moves[(*count)++] = ENCODE_MOVE(from, board->epSquare, NO_PROMO, 1, 1, 0);
            }
            if (epRight & epSquareBB) {
                int from = board->epSquare + 7;
                moves[(*count)++] = ENCODE_MOVE(from, board->epSquare, NO_PROMO, 1, 1, 0);
            }
        }
    } else {
        // Weiße Bauern
        pawns = board->pawn_W;

        // 1. Einzelzüge nach vorne
        Bitboard single_push = (pawns << 8) & empty_squares;
        Bitboard double_push = ((single_push & RANK_3) << 8) & empty_squares;

        // Normale Einzelzüge + Promotion
        while (single_push) {
            int to = __builtin_ctzll(single_push);
            int from = to - 8;
            if ((1ULL << to) & RANK_8) {
                // Promotion
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 0, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 0, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 0, 0, 0);
            }
            single_push &= single_push - 1;
        }

        // Doppelschritt
        Bitboard double_push_copy = double_push;
        while (double_push_copy) {
            int to = __builtin_ctzll(double_push_copy);
            int from = to - 16;
            moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 0, 0, 0);
            double_push_copy &= double_push_copy - 1;
        }

        // 2. Schlagzüge
        // Links oben
        Bitboard left_attacks = (pawns & ~FILE_A) << 7 & board->occupancyBlack;
        // Rechts oben
        Bitboard right_attacks = (pawns & ~FILE_H) << 9 & board->occupancyBlack;

        Bitboard left_copy = left_attacks;
        while (left_copy) {
            int to = __builtin_ctzll(left_copy);
            int from = to - 7;
            if ((1ULL << to) & RANK_8) {
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 1, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 1, 0, 0);
            }
            left_copy &= left_copy - 1;
        }

        Bitboard right_copy = right_attacks;
        while (right_copy) {
            int to = __builtin_ctzll(right_copy);
            int from = to - 9;
            if ((1ULL << to) & RANK_8) {
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_Q, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_R, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_B, 1, 0, 0);
                moves[(*count)++] = ENCODE_MOVE(from, to, PROMO_N, 1, 0, 0);
            } else {
                moves[(*count)++] = ENCODE_MOVE(from, to, NO_PROMO, 1, 0, 0);
            }
            right_copy &= right_copy - 1;
        }

        // En Passant
        if (board->epSquare != 0xFF) {
            Bitboard epSquareBB = 1ULL << board->epSquare;
            Bitboard epLeft = (pawns & ~FILE_A) << 7;
            Bitboard epRight = (pawns & ~FILE_H) << 9;
            if (epLeft & epSquareBB) {
                int from = board->epSquare - 7;
                moves[(*count)++] = ENCODE_MOVE(from, board->epSquare, NO_PROMO, 1, 1, 0);
            }
            if (epRight & epSquareBB) {
                int from = board->epSquare - 9;
                moves[(*count)++] = ENCODE_MOVE(from, board->epSquare, NO_PROMO, 1, 1, 0);
            }
        }
    }
}

void generate_knight_moves(Board *board, Move *moves, int *count) {
    Bitboard knights = board->turn ? board->knight_B : board->knight_W;
    Bitboard own_occ = board->turn ? board->occupancyBlack : board->occupancyWhite;
    Bitboard opp_occ = board->turn ? board->occupancyWhite : board->occupancyBlack;

    while (knights) {
        int from = __builtin_ctzll(knights); // Pop Least significant bit
        Bitboard attacks = knight_attack_table[from] & ~own_occ;

        while (attacks) {
            int to = __builtin_ctzll(attacks);
            Bitboard to_bb = 1ULL << to;

            int is_capture = (to_bb & opp_occ) != 0;
            moves[*count] = ENCODE_MOVE(from, to, 0, is_capture, 0, 0); // type 2 = knight
            (*count)++;

            attacks &= attacks - 1;
        }

        knights &= knights - 1;
    }
}

void generate_bishop_moves(Board *board, Move *moves, int *count) {
    return;
}

void generate_rook_moves(Board *board, Move *moves, int *count) {
    return;
}

void generate_queen_moves(Board *board, Move *moves, int *count) {
    return;
}

void generate_king_moves(Board *board, Move *moves, int *count) {
    Bitboard king = board->turn ? board->king_B : board->king_W;
    Bitboard own_occ = board->turn ? board->occupancyBlack : board->occupancyWhite;
    Bitboard opp_occ = board->turn ? board->occupancyWhite : board->occupancyBlack;

    int from = __builtin_ctzll(king); // Pop Least significant bit
    Bitboard attacks = king_attack_table[from] & ~own_occ;

    while (attacks) {
        int to = __builtin_ctzll(attacks);
        Bitboard to_bb = 1ULL << to;

        int is_capture = (to_bb & opp_occ) != 0;
        moves[*count] = ENCODE_MOVE(from, to, 0, is_capture, 0, 0);
        (*count)++;

        attacks &= attacks - 1;
    }
}

Bitboard calculate_pawn_attacks(Bitboard pawns, uint8_t is_white) {
    Bitboard attacks = 0ULL;

    if (is_white) {
        // Weiß: Angriffe nach Nordwest (shift 7) und Nordost (shift 9)
        // aber keine Angriffe von der H-Linie nach rechts (kein Overflow)
        attacks |= (pawns & ~FILE_A) << 7; // keine Angriffe von Spalte A nach links
        attacks |= (pawns & ~FILE_H) << 9; // keine Angriffe von Spalte H nach rechts
    } else {
        // Schwarz: Angriffe nach Südwest (shift 9) und Südost (shift 7)
        attacks |= (pawns & ~FILE_A) >> 9; // keine Angriffe von Spalte A nach links
        attacks |= (pawns & ~FILE_H) >> 7; // keine Angriffe von Spalte H nach rechts
    }

    return attacks;
}

Bitboard calculate_knight_attacks(Bitboard knights) {
    Bitboard attacks = 0ULL;
    while (knights) {
        int from = __builtin_ctzll(knights);  // Pop Least significant bit
        attacks |= knight_attack_table[from];
        knights &= knights - 1;
    }
    return attacks;
}

Bitboard calculate_bishop_attacks(Board *board, Bitboard bishops) {
    return 0;
}

Bitboard calculate_rook_attacks(Board *board, Bitboard rooks) {
    return 0;
}

Bitboard calculate_queen_attacks(Board *board, Bitboard queens) {
    return 0;
}

Bitboard calculate_king_attacks(Bitboard king) {
    Bitboard attacks = 0ULL;
    int from = __builtin_ctzll(king); // Pop Least significant bit
    attacks |= king_attack_table[from];
    return attacks;
}


void update_attackmaps(Board *board) {
    board->attacksWhite = 0ULL;
    board->attacksBlack = 0ULL;

    // Weiß
    board->attacksWhite |= calculate_pawn_attacks(board->pawn_W, 0);
    board->attacksWhite |= calculate_knight_attacks(board->knight_W);
    board->attacksWhite |= calculate_bishop_attacks(board, board->bishop_W);
    board->attacksWhite |= calculate_rook_attacks(board, board->rook_W);
    board->attacksWhite |= calculate_queen_attacks(board, board->queen_W);
    board->attacksWhite |= calculate_king_attacks(board->king_W);

    // Schwarz
    board->attacksBlack |= calculate_pawn_attacks(board->pawn_B, 1);
    board->attacksBlack |= calculate_knight_attacks(board->knight_B);
    board->attacksBlack |= calculate_bishop_attacks(board, board->bishop_B);
    board->attacksBlack |= calculate_rook_attacks(board, board->rook_B);
    board->attacksBlack |= calculate_queen_attacks(board, board->queen_B);
    board->attacksBlack |= calculate_king_attacks(board->king_B);
}

