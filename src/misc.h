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



template <typename T>
inline T __fastcall abs (const T &a) {
  if (a < 0)  return -a;
  return a;
}



/*inline size_t __fastcall abs (offset_t a) {
  if (a < 0)  a = -a;
  return (size_t) a;
}*/



/*inline limb_t __fastcall abs (slimb_t a) {
  if (a < 0)  a = -a;
  return (limb_t) a;
}*/



template <typename T>
inline sign_t __fastcall sign (T a) {
  return sign_t (a>0) - sign_t (a<0);
}



bool __fastcall can_copy_up (const limb_t *z, const limb_t *u, size_t size);



    // Counting leading zeros.
u8 __fastcall count_lz (u8 x);
u8 __fastcall count_lz (u16 x);
u8 __fastcall count_lz (u32 x);
u8 __fastcall count_lz (u64 x);



inline bool __fastcall is_power_of_2 (limb_t a) { return a>0 && (a&(a-1)) == 0; }



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
	const u8 BITS = sizeof(DT) * 4;
  return ((DT)u1<<BITS) | u0;
}



    // Split value 'u' by high and low parts of half data type.
template <typename T, typename HT>
void __fastcall split (HT &h, HT &l, T u) {
	const u8 BITS = sizeof(T) * 4;
  h = (HT)(u >> BITS);
  l = (HT)u;
}


template <typename T, typename HT>
HT __fastcall get_lo (T u) { HT h, l;  split<T, HT> (h, l, u);  return l; }

template <typename T, typename HT>
HT __fastcall get_hi (T u) { HT h, l;  split<T, HT> (h, l, u);  return h; }



template <typename T> bool __fastcall is_less_or_equal (T u1, T u0, T v1, T v0) { return u1<v1 || (u1==v1&&u0<=v0); }
template <typename T> bool __fastcall is_less (T u1, T u0, T v1, T v0) { return u1<v1 || (u1==v1&&u0<v0); }



template <typename T>
void __fastcall add_lo_hi (T &u1, T &u0, T v1, T v0) {
	T t = u0 + v0;
	u1 += v1 + (t<u0);
	u0 = t;
}



template <typename T>
void __fastcall sub_lo_hi (T &u1, T &u0, T v1, T v0) {
	u1 -= v1 + (u0<v0);
	u0 -= v0;
}



    // Multiply 'u' by 'v' of type 'T' to get double size number (h, l).
template <typename T>
void __fastcall mul_lo_hi (T &h, T &l, T u, T v) {
  T u0, v0, u1, v1, w0, w1, w2, w3;
  const u8 BITS = sizeof(T) * 8;
  const T H_BITS = BITS / 2;
  const T L_MASK = T(~(T)0) >> H_BITS;
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
T __fastcall div_2_by_1_h (T &q, T u1, T u0, T d);



    // 3/2 division (u2, u1, u0) by (d1, d0) if we don't have data type which is double to 'T'.
    // (u2, u1) < (d1, d0), also d1>0 and is normalized.
    // Result in 'q'. Reminder in (r1, r0).
template <typename T, typename HT>
void __fastcall div_3_by_2_h (T &q, T &r1, T &r0, T u2, T u1, T u0, T d1, T d0) {
  if (u2 == d1) {
  	T s1=d0, s0=0;
  	sub_lo_hi (s1, s0, u1, u0);
  	--(q=0);	// 0xFF..FF
  	r1 = d1;  r0 = d0;
    if (is_less (d1, d0, s1, s0)) {
      --q;	// 0xFF..FE
      add_lo_hi (r1, r0, d1, d0);
    }
    sub_lo_hi (r1, r0, s1, s0);
    return;
  }
  r0 = u0;
  r1 = div_2_by_1_h <T, HT> (q, u2, u1, d1);
  T DQ0, DQ1;
  mul_lo_hi (DQ1, DQ0, q, d0);
  if (is_less (r1, r0, DQ1, DQ0)) {
    --q;
    add_lo_hi (r1, r0, d1, d0);
    if (r1 >= d1 && is_less (r1, r0, DQ1, DQ0)) {
      --q;
      add_lo_hi (r1, r0, d1, d0);
    }
  }
  sub_lo_hi (r1, r0, DQ1, DQ0);
}



template <typename T, typename DT>
T __fastcall div_2_by_1_d (T &q, T u1, T u0, T d);



    // 3/2 division (u2, u1, u0) by (d1, d0) if we have data type 'DT' which is double to 'T'.
    // (u2, u1) < (d1, d0), also d1>0 and is normalized.
    // Result in 'q'. Reminder in (r1, r0)
template <typename T, typename DT>
void __fastcall div_3_by_2_d (T &q, T &r1, T &r0, T u2, T u1, T u0, T d1, T d0) {
	DT D = glue<T, DT> (d1, d0), R;
  if (u2 == d1) {
    DT S = glue<T, DT> (d0, 0) - glue<T, DT> (u1, u0);
    --(q=0); 	// 0xFF..FF
    R = D;
    if (D < S) {
    	--q;	// 0xFF..FE
    	R += D;
    }
    split<DT, T> (r1, r0, R-S);
    return;
  }
  // Here u2 < d1.
 	R = glue<T, DT> (div_2_by_1_d<T, DT> (q, u2, u1, d1), u0);
  DT DQ = (DT)q * d0;
  if (R < DQ) {
    --q;
    R += D;
    if (R>=D && R<DQ) {
      --q;
      R += D;
    }
  }
  split<DT, T> (r1, r0, R-DQ);
}



    // 2/1 division (u1, u0) by d.
    // u1 < d, also d>0 and d is normalized.
    // Result in q, return the reminder.
    // 'HT' - integral data type which is half of 'T'
template <typename T, typename HT>
T __fastcall div_2_by_1_h (T &q, T u1, T u0, T d) {
  HT u1h, u1l, u0h, u0l, d1, d0, qh, ql, r1, r0;
  split<T, HT> (u1h, u1l, u1);
  split<T, HT> (u0h, u0l, u0);
  split<T, HT> (d1, d0, d);
  div_3_by_2_d<HT, T> (qh, r1, r0, u1h, u1l, u0h, d1, d0);
  div_3_by_2_d<HT, T> (ql, r1, r0, r1, r0, u0l, d1, d0);
  q = glue<HT, T> (qh, ql);
  return glue<HT, T> (r1, r0);
}



    // 2/1 division (u1, u0) of type 'DT' by d of type 'T'.
    // Data type 'DT' is double to 'T'.
    // u1 < d, also d>0.
    // Result in q, return the reminder.
template <typename T, typename DT>
T __fastcall div_2_by_1_d (T &q, T u1, T u0, T d) {
	DT U = glue<T, DT> (u1, u0);
	q = U / d;
  return U - (DT)q*d;
}



size_t __fastcall normalize_size (const limb_t *a, size_t n);



char __fastcall char_by_digit (u8);
u8 __fastcall digit_by_char (char);
bool __fastcall is_digit (u8, u8=10U);



char * __fastcall to_string (char *, const limb_t *u, size_t size_u, u8=10U);
size_t __fastcall from_string (limb_t *z, const char *str, u8=10U);

#endif
