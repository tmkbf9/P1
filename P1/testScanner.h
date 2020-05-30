#ifndef __TEST_SCANNER_H
#define __TEST_SCANNER_H

#include <iosfwd>

class Scanner;

class TestScanner {
public:
  TestScanner() : lineNumber(0) {}

  void testScan(std::istream & inputSource, std::ostream & errorStream);

private:
  int lineNumber;
};

#endif
