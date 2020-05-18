#ifndef P1_TOKEN_H
#define P1_TOKEN_H


#include <iostream>
#include <fstream>
#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>
#include <iomanip>
//#include <locale>         // std::locale, std::isalpha

using namespace std;

struct token {
	string tokenID;
	string tokenLiteral;
	int linecount;
};


#endif //P1_TOKEN_H
