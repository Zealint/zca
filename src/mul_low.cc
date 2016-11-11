// Low-level multiplication. A description to the functions is written in mul_low.h file.
 
#include "misc.h"	// mul_lo_hi template.
#include "mul_low.h"

#ifndef USE_ASM



limb_t __fastcall mul_n_by_1 (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c) {
	assert (size > 0);
#ifdef USE_DLIMB
	dlimb_t s = c;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)u[i] * v;
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
#else
	limb_t h, l, s = c;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, u[i], v);
    z[i] = l + s;
    s = h + (z[i]<l);
  }  
#endif
	return (limb_t)s;
}



limb_t __fastcall addmul_n_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
	assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)v[i]*w + u[i];
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
#else
  limb_t h, l, p, q, s = 0;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, v[i], w);
    p = u[i];
    q = l + p;
    h += (q<l);
    z[i] = q + s;
    s = h + (z[i]<q);
  }
#endif
  return (limb_t)s;
}



limb_t __fastcall submul_n_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
	assert (size > 0);
#ifdef USE_DLIMB
  dlimb_t s = 0, t;
  for (size_t i=0; i<size; ++i) {
    s += (dlimb_t)v[i] * w;
    t = (dlimb_t)u[i] - (limb_t)s;
    z[i] = (limb_t)t;
    s >>= LIMB_BITS;
    s += (t >> (DLIMB_BITS-1));
  }
#else
  limb_t h, l, p, q, s=0;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, v[i], w);
    p = u[i];
    q = l + s;
    h += (q<l);
    z[i] = p - q;
    s = h + (p<q);
  }
#endif
  return (limb_t)s;
}



#endif
