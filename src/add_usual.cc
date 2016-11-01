// Low-level addition in usual case.
// Here we use a comparison to detect carry.
// A description to the functions is written in add_low.h file.

#include "add_low.h"

#if !defined USE_X64 && !defined USE_BIT_TRICKS && !defined USE_ASM



limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t < s);
    z[i] = t;
  }
  return s;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t<s);
    u[i] = t;
  }
  return s;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  limb_t carry = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i] + carry;
    carry = (s<carry);
    limb_t t = s + v[i];
    carry += (t<s);
    z[i] = t;
  }
  return carry;
}

#endif
