#include "test.h"
#include <stdio.h>


int main() {
  if (unit_tests()) {
    fprintf (stdout, "OK\n");
    return 0;
  } else {
    fprintf (stdout, "ERROR\n");
    return 1;
  }
}
