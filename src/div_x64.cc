// Low-level division in case of USE_X64 is defined.
// Here we use double limb (64 bit, unsigned) to make 64/32 division.
// A description to the functions is written in div_low.h file.

#include "consts.h"
#include "div_low.h"
#include "misc.h"

#if defined USE_X64 && !defined USE_ASM

#ifndef DIV_PREINVERSION



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d) {
	return div_2_by_1<limb_t, dlimb_t> (q, glue<limb_t, dlimb_t> (u1, u0), d);
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0) {
	dlimb_t r = div_3_by_2<limb_t, dlimb_t> (q, u2, u1, u0, glue<limb_t, dlimb_t> (d1, d0));
	r1 = (limb_t)(r >> LIMB_BITS);
	r0 = (limb_t)r;
}



#else



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v) {
  dlimb_t Q = (dlimb_t)u1 * v;
  Q += glue<limb_t, dlimb_t> (u1+1, u0);
  limb_t q1=(limb_t)(Q>>LIMB_BITS), q0=(limb_t)Q;
  limb_t r = u0 - q1*d;
  if (r > q0) {
    --q1;
    r += d;
  }
  if (r >= d) {
    ++q1;
    r -= d;
  }
  q = q1;
  return r;
}



dlimb_t __fastcall div_3_by_2 (limb_t &q, limb_t u2, limb_t u1, limb_t u0, dlimb_t d, limb_t v) {
  dlimb_t Q, T, U=glue<limb_t, dlimb_t> (u2, u1);
  Q = (dlimb_t)v*u2;
  Q += U;
  limb_t q1=(limb_t)(Q>>LIMB_BITS), q0=(limb_t)Q;
  limb_t d1=(limb_t)(d>>LIMB_BITS), d0=(limb_t)d;
  dlimb_t r = glue<limb_t, dlimb_t> (u1 - q1*d1, u0);
  T = (dlimb_t)d0 * q1;
  T += d;
  r -= T;
  ++q1;
  if ((limb_t)(r>>LIMB_BITS) >= q0) {
    --q1;
    r += d;
  }
  if (r >= d) {
    ++q1;
    r -= d;
  }
  q = q1;
  return r;
}



#endif

#endif
