// Low-level division. A description to the functions is written in div_low.h file.
// If 'USE_ASM' is defined then this file isn't used.

#include "add_low.h"
#include "sub_low.h"
#include "mul_low.h"
#include "div_low.h"
#include "misc.h"

#ifndef USE_ASM

limb_t __fastcall div_2_by_1 (limb_t &q, limb_t a1, limb_t a0, limb_t d) {
	assert (d & HIGH_BIT_MASK);
#ifdef USE_DLIMB
	return div_2_by_1_d<limb_t, dlimb_t> (q, a1, a0, d);
#else
	return div_2_by_1_h<limb_t, hlimb_t> (q, a1, a0, d);
#endif
}



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t a2, limb_t a1, limb_t a0, limb_t d1, limb_t d0) {
	assert (d1 & HIGH_BIT_MASK);
	assert (&q!=&r1 && &q!=&r0);
#ifdef USE_DLIMB
	div_3_by_2_d<limb_t, dlimb_t> (q, r1, r0, a2, a1, a0, d1, d0);
#else
	div_3_by_2_h<limb_t, hlimb_t> (q, r1, r0, a2, a1, a0, d1, d0);
#endif
}



#ifdef DIV_PREINVERSION

limb_t __fastcall div_2_by_1 (limb_t &q, limb_t a1, limb_t a0, limb_t d, limb_t v) {
	assert (d & HIGH_BIT_MASK);
#ifdef USE_DLIMB
  dlimb_t Q = (dlimb_t)a1 * v;
  Q += glue<limb_t, dlimb_t> (a1+1, a0);
  limb_t q1, q0;
  split<dlimb_t, limb_t> (q1, q0, Q);
  limb_t r = a0 - q1*d;
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
  mul_lo_hi (q1, q0, a1, v);
  q0 += a0;
  q1 += a1+1+(q0<a0);
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



void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t a2, limb_t a1, limb_t a0, limb_t d1, limb_t d0, limb_t v) {
	assert (d1 & HIGH_BIT_MASK);
	assert (&q!=&r1 && &q!=&r0);
#ifdef USE_DLIMB
  dlimb_t Q, T, U=glue<limb_t, dlimb_t> (a2, a1);
  dlimb_t D = glue<limb_t, dlimb_t> (d1, d0);
  Q = (dlimb_t)v*a2;
  Q += U;
  limb_t q1, q0;
  split<dlimb_t, limb_t> (q1, q0, Q);
  dlimb_t r = glue<limb_t, dlimb_t> (a1 - q1*d1, a0);
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
  mul_lo_hi (q1, q0, v, a2);
  add_lo_hi (q1, q0, u2, a1);
  R0 = a0;
  R1 = a1 - q1*d1;
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

#endif	// USE_ASM

#ifdef DIV_PREINVERSION

limb_t __fastcall inv_2_by_1 (limb_t d) {
	limb_t v;
  div_2_by_1 (v, ~d, LIMB_MAX, d);
  return v;
}



limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0) {
  limb_t v, r1, r0;
  div_3_by_2 (v, r1, r0, ~d1, ~d0, LIMB_MAX, d1, d0);
  return v;
}

#endif



#ifdef DIV_PREINVERSION
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t a_n, const limb_t *a, size_t n, limb_t d, limb_t v) {
#else
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t a_n, const limb_t *a, size_t n, limb_t d) {
#endif
	assert (n > 0);
	assert (d & HIGH_BIT_MASK);
	assert (a_n < d);
  limb_t r(a_n);
  while (n-- > 0)
#ifdef DIV_PREINVERSION
  	r = div_2_by_1 (q[n], r, a[n], d, v);
#else
		r = div_2_by_1 (q[n], r, a[n], d);
#endif
  return r;
}


#ifdef DIV_PREINVERSION
limb_t __fastcall mod_n_by_1 (const limb_t a_n, const limb_t *a, size_t n, limb_t d, limb_t v) {
#else
limb_t __fastcall mod_n_by_1 (const limb_t a_n, const limb_t *a, size_t n, limb_t d) {
#endif
	assert (n > 0);
	assert (d & HIGH_BIT_MASK);
	assert (a_n < d);
  limb_t r(a_n), q;
  while (n-- > 0)
#ifdef DIV_PREINVERSION
  	r = div_2_by_1 (q, r, a[n], d, v);
#else
		r = div_2_by_1 (q, r, a[n], d);
#endif
  return r;
}



#ifdef DIV_PREINVERSION
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0, limb_t v) {
#else
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0) {
#endif
	assert (n > 1);
	assert (d1 & HIGH_BIT_MASK);
	assert (a_n<d1 || a_n==d1&&a[n-1]<d0);
  limb_t r1 = a_n, r0 = a[--n];
  while (n-- > 0)
#ifdef DIV_PREINVERSION
  	div_3_by_2 (q[n], r1, r0, r1, r0, a[n], d1, d0, v);
#else
		div_3_by_2 (q[n], r1, r0, r1, r0, a[n], d1, d0);
#endif
  if (r != nullptr) { r[0] = r0;  r[1] = r1; }
}



#ifdef DIV_PREINVERSION
void __fastcall mod_n_by_2 (limb_t &r1, limb_t &r0, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0, limb_t v) {
#else
void __fastcall mod_n_by_2 (limb_t &r1, limb_t &r0, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0) {
#endif
	assert (n > 1);
	assert (d1 & HIGH_BIT_MASK);
	assert (a_n<d1 || a_n==d1&&a[n-1]<d0);
  r1 = a_n;
  r0 = a[--n];
  limb_t q;
  while (n-- > 0)
#ifdef DIV_PREINVERSION
  	div_3_by_2 (q, r1, r0, r1, r0, a[n], d1, d0, v);
#else
		div_3_by_2 (q, r1, r0, r1, r0, a[n], d1, d0);
#endif
}



#ifdef DIV_PREINVERSION
void __fastcall div_m_by_n (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d, limb_t v) {
#else
void __fastcall div_m_by_n (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d) {
#endif
	assert (size_a >= size_d);
	assert (size_d > 2);
	assert (d[size_d-1] & HIGH_BIT_MASK);

  size_t k = size_a - size_d;
  limb_t Q, R1, R0;
  size_t i = size_d;
  while (--i > 0)  if (a[i+k] != d[i])  break;
  // u >= d*beta^k
  if (a[i+k] >= d[i]) {
    if (q != nullptr)  q[k] = 1;
    sub (a+k, a+k, d, size_d);  // u -= d*beta^k
  }
  else if (q != nullptr)  q[k] = 0;
  limb_t d1=d[size_d-1], d0=d[size_d-2];
  while (k-- > 0) {
    --size_a;
    limb_t a2=a[size_d+k], a1=a[size_d+k-1], a0=a[size_d+k-2];
    if (a2==d1 && a1==d0)  Q = LIMB_MAX;
#ifdef DIV_PREINVERSION
    else  div_3_by_2 (Q, R1, R0, a2, a1, a0, d1, d0, v);
#else
    else  div_3_by_2 (Q, R1, R0, a2, a1, a0, d1, d0);
#endif
    limb_t borrow = submul_n_by_1 (a+k, a+k, d, size_d, Q);  // u -= Q*d*beta^k
    if (borrow > a2) {
      --Q;
      add (a+k, a+k, d, size_d);
    }
    a[size_d + k] = 0;
    if (q != nullptr)  q[k] = Q;
  }
  size_a = normalize_size (a, size_a);
}
