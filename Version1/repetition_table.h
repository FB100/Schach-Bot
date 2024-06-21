#ifndef VERSION1_REPETITION_TABLE_H
#define VERSION1_REPETITION_TABLE_H

#include "headers.h"

void printRepetitiontable();

int8_t amountInRepetitionTable(ulong h);

void pushRepetitionTable(ulong h);

void popRepetitionTable(ulong h);

bool isRepetition(uint64_t hash);


#endif //VERSION1_REPETITION_TABLE_H
