#include "board.h"
#include "zobrist_hashing.h"
#include "repetition_table.h"

char captureTypeStack[CS_SIZE];
bool captureColorStack[CS_SIZE];
int captureStackCount = 0;

void pushCaptureStack(char capturedPiece, bool white) {
    if (captureStackCount >= CS_SIZE) {
        fprintf(stderr, "Capturestack size limit exeeded");
        return;
    }
    captureTypeStack[captureStackCount] = capturedPiece;
    captureColorStack[captureStackCount] = white;
    captureStackCount++;
}

char popCaptureStack() {
    if (captureStackCount <= 0) {
        fprintf(stderr, "Capturestack pop on empty Stack not allowed");
        return ' ';
    }
    captureStackCount--;
    return captureTypeStack[captureStackCount];
}

void resetCaptureStack() {
    captureStackCount = 0;
}

void computeOccupancyMasks(Board *board) {
    board->occupancyWhite = board->pawn_W | board->knight_W | board->bishop_W | board->rook_W | board->queen_W | board->king_W;
    board->occupancyBlack = board->pawn_B | board->knight_B | board->bishop_B | board->rook_B | board->queen_B | board->king_B;
    board->occupancy = board->occupancyBlack | board->occupancyWhite;
}

//TODO irgendwie buggy
void updateBitBoardBoard(Move move, Board *bitBoardBoard) {
    switch (move.type) {
        case PAWN_W:
            bitBoardBoard->pawn_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case PAWN_B:
            bitBoardBoard->pawn_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case KNIGHT_W:
            bitBoardBoard->knight_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case KNIGHT_B:
            bitBoardBoard->knight_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case BISHOP_W:
            bitBoardBoard->bishop_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case BISHOP_B:
            bitBoardBoard->bishop_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case ROOK_W:
            bitBoardBoard->rook_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case ROOK_B:
            bitBoardBoard->rook_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case QUEEN_W:
            bitBoardBoard->queen_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case QUEEN_B:
            bitBoardBoard->queen_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case KING_W:
            bitBoardBoard->king_W ^= (1ULL << move.to) & (1ULL << move.from);
            break;
        case KING_B:
            bitBoardBoard->king_B ^= (1ULL << move.to) & (1ULL << move.from);
            break;
    }

    bitBoardBoard->hash ^= getZobristTable(move.from / BOARD_SIZE, move.from % BOARD_SIZE, move.type);
    bitBoardBoard->hash ^= getZobristTable(move.to / BOARD_SIZE, move.to % BOARD_SIZE, move.type);
    computeOccupancyMasks(bitBoardBoard);
}

void updateBitBoardBoardPromotion(Move move, Board *bitBoardBoard) {
    switch (move.special) {
        case 4:
            bitBoardBoard->pawn_W ^= !(move.type / 8) ? 1ULL << move.from : 0ULL;
            bitBoardBoard->queen_W ^= !(move.type / 8) ? 1ULL << move.to : 0ULL;
            bitBoardBoard->pawn_B ^= move.type / 8 ? 1ULL << move.from : 0ULL;
            bitBoardBoard->queen_B ^= move.type / 8 ? 1ULL << move.to : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(move.from / BOARD_SIZE, move.from % BOARD_SIZE, move.type);
            bitBoardBoard->hash ^= getZobristTable(move.to / BOARD_SIZE, move.to % BOARD_SIZE, move.type + 4);
            break;
        case 5:
            bitBoardBoard->pawn_W ^= !(move.type / 8) ? 1ULL << move.from : 0ULL;
            bitBoardBoard->rook_W ^= !(move.type / 8) ? 1ULL << move.to : 0ULL;
            bitBoardBoard->pawn_B ^= move.type / 8 ? 1ULL << move.from : 0ULL;
            bitBoardBoard->rook_B ^= move.type / 8 ? 1ULL << move.to : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(move.from / BOARD_SIZE, move.from % BOARD_SIZE, move.type);
            bitBoardBoard->hash ^= getZobristTable(move.to / BOARD_SIZE, move.to % BOARD_SIZE, move.type + 3);
            break;
        case 6:
            bitBoardBoard->pawn_W ^= !(move.type / 8) ? 1ULL << move.from : 0ULL;
            bitBoardBoard->bishop_W ^= !(move.type / 8) ? 1ULL << move.to : 0ULL;
            bitBoardBoard->pawn_B ^= move.type / 8 ? 1ULL << move.from : 0ULL;
            bitBoardBoard->bishop_B ^= move.type / 8 ? 1ULL << move.to : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(move.from / BOARD_SIZE, move.from % BOARD_SIZE, move.type);
            bitBoardBoard->hash ^= getZobristTable(move.to / BOARD_SIZE, move.to % BOARD_SIZE, move.type + 2);
            break;
        case 7:
            bitBoardBoard->pawn_W ^= !(move.type / 8) ? 1ULL << move.from : 0ULL;
            bitBoardBoard->knight_W ^= !(move.type / 8) ? 1ULL << move.to : 0ULL;
            bitBoardBoard->pawn_B ^= move.type / 8 ? 1ULL << move.from : 0ULL;
            bitBoardBoard->knight_B ^= move.type / 8 ? 1ULL << move.to : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(move.from / BOARD_SIZE, move.from % BOARD_SIZE, move.type);
            bitBoardBoard->hash ^= getZobristTable(move.to / BOARD_SIZE, move.to % BOARD_SIZE, move.type + 1);
            break;
        default:
            break;
    }
    computeOccupancyMasks(bitBoardBoard);
}

