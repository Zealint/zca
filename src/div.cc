// High-level division.
// A description to the function is written in div.h file.

#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "div_low.h"
#include "div_pre_low.h"
#include "misc.h"



#ifdef DIV_PREINVERSION



limb_t __fastcall inv_2_by_1 (limb_t d) {
	limb_t v;
#ifdef USE_X64
  div_2_by_1 <limb_t, dlimb_t> (v, glue<limb_t, dlimb_t> (~d, LIMB_MAX), d);
#else
  div_2_by_1 <limb_t, hlimb_t> (v, ~d, LIMB_MAX, d);
#endif
  return v;
}



limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0) {
#ifdef USE_X64
  limb_t v;
  div_3_by_2 <limb_t, dlimb_t> (v, ~d1, ~d0, LIMB_MAX, glue<limb_t, dlimb_t> (d1, d0));
#else
  limb_t v, r1, r0;
  div_3_by_2 <limb_t, hlimb_t> (v, r1, r0, ~d1, ~d0, LIMB_MAX, d1, d0);
#endif
  return v;
}



limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, limb_t d, limb_t v) {
  limb_t r(u_n), Q;
  size_t i = n;
  if (q != nullptr)  while (i-- > 0)  r = div_2_by_1 (q[i], r, u[i], d, v);
  else  while (i-- > 0)  r = div_2_by_1 (Q, r, u[i], d, v);
  return r;
}



void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, limb_t d1, limb_t d0, limb_t v) {
  limb_t r1 = u_n, r0 = u[n-1], Q;
#ifdef USE_X64
  dlimb_t R = glue<limb_t, dlimb_t> (r1, r0);
  dlimb_t D = glue<limb_t, dlimb_t> (d1, d0);
#endif
  size_t i = n - 1;
#ifdef USE_X64
  if (q != nullptr)  while (i-- > 0)  R = div_3_by_2 (q[i], (limb_t)(R>>LIMB_BITS), (limb_t)R, u[i], D, v);
  else  while (i-- > 0)  R = div_3_by_2 (Q, (limb_t)(R>>LIMB_BITS), (limb_t)R, u[i], D, v);
  if (r != nullptr) {
    r[0] = (limb_t)R;
    r[1] = (limb_t)(R>>LIMB_BITS);
  }
#else
  if (q != nullptr)  while (i-- > 0)  div_3_by_2 (q[i], r1, r0, r1, r0, u[i], d1, d0, v);
  else  while (i-- > 0)  div_3_by_2 (Q, r1, r0, r1, r0, u[i], d1, d0, v);
  if (r != nullptr) {
    r[0] = r0;
    r[1] = r1;
  }
#endif
}


#else


limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, limb_t d) {
  limb_t r(u_n), Q;
  size_t i = n;
  if (q != nullptr)  while (i-- > 0)  r = div_2_by_1 (q[i], r, u[i], d);
  else  while (i-- > 0)  r = div_2_by_1 (Q, r, u[i], d);
  return r;
}


void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, limb_t d1, limb_t d0) {
  limb_t r1 = u_n, r0 = u[n-1], Q;
  size_t i = n - 1;
  if (q != nullptr)  while (i-- > 0)  div_3_by_2 (q[i], r1, r0, r1, r0, u[i], d1, d0);
  else  while (i-- > 0)  div_3_by_2 (Q, r1, r0, r1, r0, u[i], d1, d0);
  if (r != nullptr) {
    r[0] = r0;
    r[1] = r1;
  }
}



#endif


