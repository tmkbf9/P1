#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include "token.h"

int scanner(char, char, int, int);
int typeOfChar(char);
int FSATable(int, int);
int makeDigit(char, char, int, int);
void makeSymbol(char, int);
void printTokens();
int makeID(char, char, int, int);
int isPartOfSet(char, char[]);

#endif //P1_SCANNER_H
