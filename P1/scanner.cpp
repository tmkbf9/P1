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
}

char symbolSet[13] = {'<','>', ':', '+', '-', '*', '/', '.',
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
  int charType;
  
  do {
    int currentChar = tokenStream.get();
    int lookaheadChar = tokenStream.peek();
    errorStream << linenumber << ": " << currentChar << ": " << lookaheadChar << endl;
    
    if(currentChar == '\n') {
      linenumber++;
      continue;
    }
			      
    charType = typeOfChar(currentChar);

    //check fsa table
    nextState = FSATable(nextState, charType);
    
    switch (nextState) {
    case 1:
      buffer.put(currentChar);
      if(!isLowerCase(lookaheadChar) && !isUpperCase(lookaheadChar) && !isdigit(lookaheadChar)) {
	string bufValue = buffer.str();
	buffer.flush();
	return token::ERR_Token(makeErrorString("Invalid token of single character", bufValue, linenumber),
				linenumber);
      }
      break;
    case 2:
      buffer.put(currentChar);
      if(!isdigit(lookaheadChar)) {
	string bufValue = buffer.str();
	buffer.flush();
	return token::NUM_Token(bufValue, linenumber);
      }
      break;

    case 6:
      buffer.put(currentChar);
      if(!isLowerCase(lookaheadChar) && !isUpperCase(lookaheadChar) && !isdigit(lookaheadChar)) {
	string bufValue = buffer.str();
	buffer.flush();
	return token::ID_Token(bufValue, linenumber);
      }
      break;

    case 777:
      return token::EOF_Token(linenumber);
    }
  } while(charType != eof);

  return token("Unknown", "Unknown", -1);
 }

// returns case based on FSA table
/*
token scanner(char currentChar, char lookaheadChar, int state, int linecount) {

  int charType = typeOfChar(currentChar);

  //check fsa table
  int column = FSATable(state, charType);

  switch (column) {
	
  case 0:
    break;
  case 1:
    break;
  case 2:
    state = makeDigit(currentChar, lookaheadChar, state, linecount);
    break;
  case 5:
    makeSymbol(currentChar, linecount);
    //after symtk, go back to state one
    state = 0;
    break;
  case 7:
    state = makeID(currentChar, lookaheadChar, state, linecount);
    break;
  case 111:
    cout << "NUMTK" << endl;
    break;
  case 222:
    cout << "ASSTK" << endl;
    break;
  case 333:
    cout << "EQTK" << endl;
    break;
  case 444:
    cout << "SYMTK" << endl;
    break;
    // case 555 does not exist!
  case 666:
    cout << "IDTK" << endl;
    break;
  case 777:
    return token::EOFToken(linenumber);
  case 999:
    fprintf(stderr, "ERROR: Line#: %d \n    [A-Z] is not valid for state 0", linecount);
    exit(EXIT_FAILURE);
  case 998:
    if (busy == 1) // allows lowercase letters if in IDTK
      break;
    //needs to tell line number here
    fprintf(stderr, "ERROR: Line#: %d \n    IDTK can not start with uppercase letter. \n", linecount);
    exit(EXIT_FAILURE);
  default: //needed?!?
    break;
  }

  return state;
}
*/

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
   * ASSTK = 222
   * EQTK  = 333
   * SYMTK = 444
   * IDTK  = 666
   * EOFTK = 777
   * ERROR: [A-Z] is not valid for s0 = 999
   * ERROR: IDTK must have at least 2 characters = 998
   *
   */

  int FSA_Table[7][7] = {
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
  // else if (isPartOfSet(currentChar)) {     
  //   tableColumn = op;
  // }
  else {
    tableColumn = eof;
  }
  return tableColumn;
}


// id SYMTK, symbol literal, and line num
void makeSymbol(char currentChar, int linenumber) {
  int ii;
  for (ii = 0; ii < sizeof(symbolSet); ii++) {
    if (currentChar == symbolSet[ii]) {
      finalTokenSet[tokenPos].tokenID = "SYMTK";
      finalTokenSet[tokenPos].tokenLiteral = currentChar;
      finalTokenSet[tokenPos].linenumber = linenumber;
      tokenPos++;
    }
  }
}

// makes an IDTK for as long as need be and checks for keyword token
int makeID(char currentChar, char lookaheadChar, int state, int linenumber) {

  finalTokenSet[tokenPos].tokenLiteral += currentChar;
  finalTokenSet[tokenPos].linenumber = linenumber;
  //busy = 1;	  // allows lowercase to pass through

  if (isalpha(lookaheadChar) || isdigit(lookaheadChar)) {     //continue IDTK with letters or numbers
    state = lower;	    //state 4
  }
  else {
    //check if keyword
    int ii;
    for (ii = 0; ii < 12; ii++) {
      if (finalTokenSet[tokenPos].tokenLiteral == keywords[ii]) {
	finalTokenSet[tokenPos].tokenID = keywords[ii] + "TK";
	break;
      }
      else {	  // not keyword
	finalTokenSet[tokenPos].tokenID = "IDTK";
      }
    }

    state = 0;	    //state back to 1
    tokenPos++;	    //increment token array
    //    busy = 0;	    // no more allowing lowercase letters
  }

  return state;
}

// prints final token format: token, literal, line number
void printTokens() {
  int ii = 0;
  cout << endl << endl;
  for (ii = 0; ii < tokenPos; ii++) {
    cout << setw(8) << finalTokenSet[ii].tokenID << "  ";
    cout << setw(8) << finalTokenSet[ii].tokenLiteral << "  ";
    cout << setw(8) << "Line: " << finalTokenSet[ii].linenumber << endl;
  }
  cout << "   EOFTK" << endl;
}

bool isPartOfSymbolSet(char currentChar) {
  for (int i = 0; i < sizeof(symbolSet); i++)
    {
      if (currentChar == symbolSet[i])
	{
	  return true;
	}
    }
  return false;
}