void makeMove(Move move, Piece board[BOARD_SIZE][BOARD_SIZE], Board *bitBoardBoard) {
    int whiteSize = 0;

    pushCaptureStack(board[move.to / 8][move.to % 8].type, board[move.to / 8][move.to % 8].white);

    switch (move.special) {
        case 0:
            //normal Moves
            board[move.to / 8][move.to % 8] = board[move.from / 8][move.from % 8];
            board[move.from / 8][move.from % 8].type = ' ';
            updateBitBoardBoard(move, bitBoardBoard);
            break;
        case 2:
            whiteSize = (move.from - 4) / 8;
            // kurze Rochade
            board[whiteSize][4].type = ' ';
            board[whiteSize][5].type = 'R';
            board[whiteSize][5].white = whiteSize / 7;
            board[whiteSize][6].type = 'K';
            board[whiteSize][6].white = whiteSize / 7;
            board[whiteSize][7].type = ' ';
            bitBoardBoard->rook_W ^= move.type == KING_W ? (1ULL << (whiteSize * BOARD_SIZE + 5)) & (1ULL << (whiteSize * BOARD_SIZE + 7)) : 0ULL;
            bitBoardBoard->rook_B ^= move.type == KING_B ? (1ULL << (whiteSize * BOARD_SIZE + 5)) & (1ULL << (whiteSize * BOARD_SIZE + 7)) : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 7, move.type + 3);
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 5, move.type + 3);
            updateBitBoardBoard(move, bitBoardBoard); //updated den King (Rook wird hier ja extra gemacht)
            break;
        case 3:
            whiteSize = (move.from - 4) / 8;
            // Lange Rochade
            board[whiteSize][0].type = ' ';
            board[whiteSize][2].type = 'K';
            board[whiteSize][2].white = whiteSize / 7;
            board[whiteSize][3].type = 'R';
            board[whiteSize][3].white = whiteSize / 7;
            board[whiteSize][4].type = ' ';
            bitBoardBoard->rook_W ^= move.type == KING_W ? (1ULL << (whiteSize * BOARD_SIZE + 3)) & (1ULL << (whiteSize * BOARD_SIZE)) : 0ULL;
            bitBoardBoard->rook_B ^= move.type == KING_B ? (1ULL << (whiteSize * BOARD_SIZE + 3)) & (1ULL << (whiteSize * BOARD_SIZE)) : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 0, move.type + 3);
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 3, move.type + 3);
            updateBitBoardBoard(move, bitBoardBoard);
            break;
        case 4:
            // Promotion Dame
            board[move.to / 8][move.to % 8] = board[move.from / 8][move.from % 8];
            board[move.to / 8][move.to % 8].type = 'Q';
            board[move.from / 8][move.from % 8].type = ' ';
            updateBitBoardBoardPromotion(move, bitBoardBoard);
            break;
        case 5:
            // Promotion Turm
            board[move.to / 8][move.to % 8] = board[move.from / 8][move.from % 8];
            board[move.to / 8][move.to % 8].type = 'R';
            board[move.from / 8][move.from % 8].type = ' ';
            updateBitBoardBoardPromotion(move, bitBoardBoard);
            break;
        case 6:
            // Promotion Läufer
            board[move.to / 8][move.to % 8] = board[move.from / 8][move.from % 8];
            board[move.to / 8][move.to % 8].type = 'B';
            board[move.from / 8][move.from % 8].type = ' ';
            updateBitBoardBoardPromotion(move, bitBoardBoard);
            break;
        case 7:
            // Promotion Springer
            board[move.to / 8][move.to % 8] = board[move.from / 8][move.from % 8];
            board[move.to / 8][move.to % 8].type = 'N';
            board[move.from / 8][move.from % 8].type = ' ';
            updateBitBoardBoardPromotion(move, bitBoardBoard);
            break;
        default:
            break;
    }
    pushRepetitionTable(bitBoardBoard->hash);
}

