#include "scanner.h"
#include "testScanner.h"
#include "token.h"

char symbolSet[13] = {'<','>', ':', '+', '-', '*', '/', '.',
					  ',', '[', ']', '#', '&' }; //'=' and '==' are not treated as symbols. They are treated seperately.

string keywords[12] = { "begin", "end", "repeat", "if", "void", "return", "write",
					 "scan", "program", "let", "then" };

token finalTokenSet[256];
int tokenPos = 0;   // finalTokenSet placeholder
int busy = 0;       // ignore lowercase if building an IDTK

// returns case based on FSA table
int scanner(char c1, char c2, int state, int linecount) {

	int charType = typeOfChar(c1);

	//check fsa table
	int column = FSATable(state, charType);

	switch (column) {
	
	case 000:
		break;
	case 001:
		break;
	case 002:
		state = makeDigit(c1, c2, state, linecount);
		break;
	case 003:
		makeSymbol(c1, linecount);
		//after symtk, go back to state one
		state = 0;
		break;
	case 007:
		state = makeID(c1, c2, state, linecount);
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
		cout << "EOFTK" << endl;
		break;
	case 999:
		fprintf(stderr, "ERROR: Line#: %d \n    [A-Z\ is not valid for state 0", linecount);
		exit(EXIT_FAILURE);
	case 998:
		if (busy == 1) // allows lowercase letters if in IDTK
			break;
		//needs to tell line number here
		fprintf(stderr, "ERROR: Line#: %d \n    IDTK can not start with lowercase letter. \n", linecount);
		exit(EXIT_FAILURE);
	default: //needed?!?
		break;
	}
	

	return state;
}

int FSATable(int state, int col) {

	/*
	 * STATES:
	 * s0 = 000	
	 * s1 = 001
	 * s2 = 002
	 * s3 = 003
	 * s4 = 004
	 * s5 = 005 
	 * s7 = 007
	 * s6 doesn't exist. Get over it.
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
			{000, 999, 001, 002, 005, 003, 777}, //from s0. next line is from s1 and so on
			{998, 007, 007, 007, 998, 998, 998},
			{111, 111, 111, 002, 111, 111, 111},
			{222, 222, 222, 222, 222, 004, 222},
			{333, 333, 333, 333, 333, 333, 333},
			{444, 444, 444, 444, 444, 444, 444},
			{666, 007, 007, 007, 666, 666, 666}
	};

	int tableString = FSA_Table[state][col];

	return tableString;

}

// idenifies type of char on the FSA Table
int typeOfChar(char c1) {

	int tableColumn = -1;

	if (c1 == ' ')
	{
		tableColumn = 0;
	}
	else if (isalpha(c1) && isupper(c1)) {  //uppercase
		tableColumn = 1;
	}
	else if (isalpha(c1) && islower(c1)) {        //lowercase
		tableColumn = 2;
	}
	else if (isdigit(c1)) {                  //digit
		tableColumn = 3;
	}
	else if (c1 == '=') {
		tableColumn = 5;
	}
	else if (isPartOfSet) {                                  //symbol
		tableColumn = 4;
	}
	else {
		tableColumn = 6;
	}
	return tableColumn;
}

// creates an INTTK for as long as need be
int makeDigit(char c1, char c2, int state, int linecount) {

	finalTokenSet[tokenPos].tokenLiteral += c1;     //append number
	finalTokenSet[tokenPos].linecount = linecount;  //linecount

	if (isdigit(c2)) {
		state = 1;      // send back to state 2 if digit
	}
	else {
		finalTokenSet[tokenPos].tokenID = "INTTK";
		state = 0;      // send to initial state 1 if not a digit
		tokenPos++;     // increment token array
	}
	return state;
}

// id SYMTK, symbol literal, and line num
void makeSymbol(char c1, int linecount) {
	int ii;
	for (ii = 0; ii < sizeof(symbolSet); ii++) {
		if (c1 == symbolSet[ii]) {
			finalTokenSet[tokenPos].tokenID = "SYMTK";
			finalTokenSet[tokenPos].tokenLiteral = c1;
			finalTokenSet[tokenPos].linecount = linecount;
			tokenPos++;
		}
	}
}

// makes an IDTK for as long as need be and checks for keyword token
int makeID(char c1, char c2, int state, int linecount) {

	finalTokenSet[tokenPos].tokenLiteral += c1;
	finalTokenSet[tokenPos].linecount = linecount;
	busy = 1;       // allows lowercase to pass through

	if (isalpha(c2) || isdigit(c2)) {     //continue IDTK with letters or numbers
		state = 3;      //state 4
	}
	else {
		//check if keyword
		int ii;
		for (ii = 0; ii < 12; ii++) {
			if (finalTokenSet[tokenPos].tokenLiteral == keywords[ii]) {
				finalTokenSet[tokenPos].tokenID = keywords[ii] + "TK";
				break;
			}
			else {      // not keyword
				finalTokenSet[tokenPos].tokenID = "IDTK";
			}
		}

		state = 0;      //state back to 1
		tokenPos++;     //increment token array
		busy = 0;       // no more allowing lowercase letters
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
		cout << setw(8) << "Line: " << finalTokenSet[ii].linecount << endl;
	}
	cout << "   EOFTK" << endl;
}

bool isPartOfSet(char c1, char set[] ) {
	for (int i = 0; i < (sizeof(set)/sizeof(set[0])); i++)
	{
		if (c1 == set[i])
		{
			return true;
		}
	}
	return false;
}
