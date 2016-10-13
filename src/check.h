#ifndef CHECK_H
#define CHECK_H

#pragma once

#include <stdio.h>

inline bool __check (bool v, int line) {
  if (!v)  printf ("Check at line %d failed!\n", line);
  return v;
}

#define check(v) (ok = __check(v, __LINE__) && ok)

#endif
