
#include "testScanner.h"
#include "token.h"
#include "scanner.h"

void testScanner(string filename) {
	int state = 0;          //program state
	int linecount = 0;      //line number holder

	ifstream infile(filename.c_str());

	while (true) {
		char c1, c2;
		infile.get(c1);

		if (c1 == '\n') {    //newline counter
			linecount++;
		}

		c2 = infile.peek();     //lookahead char

		if (infile.eof())
			break;

		//call scanner
		state = scanner(c1, c2, state, linecount);
	}

	// call final output
	printTokens();
}

// strips comments from raw.input1 and makes to file for
void prefilter(string filename) {

	string source;
	ifstream readFile(filename.c_str());

	// open file in write mode or out mode
	ofstream ofs;
	ofs.open("toScanner.txt", ofstream::out);

	getline(readFile, source, '\0');

	while (source.find("/*") != std::string::npos) {
		size_t Beg = source.find("/*");
		source.erase(Beg, (source.find("*/", Beg) - Beg) + 2);
	}

	while (source.find("//") != std::string::npos) {
		size_t Beg = source.find("//");
		source.erase(Beg, source.find("\n", Beg) - Beg);
	}

	ofs << source;

	// closing output file
	ofs.close();

	// closing input file
	readFile.close();
}