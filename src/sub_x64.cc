// Low-level subtraction in case of USE_X64 is defined.
// Here we use double limb (64 bit, unsigned) to detect borrow.
// A description to the functions is written in sub_low.h file.

#include "consts.h"
#include "sub_low.h"

#if defined USE_X64 && !defined USE_ASM



limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; i<size_u; ++i) {
    t = (dlimb_t)u[i] - t;
    z[i] = (limb_t)t;
    t = (t >> (2*LIMB_BITS - 1));
  }
  return (limb_t)t;
}



limb_t __fastcall sub (limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; t>0 && i<size_u; ++i) {
    t = (dlimb_t)u[i] - t;
    u[i] = (limb_t)t;
    t = (t >> (2*LIMB_BITS - 1));
  }
  return (limb_t)t;
}



limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s = (dlimb_t)u[i] - s;
    s -= v[i];
    z[i] = (limb_t)s;
    s >>= (2*LIMB_BITS - 1);
  }
  return (limb_t) s;
}



#endif
