// Low-level subtraction in usual case.
// Here we use a comparison to detect borrow.
// A description to the functions is written in sub_low.h file.

#include "sub_low.h"

#if !defined USE_X64 && !defined USE_BIT_TRICKS && !defined USE_ASM



limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i];
    z[i] = t - s;
    s = (t<s);
  }
  return s;
}



limb_t __fastcall sub (limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i];
    u[i] = t - s;
    s = (t<s);
  }
  return s;
}


#include <stdio.h>
limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  limb_t borrow = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i];
    limb_t t = v[i] + borrow;
    borrow = (s<t) || (t<borrow);
    z[i] = s - t;
  }
  return borrow;
}



#endif
