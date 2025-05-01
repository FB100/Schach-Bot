#include "tests.h"

// Test-Helfer: Prüft ob Figur auf bestimmtem Feld steht
int is_piece_on_square(Bitboard piece_bb, const char *square) {
    int index = (square[1] - '1') * 8 + (square[0] - 'a');
    if (index < 0) return 0;
    return (piece_bb >> index) & 1ULL;
}


void test_position_input_to_fen() {
    char fen[256];

    // Test 1: Gültiger FEN mit Zügen
    position_input_to_fen("position fen rnbqkbnr/ppp2ppp/4p3/8/3p4/5N2/PPP1PPPP/RNBQKB1R w KQkq d3 0 4 moves e2e4 e7e5", fen, sizeof(fen));
    assert(strcmp(fen, "rnbqkbnr/ppp2ppp/4p3/8/3p4/5N2/PPP1PPPP/RNBQKB1R w KQkq d3 0 4") == 0);

    // Test 2: Gültiger FEN ohne Züge
    position_input_to_fen("position fen rnbqkbnr/ppp2ppp/4p3/8/3p4/5N2/PPP1PPPP/RNBQKB1R b KQkq - 1 3", fen, sizeof(fen));
    assert(strcmp(fen, "rnbqkbnr/ppp2ppp/4p3/8/3p4/5N2/PPP1PPPP/RNBQKB1R b KQkq - 1 3") == 0);

    // Test 3: Kein FEN (startpos statt fen)
    position_input_to_fen("position startpos moves e2e4 e7e5", fen, sizeof(fen));
    assert(strcmp(fen, "") == 0);

    // Test 4: Ungültiger Input (kein "position fen")
    position_input_to_fen("just some random text", fen, sizeof(fen));
    assert(strcmp(fen, "") == 0);

    // Test 5: Gültiger FEN, aber länger als max_len
    position_input_to_fen("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4", fen, 20);
    assert(strcmp(fen, "rnbqkbnr/pppppppp/8") == 0); // abgeschnitten

    printf(" ✓ position_input_to_fen bestanden.\n");
}

void test_position_input_to_moves() {
    char moves[256];

    // Test 1: Gültige startpos mit mehreren Zügen
    position_input_to_moves("position startpos moves e2e4 e7e5 g1f3", moves, sizeof(moves));
    assert(strcmp(moves, "e2e4 e7e5 g1f3") == 0);

    // Test 2: Gültiger FEN mit Zügen
    position_input_to_moves("position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1 moves e7e5", moves, sizeof(moves));
    assert(strcmp(moves, "e7e5") == 0);

    // Test 3: Gültiger FEN ohne moves
    position_input_to_moves("position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", moves, sizeof(moves));
    assert(strcmp(moves, "") == 0);

    // Test 4: Ungültiger Input (keine "moves")
    position_input_to_moves("completely unrelated input", moves, sizeof(moves));
    assert(strcmp(moves, "") == 0);

    // Test 5: Moves länger als max_len
    position_input_to_moves("position startpos moves a1a2 a2a3 a3a4 a4a5 a5a6 a6a7 a7a8", moves, 10);
    assert(strcmp(moves, "a1a2 a2a3") == 0); // abgeschnitten

    printf(" ✓ position_input_to_moves bestanden.\n");
}

