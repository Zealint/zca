// Low-level division. A description to the functions is written in div_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "div_low.h"
#include "misc.h"	// glue, div_3_by_2, div_2_by_1 templates

#ifndef USE_ASM

limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d) {
#ifdef USE_DLIMB
	return div_2_by_1_d<limb_t, dlimb_t> (q, u1, u0, d);
#else
	return div_2_by_1_h<limb_t, hlimb_t> (q, u1, u0, d);
#endif
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0) {
#ifdef USE_DLIMB
	div_3_by_2_d<limb_t, dlimb_t> (q, r1, r0, u2, u1, u0, d1, d0);
#else
	div_3_by_2_h<limb_t, hlimb_t> (q, r1, r0, u2, u1, u0, d1, d0);
#endif
}



#ifdef DIV_PREINVERSION

limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v) {
#ifdef USE_DLIMB
  dlimb_t Q = (dlimb_t)u1 * v;
  Q += glue<limb_t, dlimb_t> (u1+1, u0);
  limb_t q1, q0;
  split<dlimb_t, limb_t> (q1, q0, Q);
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
#else
  limb_t q1, q0;
  mul_lo_hi (q1, q0, u1, v);
  q0 += u0;
  q1 += u1+1+(q0<u0);
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
#endif
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v) {
#ifdef USE_DLIMB
  dlimb_t Q, T, U=glue<limb_t, dlimb_t> (u2, u1);
  dlimb_t D = glue<limb_t, dlimb_t> (d1, d0);
  Q = (dlimb_t)v*u2;
  Q += U;
  limb_t q1, q0;
  split<dlimb_t, limb_t> (q1, q0, Q);
  dlimb_t r = glue<limb_t, dlimb_t> (u1 - q1*d1, u0);
  T = (dlimb_t)d0 * q1;
  T += D;
  r -= T;
  ++q1;
  if (get_hi<dlimb_t, limb_t> (r) >= q0) {
    --q1;
    r += D;
  }
  if (r >= D) {
    ++q1;
    r -= D;
  }
  q = q1;
  split<dlimb_t, limb_t> (r1, r0, r);
#else
  limb_t q1, q0, t1, t0, R1, R0;
  mul_lo_hi (q1, q0, v, u2);
  add_lo_hi (q1, q0, u2, u1);
  R0 = u0;
  R1 = u1 - q1*d1;
  mul_lo_hi (t1, t0, d0, q1);
	add_lo_hi (t1, t0, d1, d0);
  sub_lo_hi (R1, R0, t1, t0);
  ++q1;
  if (R1 >= q0) {
    --q1;
    add_lo_hi (R1, R0, d1, d0);
  }
  if (is_less_or_equal<limb_t> (d1, d0, R1, R0)) {
    ++q1;
    sub_lo_hi (R1, R0, d1, d0);
  }
  q = q1;
  r1 = R1;
  r0 = R0;
#endif
}

#endif  // DIV_PREINVERSION

#endif
