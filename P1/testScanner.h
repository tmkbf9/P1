#ifndef __TEST_SCANNER_H
#define __TEST_SCANNER_H

#include <iosfwd>

class Scanner;

class TestScanner {
public:
 TestScanner(const Scanner & scanner) : scanner(scanner) {}

  void testScan(std::istream & inputSource, std::ostream & errorStream);

private:
  const Scanner & scanner;
  int lineNumber;
};

#endif
