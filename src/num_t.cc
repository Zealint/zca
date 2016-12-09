// Implementation of class 'num_t'

#include "num_t.h"
#include "misc.h"
#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include <stdio.h>



num_t::num_t (const char *str, u8 base) {
  const size_t D = how_many_digits (str, base);
  const size_t N = how_many_limbs (D, base);
  limbs = new limb_t[N];
  from_string (str, base);
  assert (abs(N-abs_size()) <= 1);
}



void __fastcall num_t::resize (size_t s) {
  delete[] limbs;
  limbs = new limb_t[s];
  set_zero();
}



void __fastcall num_t::adapt (size_t s) {
  size_t abs_size = (size_t)abs(size);
  if (abs_size >= s)  return;
  limb_t *data = new limb_t[s];
  if (abs_size > 0)  copy_up (data, limbs, abs_size);
  delete[] limbs;
  limbs = data;
}



num_t & __fastcall num_t::clear() {
  delete[] limbs;
  limbs = nullptr;
  size = 0;
  return *this;
}



offset_t __fastcall num_t::normalize() {
  bool is_negative = size<0;
  size = (offset_t)normalize_size (limbs, (size_t)abs(size));
  if (is_negative)  size = -size;
  return size;
}



sign_t __fastcall num_t::compare_equal_size (const num_t &that) const {
  assert (size != 0);
  assert (size == that.size);
  assert (is_normalized() && that.is_normalized());
  size_t i = abs(size);
  while (--i>0 && limbs[i]==that.limbs[i]);
  sign_t res = 0;
  if (limbs[i] < that.limbs[i])  res = -1;
  if (limbs[i] > that.limbs[i])  res = +1;
  if (size < 0)  res = -res;
  return res;
}



sign_t __fastcall num_t::compare (const num_t &that) const {
  assert (is_normalized() && that.is_normalized());
  if (size < that.size)  return -1;
  if (size > that.size)  return 1;
  if (size == 0)  return 0;
  return compare_equal_size (that);
}



num_t & __fastcall num_t::operator = (const num_t &that) {
  if (this == &that)  return *this;
  size = that.size;
  if (size != 0) copy_up (limbs, that.limbs, (size_t)abs(size));
  return *this;
}



num_t & __fastcall num_t::operator = (const vec_t &that) {
  size = (offset_t)that.size;
  if (limbs!=that.limbs && size!=0)  copy_up (limbs, that.limbs, that.size);
  return *this;
}



num_t & __fastcall num_t::from_string (const char *str, u8 base) {
  bool is_negative = false;
  if (*str == '+')  ++str;
  else if (*str == '-') { is_negative = true; ++str; }
  size = ::from_string (limbs, str, base);
  if (is_negative)  size = -size;
  return *this;
}



char * __fastcall num_t::to_string (char *str, u8 base) const {
  if (size < 0) *str = '-';
  ::to_string (str+(size<0), limbs, abs(size), base);
  return str;
}



num_t & __fastcall diff (num_t &c, const vec_t &a, const vec_t &b) {
  sign_t s = a.compare(b);
  vec_t C(c);
  if (s == 0)  c.size = 0;
  else if (s > 0) {
    ::sub (C, a, b);
    c.size = (offset_t)C.size;
  }
  else {
    ::sub (C, a, b);
    c.size = -(offset_t)C.size;
  }
  C.detach();
  return c;
}



num_t & __fastcall add (num_t &z, const num_t &u, const num_t &v) {
  vec_t U(u), V(v), Z(z);
  if ((u.size ^ v.size) < 0)  ::diff (z, U, V);	// Opposite signs
  else  z.size = (offset_t)::add (Z, U, V).size;
  if (u.size < 0)  z.size = -z.size;
  U.detach();
  V.detach();
  Z.detach();
  return z;
}



num_t & __fastcall sub (num_t &z, const num_t &u, const num_t &v) {
  vec_t U(u), V(v), Z(z);
  sign_t sign_u = u.sign();
  if ((u.size ^ v.size) < 0)  z.size = (offset_t)::add (Z, U, V).size;	// Opposite signs
  else  ::diff (z, U, V);
  if (sign_u < 0)  z.size = -z.size;
  U.detach();
  V.detach();
  Z.detach();
  return z;
}



num_t & __fastcall mul (num_t &z, const num_t &u, limb_t v) {
  if (u.size==0 || v==0)  z.set_zero();
  else if (v==1)  z = u;
  else {
    vec_t Z(z), U(u);
    z.size = (offset_t)::mul (Z, U, v).size;
    if (u.size < 0)  z.size = -z.size;
    Z.detach();
    U.detach();
  }
  return z;
}



