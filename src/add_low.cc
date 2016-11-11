// Low-level addition. A description to the functions is written in add_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "add_low.h"

#ifndef USE_ASM



limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
	assert (size_u > 0);
#ifdef USE_DLIMB
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; i<size_u; ++i) {
    t += u[i];
    z[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  s = (limb_t)t;
#else
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t < s);	// Carry detection.
    z[i] = t;
  }
#endif
  return s;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
	assert (size_u > 0);
	// The trick is that if carry 's' is zero then we can stop.
#ifdef USE_DLIMB
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; t>0 && i<size_u; ++i) {
    t += u[i];
    u[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  s = (limb_t)t;
#else
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t<s);	// Carry detection.
    u[i] = t;
  }
#endif
  return s;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
	assert (size > 0);
#ifdef USE_DLIMB
 	dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += u[i];
    s += v[i];
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
#else
  limb_t carry = 0;
  // Only one of two carry detections will kick in (so, carry <= 1 always).
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i] + carry;
    carry = (s<carry);
    limb_t t = s + v[i];
    carry += (t<s);
    z[i] = t;
  }
  return carry;
#endif
}

#endif
