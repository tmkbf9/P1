#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include <iosfwd>
#include <sstream>
#include "token.h"

class Scanner {
 public:
 Scanner(std::istream & tokenStream, std::ostream & errorStream) 
   : tokenStream(tokenStream), errorStream(errorStream), linenumber(0) {}
  
  token scanner();

 private:
  Scanner(const Scanner & );
  Scanner & operator=(Scanner &);
  
  int typeOfChar(char) const;
  int FSATable(int, int) const;

  std::istream & tokenStream;
  std::ostream & errorStream;
  int linenumber;

  std::ostringstream buffer;
};

/* int makeDigit(char, char, int, int); */
/* void makeSymbol(char, int); */
/* void printTokens(); */
/* int makeID(char, char, int, int); */
/* bool isPartOfSet(char); */

#endif //P1_SCANNER_H
