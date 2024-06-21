#include "repetition_table.h"
#include "zobrist_hashing.h"

ulong hashes[RT_SIZE];
u_int8_t amount[RT_SIZE] = {0};
int hashesCount = 0;

uint16_t findIndexRepetitionTable(ulong h) {
    for (int i = 0; i < hashesCount; i++) {
        if (hashes[i] == h) {
            return i;
        }
    }
    return -1;
}

u_int8_t amountInRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    return (index == (uint16_t) - 1) ? 0 : amount[index];
}

void pushRepetitionTable(ulong h) {
    uint16_t index = findIndexRepetitionTable(h);
    if (index == (uint16_t) - 1) {
        if (hashesCount < RT_SIZE) {
            hashes[hashesCount] = h;
            amount[hashesCount] = 1;
            hashesCount++;
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
        hashesCount--;
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