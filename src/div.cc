// Low-level division

#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "misc.h"

#ifndef USE_ASM

#ifdef USE_X64



    // Make double limb (u1, u0) from limbs u1 and u0.
static dlimb_t __fastcall glue (limb_t u1, limb_t u0) {
  return ((dlimb_t)u1<<LIMB_BITS) | u0;
}



limb_t __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0) {
  dlimb_t U = glue (u2, u1);
  dlimb_t D = glue (d1, d0);
  if (u2 == d1) {
    limb_t s1 = d0 - u1;
    dlimb_t S = ((dlimb_t)s1<<LIMB_BITS) - u0;
    if (S <= D) {
      q = LIMB_MAX;
      dlimb_t R = D - S;
      r1 = (limb_t)(R >> LIMB_BITS);
      r0 = (limb_t)R;
    } else {
      q = LIMB_MAX - 1;
      dlimb_t R = 2*D - S;
      r1 = (limb_t)(R >> LIMB_BITS);
      r0 = (limb_t)R;
    }
    return q;
  }
  dlimb_t Q = U / d1;
  dlimb_t DQ = Q * d0;
  dlimb_t R = ((U - Q*d1) << LIMB_BITS) | u0;
  if (R < DQ) {
    --Q;
    R += D;
    if (R>=D && R<DQ) {
      --Q;
      R += D;
    }
  }
  R -= DQ;
  r1 = R >> LIMB_BITS;
  r0 = (limb_t)R;
  q = (limb_t)Q;
  return q;
}



#endif // USE_X64

#ifdef DIV_PREINVERSION

#ifdef USE_X64

limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v) {
  q = (dlimb_t(u1)*v >> LIMB_BITS) + u1;
  dlimb_t R = glue (u1, u0) - dlimb_t(q)*d;
  while (R >= d) {
    ++q;
    R -= d;
  }
  return (limb_t)R;
}


limb_t __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v) {
  q = u2 + limb_t (dlimb_t(u2)*v >> LIMB_BITS);
  dlimb_t qd0 = dlimb_t(q)*d0;
  dlimb_t qd1 = dlimb_t(q)*d1;
  dlimb_t R0 = qd0;
  limb_t R0_lo = limb_t (R0);
  limb_t R0_hi = limb_t (R0 >> LIMB_BITS);
  dlimb_t R = glue (u2, u1) - qd1 - R0_hi;
  R -= (u0 < R0_lo);
  u0 -= R0_lo;
  while (R>d1 || R==d1 && u0>=d0) {
    ++q;
    R -= d1 + (u0 < d0);
    u0 -= d0;
  }
  r0 = u0;
  r1 = (limb_t)R;
  return q;
}



limb_t __fastcall inv_2_by_1 (limb_t d) {
  return limb_t (dlimb_t (-1) / d);
}



limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0) {
  limb_t q, r1, r0;
  return div_3_by_2 (q, r1, r0, LIMB_MAX-d1, LIMB_MAX-d0, LIMB_MAX, d1, d0);
}



#else // USE_X64



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v) {
  limb_t q1, q0;
  mul_limbs (q1, q0, u1, v);
  q0 += u0;
  q1 += u1 + 1 + (q0 < u0);
  limb_t r = u0 - q1 * d;
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


limb_t __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v) {
  limb_t q0, q1, t0, t1;
  mul_limbs (q1, q0, v, u2);
  q0 += u1;
  q1 += u2 + (q0 < u1);
  r1 = u1 - q1*d1;
  mul_limbs (t1, t0, d0, q1);
  t0 += d0;
  t1 += d1 + (t0 < d0);
  r0 = u0 - t0;
  r1 = r1 - t1 - (u0 < t0);
  ++q1;
  if (r1 >= q0) {
    --q1;
    r0 += d0;
    r1 += d1 + (r0 < d0);
  }
  if (r1>d1 || r1==d1 && r0>=d0) {
    ++q1;
    r1 = r1 - d1 - (r0 < d0);
    r0 -= d0;
  }
  q = q1;
  return q1;
}



limb_t __fastcall inv_2_by_1 (limb_t d) {
  limb_t q;
  div_2_by_1 (q, ~d, LIMB_MAX, d);
  return q;
}



limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0) {
  limb_t r1, r0, q;
  return div_3_by_2 (q, r1, r0, ~d1, ~d0, LIMB_MAX, d1, d0);
}