#ifdef DIV_PREINVERSION
void __fastcall div_m_by_n (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d, limb_t d1, limb_t d0, limb_t v) {
#else
void __fastcall div_m_by_n (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d, limb_t d1, limb_t d0) {
#endif
  size_t k = size_u - size_d;
  limb_t Q, R1, R0;
  // Сравниваем u и d*beta^k
  size_t i = size_d;
  while (--i > 0)  if (u[i+k] != d[i])  break;
  // u >= d*beta^k
  if (u[i+k] >= d[i]) {
    if (q != nullptr)  q[k] = 1;
    sub (u + k, u + k, d, size_d);  // u -= d*beta^k
  }
  else if (q != nullptr)  q[k] = 0;
  while (k-- > 0) {
    --size_u;
    limb_t u2=u[size_d+k], u1=u[size_d+k-1], u0=u[size_d+k-2];
    if (u2==d1 && u1==d0)  Q = LIMB_MAX;
#ifdef DIV_PREINVERSION
  #ifdef USE_X64
    else {
      dlimb_t R = div_3_by_2 (Q, u2, u1, u0, glue<limb_t, dlimb_t> (d1, d0), v);
      R1 = (limb_t)(R>>LIMB_BITS);
      R0 = (limb_t)R;
    }
  #else
    else  div_3_by_2 (Q, R1, R0, u2, u1, u0, d1, d0, v);
  #endif
#else
    else  div_3_by_2 (Q, R1, R0, u2, u1, u0, d1, d0);
#endif
    limb_t borrow = submul_N_by_1 (u + k, u + k, d, size_d, Q);  // u -= Q*d*beta^k
    if (borrow > u2) {  // Если вычли слишком много (заём больше, чем u2, который должен уничтожиться).
      --Q;
      add (u + k, u + k, d, size_d); // u += d*beta^k
    }
    u[size_d + k] = 0; // Уничтожаем старший лимб.
    if (q != nullptr)  q[k] = Q;
  }
}


static u8 __fastcall normalize_divisor (limb_t &d) {
	u8 shift = count_lz (d);
	d <<= shift;
	return shift;
}



static u8 __fastcall normalize_divisor (limb_t &d1, limb_t &d0) {
	u8 shift = count_lz (d1);
	if (shift) {
		d1 = (d1<<shift) | (d0>>(LIMB_BITS-shift));
		d0 <<= shift;
	}
	return shift;
}



size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d) {
  limb_t *u_shifted, u_n=0, c=(limb_t)(u[n-1]<d);
  u8 shift = normalize_divisor (d);
#ifdef DIV_PREINVERSION
  limb_t v = inv_2_by_1 (d);
#endif
  if (shift > 0) {
    u_shifted = new limb_t[n];
    u_n = shift_left_short (u_shifted, u, n, shift);
    u = u_shifted;
  }
#ifdef DIV_PREINVERSION
  limb_t R = div_n_by_1 (q, u_n, u, n, d, v);
#else
  limb_t R = div_n_by_1 (q, u_n, u, n, d);
#endif
  if (shift > 0) {
    R >>= shift;
    delete[] u_shifted;
  }
  if (r != nullptr)  *r = R;
  return n - c;
}



size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d1, limb_t d0) {
  limb_t *u_shifted, u_n=0, c = (limb_t)(u[n-1]<d1 || u[n-1]==d1 && u[n-2]<d0) + 1;
  u8 shift = normalize_divisor (d1, d0);
  if (shift > 0) {
    u_shifted = new limb_t[n];
    u_n = shift_left_short (u_shifted, u, n, shift);
    u = u_shifted;
  }
  limb_t r1, r0;
#ifdef DIV_PREINVERSION
  div_n_by_2 (q, r, u_n, u, n, d1, d0, inv_3_by_2 (d1, d0));
#else
	div_n_by_2 (q, r, u_n, u, n, d1, d0);
#endif
  if (shift > 0) {
    if (r != nullptr)  shift_right_short (r, r, 2, shift);
    delete[] u_shifted;
  }
  return n - c;
}



size_t __fastcall div_qr (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d) {
  if (size_u < size_d)  return 0;
  if (size_d == 1) {
    size_t res = div_qr (q, u, u, size_u, d[0]);
    size_u = 1;
    if (u[0] == 0)  --size_u;
    return res;
  }
  if (size_d == 2) {
    size_t res = div_qr (q, u, u, size_u, d[1], d[0]);
    size_u = 2;
    if (u[1] == 0)  --size_u;
    if (u[0] == 0)  --size_u;
    return res;
  }
  limb_t d1=d[size_d-1], d0=d[size_d-2];
  u8 shift = normalize_divisor (d1, d0);
  size_t k = size_u - size_d;
  limb_t *u_shifted, u_m = 0;
  u_shifted = new limb_t[size_u + 1];
  if (shift > 0) {
    u_m = shift_left_short (u_shifted, u, size_u, shift);
    if (u_m)  u_shifted[size_u++] = u_m;
    limb_t *d_shifted = new limb_t [size_d];
    shift_left_short (d_shifted, d, size_d, shift);
    d = d_shifted;
  }
  else for (size_t i=0; i<size_u; ++i)  u_shifted[i] = u[i];
  size_t size_q = k + 1;
#ifdef DIV_PREINVERSION
  div_m_by_n (q, u_shifted, size_u, d, size_d, d1, d0, inv_3_by_2 (d1, d0));
#else
  div_m_by_n (q, u_shifted, size_u, d, size_d, d1, d0);
#endif
  if (shift > 0) {
    shift_right_short (u, u_shifted, size_u, shift);
    delete[] d;
  } else {
    for (size_t i=0; i<size_u; ++i)  u[i] = u_shifted[i];
  }
  delete[] u_shifted;
  while (size_u > 0 && u[size_u-1] == 0)  --size_u;
  if (q[size_q - 1] == 0)  --size_q;
  return size_q;
}
