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

        uint64_t is_king_in_check = temp.turn ? temp.whiteKingSq & temp.attacksBlack : temp.blackKingSq & temp.attacksWhite;

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
    Bitboard *pieces[] = {
            is_white ? &board->pawn_W : &board->pawn_B,
            is_white ? &board->knight_W : &board->knight_B,
            is_white ? &board->bishop_W : &board->bishop_B,
            is_white ? &board->rook_W : &board->rook_B,
            is_white ? &board->queen_W : &board->queen_B,
            is_white ? &board->king_W : &board->king_B
    };

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
        Bitboard *targets[] = {
                is_white ? &board->pawn_B : &board->pawn_W,
                is_white ? &board->knight_B : &board->knight_W,
                is_white ? &board->bishop_B : &board->bishop_W,
                is_white ? &board->rook_B : &board->rook_W,
                is_white ? &board->queen_B : &board->queen_W,
                is_white ? &board->king_B : &board->king_W
        };

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

    // 9. Belegung aktualisieren
    board->occupancyWhite =
            board->pawn_W | board->knight_W | board->bishop_W |
            board->rook_W | board->queen_W | board->king_W;

    board->occupancyBlack =
            board->pawn_B | board->knight_B | board->bishop_B |
            board->rook_B | board->queen_B | board->king_B;

    board->occupancy = board->occupancyWhite | board->occupancyBlack;

    // 10. Spielerwechsel
    board->turn ^= 1;

    // 11. Zobrist Hash, Attack Maps etc. → separat aktualisieren
}


void generate_pawn_moves(Board *board, Move *moves, int *count) {
    return;
}

void generate_knight_moves(Board *board, Move *moves, int *count) {
    int us = board->turn;
    Bitboard knights = us ? board->knight_B : board->knight_W;
    Bitboard own_occ = us ? board->occupancyBlack : board->occupancyWhite;
    Bitboard opp_occ = us ? board->occupancyWhite : board->occupancyBlack;

    while (knights) {
        int from = __builtin_ctzll(knights);
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
    return;
}
