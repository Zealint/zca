// Low-level multiplication in case of USE_X64 is defined.
// Here we use double limb (64 bit, unsigned) to detect carry, borrow or to make 64=32x32 multiplication.
// A description to the functions is written in mul_low.h file.

#include "consts.h"
#include "mul_low.h"

#if defined USE_X64 && !defined USE_ASM



limb_t __fastcall mul_N_by_1 (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c) {
  dlimb_t s = c;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)u[i] * v;
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
}



limb_t __fastcall addmul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)v[i]*w + u[i];
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
}



limb_t __fastcall submul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
  dlimb_t s = 0, t;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)v[i] * w;
    t = (dlimb_t)u[i] - (limb_t)s;
    z[i] = (limb_t)t;
    s >>= LIMB_BITS;
    s += (t >> (2*LIMB_BITS - 1));
  }
  return (limb_t)s;
}



#endif
