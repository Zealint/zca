// Low-level addition. A description to the functions is written in add_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "add_low.h"

#ifndef USE_ASM

limb_t __fastcall inc (limb_t *c, const limb_t *a, size_t size_a, limb_t b) {
	assert (size_a > 0);
#ifdef USE_DLIMB
  dlimb_t s = (dlimb_t)b;
  for (size_t i=0; i<size_a; ++i) {
    s += a[i];
    c[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  b = (limb_t)s;
#else
  for (size_t i=0; i<size_a; ++i) {
    limb_t s = a[i] + b;
    b = (s < b);	// Carry detection.
    z[i] = s;
  }
#endif
  return b;
}



limb_t __fastcall inc (limb_t *a, size_t size_a, limb_t b) {
	assert (size_a > 0);
	// The trick is that if carry 's' is zero then we can stop.
#ifdef USE_DLIMB
  dlimb_t s = (dlimb_t)b;
  for (size_t i=0; s>0 && i<size_a; ++i) {
    s += a[i];
    a[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  b = (limb_t)s;
#else
  for (size_t i=0; b>0 && i<size_a; ++i) {
    limb_t s = a[i] + b;
    b = (s<b);	// Carry detection.
    a[i] = s;
  }
#endif
  return b;
}



limb_t __fastcall add (limb_t *c, const limb_t *a, const limb_t *b, size_t size) {
	assert (size > 0);
#ifdef USE_DLIMB
 	dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += a[i];
    s += b[i];
    c[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
#else
  limb_t carry = 0;
  // Only one of two carry detections will kick in (so, carry <= 1 always).
  for (size_t i=0; i<size; ++i) {
    limb_t s = a[i] + carry;
    carry = (s<carry);
    limb_t t = s + b[i];
    carry += (t<s);
    c[i] = t;
  }
  return carry;
#endif
}

#endif
