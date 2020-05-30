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
  
  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 3);
}

void test_only_comment_produces_eof_token() {
    istringstream is("%aaa%");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();

    assert(token.tokenID == "EOFTK");
    assert(token.linenumber == 0);
}

// single character tests
void test_single_digit_produces_number_token() {
  istringstream is("1");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "NUMTK");
  assert(token.tokenLiteral == "1");
  assert(token.linenumber == 0);
}

void test_EOF_token_seen_after_last_token_read() {
  istringstream is("1");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  token = scanner.scanner();

  assert(token.tokenID == "EOFTK");
  assert(token.linenumber == 0);
}

void test_single_symbol_produces_symbol_token() {
    istringstream is("[");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();

    assert(token.tokenID == "SYMTK");
    assert(token.tokenLiteral == "[");
    assert(token.linenumber == 0);
}

void test_single_equals_produces_eq_token() {
    istringstream is("=");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();

    assert(token.tokenID == "SYMTK");
    assert(token.tokenLiteral == "=");
    assert(token.linenumber == 0);
}

// multi-character tests
void test_two_digits_create_digit_token() {
  istringstream is("12");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "NUMTK");
  assert(token.tokenLiteral == "12");
  assert(token.linenumber == 0);
}

void test_two_digits_followed_by_symbol_create_digit_token() {
  istringstream is("12<");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "NUMTK");
  assert(token.tokenLiteral == "12");
  assert(token.linenumber == 0);
}

void test_two_valid_characters_create_id_token() {
  istringstream is("vars");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "IDTK");
  assert(token.tokenLiteral == "vars");
  assert(token.linenumber == 0);
}

void test_detects_keyword_token_from_id_token() {
  istringstream is("var");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  assert(token.tokenID == "KEYTK");
  assert(token.tokenLiteral == "var");
  assert(token.linenumber == 0);
}

void test_double_equals_produces_ass_token() {
    istringstream is("==");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();

    assert(token.tokenID == "SYMTK");
    assert(token.tokenLiteral == "==");
    assert(token.linenumber == 0);
}

// error tests
void test_single_lcase_char_produces_error() {
  istringstream is("a");
  ostringstream os;
  Scanner scanner(is, os);

  token token = scanner.scanner();
  
  ostringstream errorString;
  
  assert(token.tokenID == "ERRTK");
  assert(token.tokenLiteral == "Error at line 0: Invalid token of single character <a>");
  assert(token.linenumber == 0);
}

void test_single_ucase_char_produces_error() {
    istringstream is("A");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();

    ostringstream errorString;

    assert(token.tokenID == "ERRTK");
    assert(token.tokenLiteral == "Error at line 0: Invalid token of starting with capital letter <A>");
    assert(token.linenumber == 0);
}

void test_invalid_char_produces_error() {
    istringstream is("(");
    ostringstream os;
    Scanner scanner(is, os);

    token token = scanner.scanner();
    ostringstream errorString;

    assert(token.tokenID == "ERRTK");
    assert(token.tokenLiteral == "Error at line 0: Invalid character <(>");
    assert(token.linenumber == 0);
}

int main(int argc, char ** argv) {
  test_empty_file_produces_eof_token();
  test_only_one_space_produces_eof_token();
  test_multiple_spaces_produces_eof_token();
  test_only_newlines_produces_eof_token_with_correct_linenumber();
  test_single_digit_produces_number_token();
  test_EOF_token_seen_after_last_token_read();
  test_two_digits_create_digit_token();
  test_two_digits_followed_by_symbol_create_digit_token();
  test_single_lcase_char_produces_error();
  test_detects_keyword_token_from_id_token();
  test_two_valid_characters_create_id_token();
  test_single_ucase_char_produces_error();
  test_single_symbol_produces_symbol_token();
  test_single_equals_produces_eq_token();
  test_double_equals_produces_ass_token();
  test_only_comment_produces_eof_token();
  test_invalid_char_produces_error();

  return 0;
}
