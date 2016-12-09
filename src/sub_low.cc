// Low-level subtraction. A description to the functions is written in sub_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "sub_low.h"

#ifndef USE_ASM

limb_t __fastcall dec (limb_t *c, const limb_t *a, size_t size_a, limb_t b) {
	assert (size_a > 0);
#ifdef USE_DLIMB
  dlimb_t s = (dlimb_t)b;
  for (size_t i=0; i<size_a; ++i) {
    s = (dlimb_t)a[i] - s;
    c[i] = (limb_t)s;
    s >>= DLIMB_BITS - 1;
  }
  b = (limb_t)s;
#else
  for (size_t i=0; i<size_a; ++i) {
    limb_t s = a[i];
    c[i] = s - b;
    b = (s<b);	// Borrow detection.
  }
#endif
  return b;
}



limb_t __fastcall dec (limb_t *a, size_t size_a, limb_t b) {
	assert (size_a > 0);
	// The trick is that if borrow is zero, then we can stop.
#ifdef USE_DLIMB
  dlimb_t s = (dlimb_t)b;
  for (size_t i=0; s>0 && i<size_a; ++i) {
    s = (dlimb_t)a[i] - s;
    a[i] = (limb_t)s;
    s >>= DLIMB_BITS - 1;
  }
  b = (limb_t)s;
#else
  for (size_t i=0; b>0 && i<size_a; ++i) {
    limb_t s = a[i];
    a[i] = s - b;
    b = (s<b);
  }
#endif
  return b;
}



limb_t __fastcall sub (limb_t *c, const limb_t *a, const limb_t *b, size_t size) {
  assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s = (dlimb_t)a[i] - s;
    s -= b[i];
    c[i] = (limb_t)s;
    s >>= DLIMB_BITS - 1;
  }
  return (limb_t)s;
#else
  limb_t borrow = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = a[i];
    limb_t t = b[i] + borrow;
    borrow = (s<t) || (t<borrow);	// Only one condition is true, so borrow <= 1 always.
    c[i] = s - t;
  }
  return borrow;
#endif
}

#endif
