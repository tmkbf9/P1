#include <iostream>

#include "token.h"
#include "testScanner.h"
#include "scanner.h"

using namespace std;

void TestScanner::testScan(istream & inputSource, ostream & errorStream) {
  token token;
  istringstream is;
  ostringstream os;
  Scanner scanner(is, os);
  do {
    token = scanner.scanner();
    cout << token << endl;
  } while(token != token::EOF_Token(-1));
}
