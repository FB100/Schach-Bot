#include "evaluation.h"
#include "generation.h"

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

int evaluate_position(Board *board) {
    //TODO
    return 0;
}