#endif // 'else' for USE_X64


void __fastcall DivInverse::Init (limb_t d) {
  shift = count_lz (d);
  d1 = d << shift;
  Invert1();
}

void __fastcall DivInverse::Init (limb_t dh, limb_t dl) {
  shift = count_lz (dh);
  if (shift > 0) {
    dh = dh<<shift | dl>>(LIMB_BITS - shift);
    dl <<= shift;
  }
  d1 = dh;
  d0 = dl;
  Invert2();
}



void __fastcall DivInverse::Init (limb_t dh, limb_t dl, limb_t tail) {
  shift = count_lz (dh);
  if (shift > 0) {
    dh = dh<<shift | dl>>(LIMB_BITS - shift);
    dl = dl<<shift | tail>>(LIMB_BITS - shift);
  }
  d1 = dh;
  d0 = dl;
  Invert2();
}



limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, const DivInverse &inv) {
  limb_t R = u_n, Q;
  size_t i = n;
  if (q != nullptr)  while (i-- > 0)  R = div_2_by_1 (q[i], R, u[i], inv.d1, inv.v);
  else  while (i-- > 0)  R = div_2_by_1 (Q, R, u[i], inv.d1, inv.v);
  return R;
}



void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, const DivInverse & inv) {
  limb_t R1 = u_n, R0 = u[n-1], Q;
  size_t i = n - 1;
  if (q != nullptr)  while (i-- > 0)  div_3_by_2 (q[i], R1, R0, R1, R0, u[i], inv.d1, inv.d0, inv.v);
  else  while (i-- > 0)  div_3_by_2 (Q, R1, R0, R1, R0, u[i], inv.d1, inv.d0, inv.v);
  if (r != nullptr) {
    r[0] = R0;
    r[1] = R1;
  }
}



void __fastcall div_m_by_n (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d, const DivInverse & inv) {
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
    if (u2==inv.d1 && u1==inv.d0)  Q = LIMB_MAX;
    else  div_3_by_2 (Q, R1, R0, u2, u1, u0, inv.d1, inv.d0, inv.v);
    limb_t borrow = submul_N_by_1 (u + k, u + k, d, size_d, Q);  // u -= Q*d*beta^k
    if (borrow > u2) {  // Если вычли слишком много (заём больше, чем u2, который должен уничтожиться).
      --Q;
      add (u + k, u + k, d, size_d); // u += d*beta^k
    }
    u[size_d + k] = 0; // Уничтожаем старший лимб.
    if (q != nullptr)  q[k] = Q;
  }
}


#else // DIV_PREINVERSION

#ifdef USE_X64



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d) {
  dlimb_t U = glue (u1, u0);
  q = (limb_t)(U/d);
  return (limb_t)(U - (dlimb_t)q*d);
}



#else // USE_X64



limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d) {
  const size_t HLIMB_BITS = LIMB_BITS/2;
  hlimb_t u0h, u0l, dh, dl;
  u0h = (hlimb_t)(u0 >> HLIMB_BITS);
  u0l = (hlimb_t)u0;
  dh = (hlimb_t)(d >> HLIMB_BITS);
  dl = (hlimb_t)d;
  // Делим первые три полулимба (u1, u0h) на знаменатель
  limb_t qh = u1 / dh;
  limb_t dqh = qh * dl;
  limb_t R = ((u1 - qh*dh) << HLIMB_BITS) | u0h;
  if (R < dqh) {
    --qh;
    R += d;
    if (R >= d && R < dqh) {
      --qh;
      R += d;
    }
  }
  R -= dqh;
  q = (qh << HLIMB_BITS); // High part of quotient
  // Divide last three half-limbs (R, u0l) by divisor.
  limb_t ql = R / dh;
  limb_t dql = ql * dl;
  R = ((R - ql*dh) << HLIMB_BITS) | u0l;
  if (R < dql) {
    --ql;
    R += d;
    if (R >= d && R < dql) {
      --ql;
      R += d;
    }
  }
  R -= dql;
  q |= ql;  // Low half of the answer.
  return R;
}



