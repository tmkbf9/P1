#include <assert.h>

#include "token.h"
#include "scanner.h"

//what goes in scanner.scanner(); File? ostream? string?
//depends on how scanner.scanner works

//prefilter tests
void test_empty_file_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token == EOF_Token); 
}

void test_single_ws_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token == EOF_Token);
}

void test_multiple_ws_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token == EOF_token);
}

void test_single_newline_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token == EOF_token);
}

void test_multiple_newline_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token == EOF_token);
}

void test_multiple_newline_ws_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token  == EOF_token);
}

void test_single_comment_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token  == EOF_token);
}

void test_multpiple_comment_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token  == EOF_token);
}

void test_ws_newline_comment_produces_eof_token() {
  token token = scanner.scanner();
  
  assert(token  == EOF_token);
}

//token tests
void test_single_digit_produces_num_token() {
  token token = scanner.scanner();
  
  assert(token  == NUM_token);
}

void test_multiple_digits_produces_num_token() {
  token token = scanner.scanner();
  
  assert(token  == NUM_token);
}

void test_symbol_produces_sym_token() {
  token token = scanner.scanner();
  
  assert(token  == SYM_token);
}
//correct?
void test_equals_produces_eq_token() {
  token token = scanner.scanner();
  
  assert(token  == EQ_token);
}
void test_equals_equals_produces_ass_token() {
token token = scanner.scanner();
  
  assert(token  == ASS_token);
}
//more tests: IDTK and keyword

//error tests
void test_single_lc_produces_error_and_terminates() {
}

void test_single_uc_produces_error_and_terminates() {
}

void test_invalid_char_produces_error_and_terminates() {
}
