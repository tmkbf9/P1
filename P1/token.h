#ifndef P1_TOKEN_H
#define P1_TOKEN_H

#include <string>

struct token {
  static token EOF_Token(int linenumber) { return token("EOFTK", "", linenumber); }

  token() {}
  void print(std::ostream & os);

  bool operator==(const token & other) const { return this->tokenID == other.tokenID; }
  bool operator!=(const token & other) const { return !(*this == other); }
  
  std::string tokenID;
  std::string tokenLiteral;
  int linenumber;

private:
  token(std::string tokenID, std::string tokenLiteral, int linenumber) 
    : tokenID(tokenID), tokenLiteral(tokenLiteral), linenumber(linenumber) {}
};

std::ostream & operator<<(std::ostream & os, const token & t);

#endif //P1_TOKEN_H
