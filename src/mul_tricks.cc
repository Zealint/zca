// Low-level multiplication in case of USE_BIT_TRICKS is defined.
// Here we use branch-free bit tricsk to detect carry or borrow.
// A description to the functions is written in mul_low.h file.

#include "consts.h"
#include "misc.h"
#include "mul_low.h"

#if defined USE_BIT_TRICKS && !defined USE_ASM



limb_t __fastcall mul_N_by_1 (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c) {
  limb_t h, l, s = c;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, u[i], v);
    z[i] = l + s;
    s = h + (((l&s) | ((l|s)&(~z[i]))) >> (LIMB_BITS - 1));
  }
  return s;
}



limb_t __fastcall addmul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
  limb_t h, l, p, q, s = 0;
  for (size_t i = 0; i < size; ++i) {
    mul_lo_hi (h, l, v[i], w);
    p = u[i];
    q = l + p;
    h += (((l&p) | ((l|p)&(~q))) >> (LIMB_BITS - 1));
    z[i] = q + s;
    s = h + (((q&s) | ((q|s)&(~z[i]))) >> (LIMB_BITS-1));
  }
  return s;
}



limb_t __fastcall submul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w) {
  limb_t h, l, p, q, s=0;
  for (size_t i=0; i<size; ++i) {
    mul_lo_hi (h, l, v[i], w);
    p = u[i];
    q = l + s;
    h += (((l&s) | ((l|s)&(~q))) >> (LIMB_BITS - 1));
    z[i] = p - q;
    s = h + (((~p&q) | ((~p|q)&z[i])) >> (LIMB_BITS - 1));
  }
  return s;
}



#endif
