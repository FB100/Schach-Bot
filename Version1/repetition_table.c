#include "repetition_table.h"

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