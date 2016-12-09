// Low-level multiplication. A description to the functions is written in mul_low.h file.
// If 'USE_ASM' is defined then this file isn't used.
// Function mul_n_by_m is out of ifndef(USE_ASM)

#include "misc.h"
#include "mul_low.h"

#ifndef USE_ASM

limb_t __fastcall mul_n_by_1 (limb_t *c, const limb_t *a, size_t size, limb_t b, limb_t s) {
	assert (size > 0);
#ifdef USE_DLIMB
	dlimb_t t = s;
  for (size_t i=0; i<size; ++i) {
    t += (dlimb_t)a[i] * b;
    c[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  s = (limb_t)t;
#else
	limb_t h, l;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, a[i], b);
    c[i] = l + s;
    s = h + (c[i]<l);
  }
#endif
	return s;
}



limb_t __fastcall addmul_n_by_1 (limb_t *c, const limb_t *a, const limb_t *b, size_t size, limb_t s) {
	assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t t = 0;
  for (size_t i=0; i<size; ++i) {
    t += (dlimb_t)b[i]*s + a[i];
    c[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
#else
  limb_t h, l, p, q, t=0;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, b[i], s);
    p = a[i];
    q = l + p;
    h += (q<l);
    c[i] = q + t;
    t = h + (c[i]<q);
  }
#endif
  return (limb_t)t;
}



limb_t __fastcall submul_n_by_1 (limb_t *c, const limb_t *a, const limb_t *b, size_t size, limb_t s) {
	assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t t = 0, r;
  for (size_t i=0; i<size; ++i) {
    t += (dlimb_t)b[i] * s;
    r = (dlimb_t)a[i] - (limb_t)t;
    c[i] = (limb_t)r;
    t >>= LIMB_BITS;
    t += (r >> (DLIMB_BITS-1));
  }
#else
  limb_t h, l, p, q, t=0;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, b[i], s);
    p = a[i];
    q = l + t;
    h += (q<l);
    c[i] = p - q;
    t = h + (p<q);
  }
#endif
  return (limb_t)t;
}

#endif

limb_t __fastcall mul_n_by_m (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (size_a>0 && size_b>0);
  c[size_a] = mul_n_by_1 (c, a, size_a, *b);
  while (--size_b > 0) {
    ++b;
    ++c;
    c[size_a] = addmul_n_by_1 (c, c, a, size_a, *b);
  }
  return c[size_a];
}
