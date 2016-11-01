// Low-level subtraction in case of USE_BIT_TRICKS is defined.
// Here we use branch-free bit tricks to detect borrow.
// A description to the functions is written in sub_low.h file.

#include "consts.h"
#include "add_low.h"

#if defined USE_BIT_TRICKS && !defined USE_ASM



limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i];
    limb_t r = t - s;
    s = ((~t&s) | ((~t|s)&r)) >> (LIMB_BITS - 1);
    z[i] = r;
  }
  return s;
}



limb_t __fastcall sub (limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i];
    limb_t r = t - s;
    s = ((~t&s) | ((~t|s)&r)) >> (LIMB_BITS - 1);
    u[i] = r;
  }
  return s;
}



limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  limb_t borrow = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i];
    limb_t t = v[i];
    limb_t r = s - t - borrow;
    borrow = ((~s&t) | ((~s|t)&r)) >> (LIMB_BITS - 1);
    z[i] = r;
  }
  return borrow;
}



#endif