limb_t __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0) {
  if (u2 == d1) {
    limb_t s1 = d0 - u1 - (u0 != 0);
    limb_t s0 = -u0;
    if (s1 < d1 || s1 == d1 && s0 <= d0) {
      q = LIMB_T_MAX;
      r1 = d1 - s1 - (d0 < s0);
      r0 = d0 - s0;
    } else {
      q = LIMB_T_MAX - 1;
      r1 = d1 - s1 - (d0 < s0);
      r0 = d0 - s0;
      r0 += d0;
      r1 += d1 + (r0 < d0);
    }
    return q;
  }
  r1 = div_2_by_1 (q, u2, u1, d1);
  r0 = u0;
  limb_t DQ0, DQ1;
  mul_limbs (DQ1, DQ0, q, d0);
  if (r1<DQ1 || r1==DQ1 && r0<DQ0) {
    --q;
    r0 += d0;
    r1 += d1 + (r0 < d0);
    if (r1 > d1 && (r1 < DQ1 || r1 == DQ1 && r0 < DQ0)) {
      --q;
      r0 += d0;
      r1 += d1 + (r0 < d0);
    }
  }
  r1 -= DQ1 + (r0 < DQ0);
  r0 -= DQ0;
  return q;
}



#endif // 'else' for USE_X64

#endif // 'else' for DIV_PREINVERSION


/*
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, const DivInverse &inv) {
  limb_t R = u_n, Q;
  size_t i = n;
  if ( q != nullptr )  while (i -- > 0)  R = div_2_by_1 ( q [ i ], R, u [ i ], inv . d1 );
  else
    while ( i -- > 0 ) R = div_2_by_1 ( Q, R, u [ i ], inv . d1 );
  return R;
}

void __fastcall div_n_by_2 ( limb_t * q, limb_t * r, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
  limb_t R1 = u_n, R0 = u [ n - 1 ], Q;
  size_t i = n - 1;
  if ( q != nullptr )
    while ( i -- > 0 ) div_3_by_2 ( q [ i ], R1, R0, R1, R0, u [ i ], inv . d1, inv . d0 );
  else
    while ( i -- > 0 ) div_3_by_2 ( Q, R1, R0, R1, R0, u [ i ], inv . d1, inv . d0 );
  if ( r != nullptr ) {
    r [ 0 ] = R0;
    r [ 1 ] = R1;
  }
}
*/


#endif // USE_ASM



size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d) {
  DivInverse inv(d);
  limb_t *u_shifted, u_n=0, c=(limb_t)(u[n-1]<d);
  if (inv.shift > 0) {
    u_shifted = new limb_t[n];
    u_n = shift_left_short (u_shifted, u, n, inv.shift);
    u = u_shifted;
  }
  limb_t R = div_n_by_1 (q, u_n, u, n, inv);
  if (inv.shift > 0) {
    R >>= inv.shift;
    delete[] u_shifted;
  }
  if (r != nullptr)  *r = R;
  return n - c;
}


size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d1, limb_t d0) {
  DivInverse inv (d1, d0);
  limb_t *u_shifted, u_n=0, c = (limb_t)(u[n-1]<d1 || u[n-1]==d1 && u[n-2]<d0) + 1;
  if (inv.shift > 0) {
    u_shifted = new limb_t[n];
    u_n = shift_left_short (u_shifted, u, n, inv.shift);
    u = u_shifted;
  }
  limb_t R1, R0;
  div_n_by_2 (q, r, u_n, u, n, inv);
  if (inv . shift > 0) {
    if (r != nullptr)  shift_right_short (r, r, 2, inv.shift);
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

  DivInverse inv (d, size_d);
  size_t k = size_u - size_d;
  limb_t *u_shifted, u_m = 0;
  u_shifted = new limb_t[size_u + 1];
  if (inv.shift > 0) {
    u_m = shift_left_short (u_shifted, u, size_u, inv.shift);
    if (u_m)  u_shifted[size_u++] = u_m;
    limb_t *d_shifted = new limb_t [size_d];
    shift_left_short (d_shifted, d, size_d, inv.shift);
    d = d_shifted;
  }
  else for (size_t i=0; i<size_u; ++i)  u_shifted[i] = u[i];
  size_t size_q = k + 1;
  div_m_by_n (q, u_shifted, size_u, d, size_d, inv);
  if (inv . shift > 0) {
    shift_right_short (u, u_shifted, size_u, inv.shift);
    delete[] d;
  } else {
    for (size_t i=0; i<size_u; ++i)  u[i] = u_shifted[i];
  }
  delete[] u_shifted;
  while (size_u > 0 && u[size_u-1] == 0)  --size_u;
  if (q[size_q - 1] == 0)  --size_q;
  return size_q;
}