num_t & __fastcall mul (num_t &z, const num_t &u, slimb_t v) {
  z.size = v>=0 ? ::mul(z, u, (limb_t)v).size : -::mul(z, u, (limb_t)abs(v)).size;
  return z;
}



num_t & __fastcall mul (num_t &z, const num_t &u, const num_t &v) {
  vec_t Z(z), U(u), V(v);
  z.size = (offset_t)::mul (Z, U, V).size;
  if ((u.size ^ v.size) < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  V.detach();
  return z;
}



num_t & __fastcall div (num_t &z, const num_t &u, limb_t v) {
  assert (v != 0);
  if (u.size == 0)  z.set_zero();
  else {
    limb_t r;
    sign_t sign_u = u.sign();
    z.size = (offset_t)::div_qr (z.limbs, &r, u.limbs, abs(u.size), v);
    if (sign_u < 0) {
      z.size = -z.size;
      if (r > 0)  --z;
    }
  }
  return z;
}



num_t & __fastcall div (num_t &z, const num_t &u, slimb_t v) {
  z.size = v>=0 ? ::div(z, u, (limb_t)v).size : -::div(z, u, (limb_t)abs(v)).size;
  return z;
}



num_t & __fastcall mod (num_t &z, const num_t &u, limb_t v, DivisionMode dm) {
  assert (v != 0);
  vec_t U(u);
  z[0] = ::mod (U, v);
  z.size = u.size<0 ? -1 : 1;
  z.normalize();
  if (dm == DM_EUCLIDEAN && z.size < 0)  z.inc(v);
  if (dm == DM_TRUNCATED && u.size < 0 && z.size > 0)  z.dec(v);
  U.detach();
  return z;
}



num_t & __fastcall mod (num_t &z, const num_t &u, slimb_t v, DivisionMode dm) {
  assert (v != 0);
  ::mod (z, u, (limb_t)abs(v), dm);
  if (dm == DM_FLOORED && v < 0 && z.size>0)  ::inc (z, u, v);
  return z;
}



num_t & __fastcall div (num_t &z, const num_t &u, const num_t &v) {
  assert (!v.is_zero());
  if (u.size == 0)  z.set_zero();
  else {
    num_t r(u);
    size_t size_r = abs(r.size);
    z.size = (offset_t)::div_qr (z.limbs, r.limbs, size_r, v.limbs, abs(v.size));
    if ((u.size ^ v.size) < 0) {
      z.size = -z.size;
      if (size_r > 0)  --z;
    }
  }
  return z;
}



num_t & __fastcall mod (num_t &z, const num_t &u, const num_t &v, DivisionMode dm) {
  assert (!v.is_zero());
  if (u.size == 0)  z.set_zero();
  else {
    num_t r(u);
    size_t size_r = abs (r.size);
    num_t tmp(u);	// !!! nullptr doesn't work.
    ::div_qr (tmp.limbs, r.limbs, size_r, v.limbs, abs(v.size));
    z.size = (offset_t)size_r;
    if (size_r > 0)  copy_up (z.limbs, r.limbs, size_r);
    if (dm == DM_EUCLIDEAN && u.size < 0 && v.size < 0)  z += v;
    if (dm == DM_EUCLIDEAN && u.size < 0 && v.size > 0)  ::sub (z, v, z);
    if (dm == DM_TRUNCATED && u.size < 0 && v.size < 0)  z.neg();
    if (dm == DM_TRUNCATED && u.size < 0 && v.size > 0)  z.neg();
    if (dm == DM_FLOORED && u.size > 0 && v.size < 0) { z += v;  z.neg(); }
    if (dm == DM_FLOORED && u.size < 0 && v.size > 0) { z -= v;  z.neg(); }
    if (dm == DM_FLOORED && u.size < 0 && v.size < 0) { z.neg(); }
  }
  return z;
}



num_t & __fastcall shift_left (num_t &z, const num_t &u, bitcnt_t shift) {
  vec_t Z(z), U(u);
  z.size = ::shift_left (Z, U, shift).size;
  if (u.size < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  return z;
}



num_t & __fastcall shift_right (num_t &z, const num_t &u, bitcnt_t shift) {
  vec_t Z(z), U(u);
  z.size = ::shift_right (Z, U, shift).size;
  if (u.size < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  return z;
}
