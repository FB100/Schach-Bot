#include "repetition_table.h"
#include "zobrist_hashing.h"

ulong hashes[RT_SIZE];
u_int8_t amount[RT_SIZE] = {0};
int count = 0;

uint16_t findIndexRepetitionTable(ulong h) {
    for (int i = 0; i < count; i++) {
        if (hashes[i] == h) {
            return true;
        }
    }
    return -1;
}

u_int8_t amountInRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    return index == (uint16_t) -1? 0 : amount[index];
}

void pushRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    if (index == (uint16_t) -1) {
        if (count < RT_SIZE) {
            hashes[count] = h;
            count++;
        }
    } else {
        amount[index]++;
    }
}

//Gibt zurück, ob ein Remis durch Stellungswiederholung passiert
bool handleRepetitions(Piece board[BOARD_SIZE][BOARD_SIZE]){
    ulong hash = computeHash(board);
    if (amountInRepetitionTable(hash) < 3){
        pushRepetitionTable(hash);
        return true;
    }
    return false;
}