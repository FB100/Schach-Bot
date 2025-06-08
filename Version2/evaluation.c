#include "evaluation.h"
#include "generation.h"

#define VALUE_PAWN 100
#define VALUE_KNIGHT 300
#define VALUE_BISHOP 300
#define VALUE_ROOK 500
#define VALUE_QUEEN 900


int evaluate_position(Board *board);

int recursive_search_captures(Board *board, uint8_t depth, int alpha, int beta);

int recursive_search(Board *board, uint8_t depth, int alpha, int beta);

//TODO
Move get_best_move(Board *board, uint8_t depth) {

    Move bestMove = 0;
    Move *moves = malloc(sizeof(Move) * MAX_MOVES);
    int moves_size = generate_moves(board, moves);
    moves = realloc(moves, sizeof(Move) * moves_size);
    if (moves == NULL || moves_size == 0) {
        return 0;
    }
    //TODO sort moves
    int score;
    int best_score = -INT32_MAX;
    for (int i = 0; i < moves_size; i++) {
        Board temp = *board;
        make_move(&temp, moves[i]);
        score = -recursive_search(&temp, depth - 1, -INT32_MAX, -INT32_MAX);
        if (score > best_score) {
            bestMove = moves[i];
            best_score = score;
        }
    }

    free(moves);
    return bestMove;;
}

//TODO alpha beta pruning
int recursive_search(Board *board, uint8_t depth, int alpha, int beta) {
    if (depth == 0) {
        return recursive_search_captures(board, MAX_CAPTURE_DEPTH, alpha, beta);
    }

    Move *moves = malloc(sizeof(Move) * MAX_MOVES);
    int moves_size = generate_moves(board, moves) + 1;
    moves = realloc(moves, sizeof(Move) * moves_size);
    if (moves == NULL || moves_size == 0) {
        return 0;
    }
    //TODO sort moves
    int score;
    int best_score = -INT32_MAX;
    for (int i = 0; i < moves_size; i++) {
        score = -recursive_search(board, depth - 1, beta, alpha);
        best_score = score > best_score ? score : best_score;
    }

    free(moves);
    return best_score;
}

int recursive_search_captures(Board *board, uint8_t depth, int alpha, int beta) {
    //TODO
    if (depth == 0) {
        return evaluate_position(board);
    }
    return evaluate_position(board);
}

int evaluate_material(Board *board){
    int evaluation = 0;
    evaluation += __builtin_popcountl(board->pawn_W) * VALUE_PAWN;
    evaluation += __builtin_popcountl(board->knight_W) * VALUE_KNIGHT;
    evaluation += __builtin_popcountl(board->bishop_W) * VALUE_BISHOP;
    evaluation += __builtin_popcountl(board->rook_W) * VALUE_ROOK;
    evaluation += __builtin_popcountl(board->queen_W) * VALUE_QUEEN;

    evaluation -= __builtin_popcountl(board->pawn_B) * VALUE_PAWN;
    evaluation -= __builtin_popcountl(board->knight_B) * VALUE_KNIGHT;
    evaluation -= __builtin_popcountl(board->bishop_B) * VALUE_BISHOP;
    evaluation -= __builtin_popcountl(board->rook_B) * VALUE_ROOK;
    evaluation -= __builtin_popcountl(board->queen_B) * VALUE_QUEEN;
    return evaluation;
}

// Gibt einen Score für Weiß (Höher = besser für Weiß)
int evaluate_position_for_white(Board *board) {
    int evaluation = 0;
    evaluation += evaluate_material(board);
    return evaluation;
}

// Gibt einen Score für den Spieler, der gerade dran ist
int evaluate_position(Board *board) {
    return board->turn ? -evaluate_position_for_white(board) : evaluate_position_for_white(board);
}