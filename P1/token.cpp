#include <iostream>
#include "token.h"

ostream & operator<<(ostream & os, const token & t) {
  t.print(os);
  return os;
}
