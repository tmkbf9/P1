#include <iostream>
#include <fstream>
#include "testScanner.h"

using namespace std;

int main(int argc, char ** argv) {
  TestScanner testScanner;

  cerr.setstate(std::ios_base::badbit);
  
  if(argc == 1) // read from stdin
    testScanner.testScan(cin, cerr);
  else if(argc == 2)  {// read from input file
    ifstream ifs(argv[1]);
    testScanner.testScan(ifs, cerr);
  }
  else {
    cerr << "Two many command line parameters given. Aborting" << endl;
    return 1;
  }

  return 0;
}
