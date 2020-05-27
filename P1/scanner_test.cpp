#include <iostream>
#include <sstream>
#include <assert.h>
#include "scanner.h"

using namespace std;

//
// If you want to view what happens inside of a test, replace 'os' with 'cerr' in constructor for Scanner
//

// prefilter tests
void test_empty_file_produces_eof_token() {
  istringstream is;
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 0);
}

void test_only_one_space_produces_eof_token() {
  istringstream is(" ");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 0);
}

void test_multiple_spaces_produces_eof_token() {
  istringstream is("     ");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 0);
}

void test_only_newlines_produces_eof_token_with_correct_linenumber() {
  istringstream is("\n\n\n");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  cout << token << endl;
  
  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 3);
}

// single character tests
void test_single_digit_produces_number_token() {

}

void test_single_operator_produces_op_token() {
  
}

void test_single_equals_produces_op_token() {
  
}

// error tests
void test_single_lcase_char_produces_error() {
}

void test_single_ucase_char_produces_error() {
}

int main(int argc, char ** argv) {
  test_empty_file_produces_eof_token();
  test_only_one_space_produces_eof_token();
  test_multiple_spaces_produces_eof_token();
  test_only_newlines_produces_eof_token_with_correct_linenumber();
  
  return 0;
}
