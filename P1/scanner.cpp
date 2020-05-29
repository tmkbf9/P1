#include <iostream>
#include <iomanip>

#include "scanner.h"
#include "testScanner.h"
#include "token.h"

using namespace std;

namespace {
  bool isUpperCase(char c) {
    return isalpha(c) && isupper(c);
  }
  
  bool isLowerCase(char c) {
    return isalpha(c) && islower(c);
  }

  string makeErrorString(const string & reason, const string & curValue, int linenumber) {
    ostringstream err;
    err << "Error at line " << linenumber << ": " << reason << " <" << curValue << ">";

    return err.str();
  }

  bool isInSymbolSet(char c, char symbolSet[]) {
      for (int indexOfSet = 0; indexOfSet < 13; indexOfSet++) {
          if (c == symbolSet[indexOfSet]) return true;
      }
      return false;
  }
}

char symbolSet[13] = { '<','>', ':', '+', '-', '*', '/', '.',
              ',', '[', ']', '#', '&' }; //'=' and '==' are not treated as symbols. They are treated seperately.

string keywords[12] = { "begin", "end", "repeat", "if", "void", "return", "write",
			"scan", "program", "let", "then" };

enum ColumnLabels
  {
   whitespace = 0,
   upper,
   lower,
   digit,
   op,
   equals,
   eof
  };
		   

token finalTokenSet[256];
int tokenPos = 0;   // finalTokenSet placeholder

token Scanner::scanner() {
  int nextState = 0;
  int charType = 0;
  bool isComment = false;
  
  do {
    int currentChar = tokenStream.get();
    int lookaheadChar = tokenStream.peek();
    errorStream << linenumber << ": " << currentChar << ": " << lookaheadChar << endl;
    
    if(currentChar == '\n') {
      linenumber++;
      continue;
    }

    if (currentChar == '%' && isComment == true) {
        isComment = false;
        continue;
    }
    else if (isComment == true && currentChar != '%') {
        continue;
    }
    if(currentChar == '%' && isComment == false) {
        isComment = true;
        continue;
    }
 

        charType = typeOfChar(currentChar);

        if (charType != -1) {
            //check fsa table
            nextState = FSATable(nextState, charType);
        } else {
        nextState = -1;
        }

        buffer.put(currentChar);
        string bufValue = buffer.str();

        switch (nextState) {
        case -1:
            return token::ERR_Token(makeErrorString("Invalid character", bufValue, linenumber), linenumber);
            break;
        case 1:
            if (!isLowerCase(lookaheadChar) && !isUpperCase(lookaheadChar) && !isdigit(lookaheadChar)) {
                buffer.flush();
                return token::ERR_Token(makeErrorString("Invalid token of single character", bufValue, linenumber),
                    linenumber);
            }
            break;
        case 2:
            if (!isdigit(lookaheadChar)) {
                buffer.flush();
                return token::NUM_Token(bufValue, linenumber);
            }
            break;
        case 3:
            if (lookaheadChar != '=') {
                buffer.flush();
                return token::SYM_Token(bufValue, linenumber);
            }
            break;
        case 4:
            buffer.flush();
            return token::SYM_Token(bufValue, linenumber);
            break;
        case 5:
            buffer.flush();
            return token::SYM_Token(bufValue, linenumber);
            break;
        case 6:
            if (!isLowerCase(lookaheadChar) && !isUpperCase(lookaheadChar) && !isdigit(lookaheadChar)) {
                buffer.flush();
                return token::ID_Token(bufValue, linenumber);
            }
            break;
        case 777:
            return token::EOF_Token(linenumber);
            break;
        case 999:
            buffer.flush();
            return token::ERR_Token(makeErrorString("Invalid token of starting with capital letter", bufValue, linenumber), linenumber);
            break;
        }
  } while(charType != eof);

  return token("Unknown", "Unknown", -1);
 }

int Scanner::FSATable(int state, int col) const {

  /*
   * STATES:
   * s0 = 000	
   * s1 = 001
   * s2 = 002
   * s3 = 003
   * s4 = 004
   * s5 = 005 
   * s6 = 006
   *
   * TOKENS:
   * NUMTK = 111 
   * ASSTK = 333 -> converted to SYMTK
   * EQTK  = 222 -> converted to SYMTK
   * SYMTK = 444
   * IDTK  = 666
   * EOFTK = 777
   * ERROR: [A-Z] is not valid for s0 = 999
   * ERROR: IDTK must have at least 2 characters = 998
   *
   */

  int FSA_Table[7][7] = {
            // ws, A-Z, a-z, 0-9, SYM, '=', EOF 
			 {000, 999, 001, 002, 005, 003, 777},
			 {998, 006, 006, 006, 998, 998, 998},
			 {111, 111, 111, 002, 111, 111, 111},
			 {222, 222, 222, 222, 222, 004, 222},
			 {333, 333, 333, 333, 333, 333, 333},
			 {444, 444, 444, 444, 444, 444, 444},
			 {666, 006, 006, 006, 666, 666, 666}
  };

  int nextState = FSA_Table[state][col];

  return nextState;;
}


// identifies type of char on the FSA Table
int Scanner::typeOfChar(char currentChar) const {

  int tableColumn = -1;

  if (currentChar == ' ') {
    tableColumn = whitespace;
  }
  else if (isUpperCase(currentChar)) {
    tableColumn = upper;
  }
  else if (isLowerCase(currentChar)) {
    tableColumn = lower;
  }
  else if (isdigit(currentChar)) {
    tableColumn = digit;
  }
  else if (currentChar == '=') {
    tableColumn = equals;
  }
  else if (isInSymbolSet(currentChar, symbolSet)) {     
    tableColumn = op;
  }
  else if ((int) currentChar == EOF) {
    tableColumn = eof;
  }
  return tableColumn;
}

