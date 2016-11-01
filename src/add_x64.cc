// Low-level addition in case of USE_X64 is defined.
// Here we use double limb (64 bit, unsigned) to detect carry.
// A description to the functions is written in add_low.h file.

#include "consts.h"
#include "add_low.h"

#if defined USE_X64 && !defined USE_ASM



limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; i<size_u; ++i) {
    t += u[i];
    z[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  return (limb_t)t;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; t>0 && i<size_u; ++i) {
    t += u[i];
    u[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  return (limb_t)t;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += u[i];
    s += v[i];
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
}



#endif
