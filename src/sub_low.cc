// Low-level subtraction in usual case. A description to the functions is written in sub_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "sub_low.h"

#ifndef USE_ASM



limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
	assert (size_u > 0);
#ifdef USE_DLIMB
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; i<size_u; ++i) {
    t = (dlimb_t)u[i] - t;
    z[i] = (limb_t)t;
    t >>= DLIMB_BITS - 1;
  }
  s = (limb_t)t;
#else
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i];
    z[i] = t - s;
    s = (t<s);	// Borrow detection.
  }
#endif
  return s;
}



limb_t __fastcall sub (limb_t *u, size_t size_u, limb_t s) {
	assert (size_u > 0);
	// The trick is that if borrow is zero, then we can stop.
#ifdef USE_DLIMB
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; t>0 && i<size_u; ++i) {
    t = (dlimb_t)u[i] - t;
    u[i] = (limb_t)t;
    t >>= DLIMB_BITS - 1;
  }
  s = (limb_t)t;
#else
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i];
    u[i] = t - s;
    s = (t<s);
  }
#endif
  return s;
}



limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s = (dlimb_t)u[i] - s;
    s -= v[i];
    z[i] = (limb_t)s;
    s >>= DLIMB_BITS - 1;
  }
  return (limb_t)s;
#else
  limb_t borrow = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i];
    limb_t t = v[i] + borrow;
    borrow = (s<t) || (t<borrow);	// Only one condition is true, so borrow <= 1 always.
    z[i] = s - t;
  }
  return borrow;
#endif
}



#endif
