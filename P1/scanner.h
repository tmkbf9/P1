#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include "token.h"

class Scanner {
 public:
 Scanner(std::istream & tokenStream, std::ostream & errorStream) : tokenStream(tokenStream), errorStream(errorStream) {}
  token scanner() const;

 private:
  Scanner(const Scanner & );
  Scanner & operator=(Scanner &);
  
  std::istream & tokenStream;
  std::ostream & errorStream;
};

/* int scanner(char currentChar, char lookaheadChar, int, int); */
/* int typeOfChar(char); */
/* int FSATable(int, int); */
/* int makeDigit(char, char, int, int); */
/* void makeSymbol(char, int); */
/* void printTokens(); */
/* int makeID(char, char, int, int); */
/* bool isPartOfSet(char); */

#endif //P1_SCANNER_H