void test_set_position_from_FEN() {
    Board board;
    memset(&board, 0, sizeof(Board));

    // Test 1: Startposition
    set_position_from_FEN(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    assert(board.pawn_W == 0x000000000000FF00ULL);
    assert(board.rook_W == 0x0000000000000081ULL);
    assert(board.knight_W == 0x0000000000000042ULL);
    assert(board.bishop_W == 0x0000000000000024ULL);
    assert(board.queen_W == 0x0000000000000008ULL);
    assert(board.king_W == 0x0000000000000010ULL);
    assert(board.pawn_B == 0x00FF000000000000ULL);
    assert(board.rook_B == 0x8100000000000000ULL);
    assert(board.knight_B == 0x4200000000000000ULL);
    assert(board.bishop_B == 0x2400000000000000ULL);
    assert(board.queen_B == 0x0800000000000000ULL);
    assert(board.king_B == 0x1000000000000000ULL);
    assert(board.turn == 0); // w
    assert(board.castling == 0xF); // KQkq (alle Rechte gesetzt)
    assert(board.epSquare == -1); // kein En Passant

    // Test 2: En Passant
    memset(&board, 0, sizeof(Board));
    set_position_from_FEN(&board, "8/8/8/4pP2/8/8/8/8 b - f6 0 1");
    assert(board.epSquare == 45);

    // Test 3: Kein Rochaderecht
    memset(&board, 0, sizeof(Board));
    set_position_from_FEN(&board, "8/8/8/8/8/8/8/8 w - - 0 1");
    assert(board.castling == 0);
    assert(board.occupancy == 0); // Kein Stein gesetzt

    printf(" ✓ set_position_from_FEN bestanden.\n");
}

void test_set_position() {
    Board board;
    memset(&board, 0, sizeof(Board));

    // Test 1: Nur startpos, keine Züge
    set_position(&board, "position startpos");
    assert(is_piece_on_square(board.pawn_W, "e2"));
    assert(is_piece_on_square(board.king_B, "e8"));
    assert(board.turn == 0); // Weiß am Zug

    // Test 2: startpos + einfache Züge
    set_position(&board, "position startpos moves e2e4 e7e5 g1f3 b8c6 f1b5");
    assert(is_piece_on_square(board.pawn_W, "e4"));
    assert(is_piece_on_square(board.knight_W, "f3"));
    assert(is_piece_on_square(board.bishop_W, "b5"));
    assert(board.turn == 1); // Schwarz am Zug

    // Test 3: Benutzerdefiniertes FEN ohne Züge
    set_position(&board, "position fen 8/1B6/8/5p2/8/8/5Qrq/1K1R2bk w - - 0 1");
    assert(is_piece_on_square(board.queen_W, "f2"));
    assert(is_piece_on_square(board.rook_B, "g2"));
    assert(board.turn == 0);

    // Test 4: Benutzerdefiniertes FEN mit Zügen
    set_position(&board, "position fen 8/3P3k/n2K3p/2p3n1/1b4N1/2p1p1P1/8/3B4 w - - 0 1 moves g4f6 h7g7 f6h5 g7g6");
    assert(is_piece_on_square(board.knight_W, "h5")); // Springer hat gezogen
    assert(is_piece_on_square(board.pawn_B, "h6"));   // Schwarzer Bauer ist auf h6


    // Test 5: Weiße kurze Rochade (O-O)
    set_position(&board, "position startpos moves e2e4 e7e5 g1f3 b8c6 f1e2 g8f6 e1g1");
    assert(is_piece_on_square(board.king_W, "g1"));
    assert(is_piece_on_square(board.rook_W, "f1"));

    // Test 6: Schwarze lange Rochade (O-O-O)
    set_position(&board, "position startpos moves e2e4 d7d5 d2d4 d5e4 d1e2 d8d5 e2e4 d5e4 e1c1 e8c8");
    assert(is_piece_on_square(board.king_B, "c8"));
    assert(is_piece_on_square(board.rook_B, "d8"));

    // Test 7: En Passant
    set_position(&board, "position startpos moves e2e4 d7d5 e4d5");
    assert(is_piece_on_square(board.pawn_W, "d5"));
    assert(!is_piece_on_square(board.pawn_B, "d5")); // Schwarzer Bauer wurde geschlagen

    // Test 8: Bauernumwandlung
    set_position(&board, "position startpos moves a2a4 h7h5 a4a5 h5h4 a5a6 h4h3 a6b7 h3g2 b7a8n g2h1");
    assert(is_piece_on_square(board.knight_W, "a8"));
    assert(is_piece_on_square(board.queen_B, "h1"));

    printf(" ✓ set_position bestanden.\n");
}

void test_apply_move_string() {
    Board board;
    memset(&board, 0, sizeof(Board));

    // Test 1: Einfache Züge
    set_position_from_FEN(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    char moves1[] = "e2e4 e7e5";
    apply_move_string(&board, moves1);
    assert(is_piece_on_square(board.pawn_W, "e4"));
    assert(is_piece_on_square(board.pawn_B, "e5"));

    // Test 2: Schlagzug mit 'x' und Schachzeichen '+'
    set_position_from_FEN(&board, "8/8/8/3p4/4P3/8/8/8 w - - 0 1");
    char moves2[] = "e4xd5+";
    apply_move_string(&board, moves2);
    assert(is_piece_on_square(board.pawn_W, "d5"));
    assert(!is_piece_on_square(board.pawn_B, "d5"));

    // Test 3: Rochade kurz (weiß)
    set_position_from_FEN(&board, "rnbqk2r/pppppppp/5n2/8/8/5N2/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    board.whiteKingSq = squarename_to_square("e1");
    char moves3[] = "0-0";
    apply_move_string(&board, moves3);
    assert(is_piece_on_square(board.king_W, "g1"));
    assert(is_piece_on_square(board.rook_W, "f1"));

    // Test 4: Rochade lang (Schwarz)
    set_position_from_FEN(&board, "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
    board.turn = 1;
    board.blackKingSq = squarename_to_square("e8");
    char moves4[] = "0-0-0";
    apply_move_string(&board, moves4);
    assert(is_piece_on_square(board.king_B, "c8"));
    assert(is_piece_on_square(board.rook_B, "d8"));

    // Test 5: Umwandlung
    set_position_from_FEN(&board, "8/P7/8/8/8/8/8/8 w - - 0 1");
    char moves5[] = "a7a8q";
    apply_move_string(&board, moves5);
    assert(is_piece_on_square(board.queen_W, "a8"));
    assert(!is_piece_on_square(board.pawn_W, "a7"));


    printf(" ✓ apply_move_string bestanden.\n");
}




int test_main() {
    // setup
    printf("Test setup:\n");
    test_position_input_to_fen();
    test_position_input_to_moves();
    test_set_position_from_FEN();
    test_set_position();
    // moves_external
    printf("Test moves_external:\n");
    test_apply_move_string();


    return 0;
}