void unmakeMove(Move move, Piece board[BOARD_SIZE][BOARD_SIZE], Board *bitBoardBoard) {
    int whiteSize = 0;
    Piece capturedUnit;
    capturedUnit.type = popCaptureStack();
    capturedUnit.white = captureColorStack[captureStackCount];

    switch (move.special) {
        case 0:
            //normal Moves
            board[move.from / BOARD_SIZE][move.from % BOARD_SIZE] = board[move.to / BOARD_SIZE][move.to % BOARD_SIZE];
            board[move.to / BOARD_SIZE][move.to % BOARD_SIZE] = capturedUnit;
            updateBitBoardBoard(move, bitBoardBoard);
            break;
        case 2:
            whiteSize = (move.from - 4) / BOARD_SIZE;
            // kurze Rochade
            board[whiteSize][4].type = 'K';
            board[whiteSize][4].white = whiteSize / 7;
            board[whiteSize][5].type = ' ';
            board[whiteSize][6].type = ' ';
            board[whiteSize][7].type = 'R';
            board[whiteSize][7].white = whiteSize / 7;
            bitBoardBoard->rook_W ^= move.type == KING_W ? (1ULL << (whiteSize * BOARD_SIZE + 5)) & (1ULL << (whiteSize * BOARD_SIZE + 7)) : 0ULL;
            bitBoardBoard->rook_B ^= move.type == KING_B ? (1ULL << (whiteSize * BOARD_SIZE + 5)) & (1ULL << (whiteSize * BOARD_SIZE + 7)) : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 7, move.type + 3);
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 5, move.type + 3);
            updateBitBoardBoard(move, bitBoardBoard);
            break;
        case 3:
            whiteSize = (move.from - 4) / BOARD_SIZE;
            // Lange Rochade
            board[whiteSize][0].type = 'R';
            board[whiteSize][0].white = whiteSize / 7;
            board[whiteSize][2].type = ' ';
            board[whiteSize][3].type = ' ';
            board[whiteSize][4].type = 'K';
            board[whiteSize][4].white = whiteSize / 7;
            bitBoardBoard->rook_W ^= move.type == KING_W ? (1ULL << (whiteSize * BOARD_SIZE + 3)) & (1ULL << (whiteSize * BOARD_SIZE)) : 0ULL;
            bitBoardBoard->rook_B ^= move.type == KING_B ? (1ULL << (whiteSize * BOARD_SIZE + 3)) & (1ULL << (whiteSize * BOARD_SIZE)) : 0ULL;
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 0, move.type + 3);
            bitBoardBoard->hash ^= getZobristTable(whiteSize, 3, move.type + 3);
            updateBitBoardBoard(move, bitBoardBoard);
            break;
        case 4 ... 7:
            // Promotion
            board[move.from / BOARD_SIZE][move.from % BOARD_SIZE] = board[move.to / BOARD_SIZE][move.to % BOARD_SIZE];
            board[move.from / BOARD_SIZE][move.from % BOARD_SIZE].type = 'P';
            board[move.to / BOARD_SIZE][move.to % BOARD_SIZE] = capturedUnit;
            updateBitBoardBoardPromotion(move, bitBoardBoard);
            break;
        default:
            break;
    }
    popRepetitionTable(bitBoardBoard->hash);
}