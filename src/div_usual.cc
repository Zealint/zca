// Low-level division in case of USE_X64 is NOT defined.
// Here we use usual limb (32 bit, unsigned) to make division.
// A description to the functions is written in div_low.h file.

#include "div_low.h"
#include "misc.h"

#if !defined USE_X64 && !defined USE_ASM

#ifdef DIV_PREINVERSION



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v) {
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
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v) {
  limb_t q1, q0, t1, t0, R1, R0;
  mul_lo_hi (q1, q0, v, u2);
  q0 += u1;
  q1 += u2 + (q0<u1);
  R0 = u0;
  R1 = u1 - q1*d1;
  mul_lo_hi (t1, t0, d0, q1);
  t0 += d0;
  t1 += d1 + (t0<d0);
  R1 -= t1+(R0<t0);
  R0 -= t0;
  ++q1;
  if (R1 >= q0) {
    --q1;
    R0 += d0;
    R1 += d1 + (R0<d0);
  }
  if (R1>d1 || (R1==d1 && R0>=d0)) {
    ++q1;
    R1 -= d1 + (R0<d0);
    R0 -= d0;
  }
  q = q1;
  r1 = R1;
  r0 = R0;
}



#else



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d) {
	return div_2_by_1<limb_t, hlimb_t> (q, u1, u0, d);
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0) {
	div_3_by_2<limb_t, hlimb_t> (q, r1, r0, u2, u1, u0, d1, d0);
}



#endif


#endif
