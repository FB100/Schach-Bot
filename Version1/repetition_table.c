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
    return index == (uint16_t) - 1 ? 0 : amount[index];
}

void pushRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    if (index == (uint16_t) - 1) {
        if (count < RT_SIZE) {
            hashes[count] = h;
            amount[count] = 1;
            count++;
        }
    } else {
        amount[index]++;
    }
}

void popRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    if (index == (uint16_t) - 1) {
        return;
    }
    amount[index]--;
    if(amount[index] == 0){
        count--;
    }
}


//Gibt zurück, ob ein Remis durch Stellungswiederholung passiert
bool isRepetition(uint64_t hash) {
    if (hash == 0) { return false; }
    if (amountInRepetitionTable(hash) < 3) {
        return false;
    }
    return true;
}