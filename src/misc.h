#ifndef MISC_H
#define MISC_H
#pragma once

#include "typedef.h"

template <typename T>
inline void __fastcall swap (T &a, T &b) {
  T t(a);
  a = b;
  b = t;
}



template <typename T>
inline T __fastcall max (T a, T b) {
  if (a >= b)  return a;
  return b;
}



template <typename T>
inline T __fastcall min (T a, T b) {
  if (a <= b)  return a;
  return b;
}



inline size_t __fastcall abs (offset_t a) {
  if (a < 0)  a = -a;
  return (size_t) a;
}



inline sign_t __fastcall sign (offset_t a) {
  return sign_t (a > 0) - sign_t (a < 0);
}


    // Counting leading zeros.
u8 __fastcall count_lz (limb_t x);



void mul_limbs (limb_t &h, limb_t &l, limb_t u, limb_t v);



    // Copy up 'size' limbs: z=u,
    // Vector 'z' should have enough memory. &u >= &v.
void __fastcall copy_up (limb_t *z, const limb_t *u, size_t size);



    // Copy down 'size' limbs: z=u,
    // Vector 'z' should have enough memory. &u <= &v.
void __fastcall copy_down (limb_t *z, const limb_t *u, size_t size);



    // Make vector 'z' of size 'size' zero: z=0.
void __fastcall make_zero (limb_t *z, size_t size);



    // Make double value (u1, u0) from single values u1 and u0.
template <typename T, typename DT>
DT __fastcall glue (T u1, T u0) {
	const u8 BITS = sizeof(T) * 8;
  return ((DT)u1<<BITS) | u0;
}


    // Multiply 'u' by 'v' of type 'T' to get double size number (h, l).
template <typename T>
void mul_lo_hi (T &h, T &l, T u, T v) {
  T u0, v0, u1, v1, w0, w1, w2, w3;
  const u8 BITS = sizeof(T) * 8;
  const T H_BITS = BITS / 2;
  const T L_MASK = (~(T)0) >> H_BITS;
  u0 = u & L_MASK; u1 = u >> H_BITS;
  v0 = v & L_MASK; v1 = v >> H_BITS;
  w0 = u0 * v0;
  w1 = u1 * v0;
  w2 = u0 * v1;
  w3 = u1 * v1;
  l = w0 + ((w1+w2) << H_BITS);
  w1 += (w0 >> H_BITS);
  w2 += (w1 & L_MASK);
  w1 >>= H_BITS;
  w2 >>= H_BITS;
  h = w1 + w2 + w3;
}



template <typename T, typename HT>
T __fastcall div_2_by_1 (T &q, T u1, T u0, T d);



    // 3/2 division (u2, u1, u0) by (d1, d0) if we don't have data type which is double to 'T'.
    // (u2, u1) < (d1, d0), also (d1, d0)>0 and is normalized.
    // Result in 'q'. Reminder in (r1, r0).
template <typename T, typename HT>
void __fastcall div_3_by_2 (T &q, T &r1, T &r0, T u2, T u1, T u0, T d1, T d0) {
  if (u2 == d1) {
    T s1 = d0 - u1 - (u0 != 0);
    T s0 = -u0;
    if (s1 < d1 || s1 == d1 && s0 <= d0) {
      q = ~(T)0;
      r1 = d1 - s1 - (d0 < s0);
      r0 = d0 - s0;
    } else {
      q = ~(T)0 - (T)1;
      r1 = d1 - s1 - (d0 < s0);
      r0 = d0 - s0;
      r0 += d0;
      r1 += d1 + (r0 < d0);
    }
    return;
  }
  r0 = u0;
  r1 = div_2_by_1 <T, HT> (q, u2, u1, d1);
  T DQ0, DQ1;
  mul_lo_hi<T> (DQ1, DQ0, q, d0);
  if (r1<DQ1 || r1==DQ1 && r0<DQ0) {
    --q;
    r0 += d0;
    r1 += d1 + (r0 < d0);
    if (r1 >= d1 && (r1 < DQ1 || r1 == DQ1 && r0 < DQ0)) {
      --q;
      r0 += d0;
      r1 += d1 + (r0 < d0);
    }
  }
  r1 -= DQ1 + (r0 < DQ0);
  r0 -= DQ0;
}



template <typename T, typename DT>
T __fastcall div_2_by_1 (T &q, DT u, T d);



    // 3/2 division (u2, u1, u0) by d=(d1, d0) if we have data type 'DT' which is double to 'T'.
    // (u2, u1) < d, also d>0 and is normalized.
    // Result in 'q'. Return the reminder.
template <typename T, typename DT>
DT __fastcall div_3_by_2 (T &q, T u2, T u1, T u0, DT d) {
	const u8 BITS = sizeof(T) * 8;
	DT U = glue<T, DT> (u2, u1), r;
	T d1=(T)(d>>BITS), d0=(T)d;
  if (u2 == d1) {
    DT s = glue<T, DT> (d0, 0) - glue<T, DT> (u1, u0);
    if (s <= d) {
      q = ~(T)0;	// 0xFF..F
      r = d - s;
    } else {
      q = (~(T)0) << 1;	// 0xFF..FE
      r = 2*d - s;
    }
    return r;
  }
 	r = ((DT)div_2_by_1<T, DT> (q, U, d1) << BITS) | u0;
  DT DQ = (DT)q * d0;
  if (r < DQ) {
    --q;
    r += d;
    if (r>=d && r<DQ) {
      --q;
      r += d;
    }
  }
  return r -= DQ;
}



    // 2/1 division (u1, u0) by d.
    // u1 < d, also d>0 and d is normalized.
    // Result in q, return the reminder.
    // 'HT' - integral data type which is half of 'T'
template <typename T, typename HT>
T __fastcall div_2_by_1 (T &q, T u1, T u0, T d) {
  const bitcnt_t H_BITS = sizeof (HT) * 8;
  HT u1h, u1l, u0h, u0l, dh, dl, r1, r0, qh, ql;
  u1h = (HT)(u1 >> H_BITS);
  u1l = (HT)u1;
  u0h = (HT)(u0 >> H_BITS);
  u0l = (HT)u0;
  dh = (HT)(d >> H_BITS);
  dl = (HT)d;
  T r = div_3_by_2 <HT, T> (qh, u1h, u1l, u0h, d);
  r = div_3_by_2 <HT, T> (ql, (HT)(r>>H_BITS), (HT)r, u0l, d);
  q = glue<HT, T> (qh, ql);
  return r;
}



    // 2/1 division u=(u1, u0) of type 'DT' by d of type 'T'.
    // Data type 'DT' is double to 'T'.
    // u1 < d, also d>0 and d is normalized.
    // Result in q, return the reminder.
template <typename T, typename DT>
T __fastcall div_2_by_1 (T &q, DT u, T d) {
	q = u / d;
  return u - (DT)q*d;
}


#endif
