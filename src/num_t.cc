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
  size_t i = 0;
  if (str[i]=='+' || str[i]=='-')  ++i;
  while (str[i] == '0')  ++i;	// Remove leading zeroes.
  size_t d=0;	// How many digits do we have?
  while (is_digit(str[i+d], base))  ++d;
  d += (d==0);	// In case of d=0 we will have d=1.
  // !!! N is too big !!!
  const size_t N = (size_t)ceil(log((double)base)/log((double)LIMB_BITS)*d);	// Maximum number of limbs for d-digit number
  limbs = new limb_t[N];
  from_string (str, base);
}



void __fastcall num_t::resize (size_t s) {
  delete[] limbs;
  limbs = new limb_t[s];
  set_zero();
}



void __fastcall num_t::adapt (size_t s) {
  if ((size_t)abs(size) >= s)  return;
  limb_t *data = new limb_t[s];
  copy_up (data, limbs, abs(size));
  delete[] limbs;
  limbs = data;
}



offset_t __fastcall num_t::normalize() {
  bool is_negative = sign()<0;
  size = normalize_size (limbs, abs(size));
  if (is_negative)  size = -size;
  return size;
}



sign_t __fastcall num_t::compare_equal_size (const num_t &that) const {
  assert (size != 0);
  assert (size == that.size);
  size_t i = abs(size);
  while (--i>0 && limbs[i]==that.limbs[i]);
  sign_t res = 0;
  if (limbs[i] < that.limbs[i])  res = -1;
  if (limbs[i] > that.limbs[i])  res = +1;
  if (size < 0)  res = -res;
  return res;
}



sign_t __fastcall num_t::compare (const num_t &that) const {
  if (size < that.size)  return -1;
  if (size > that.size)  return 1;
  if (size == 0)  return 0;
  return compare_equal_size (that);
}



sign_t __fastcall num_t::compare (limb_t s) const {
  assert (is_normalized());
  if (size < 0)  return -1;
  if (size > 1)  return +1;
  if (size == 0) return 0 - (s!=0);
  if (limbs[0] < s)  return -1;
  if (limbs[0] > s)  return +1;
  return 0;
}



sign_t __fastcall num_t::compare (slimb_t s) const {
  assert (is_normalized());
  if (size<0 && s>=0)  return -1;
  if (size>0 && s<=0)  return +1;
  limb_t t = (limb_t)abs(s);
  sign_t res = 0;
  if (abs(size) > 1)  res = +1;
  else if (size == 0) res = 0 - (s!=0);
  else if (limbs[0] < t)  res = -1;
  else if (limbs[0] > t)  res = +1;
  if (s<0)  res = -res;
  return res;
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



const num_t & __fastcall num_t::operator = (i64 a) {
  if (a == 0)  set_zero();
  else {
    vec_t t (*this);
    t = (u64)abs(a);
    //printf ("%llx :: t.size = %u\n", a, t.size);
    if (a < 0)  size = -(offset_t)t.size;
    else  size = (offset_t)t.size;
    t.detach();
  }
  assert (is_normalized());
  return *this;
}



const num_t & __fastcall num_t::operator = (u64 a) {
  if (a == 0)  set_zero();
  else {
    vec_t t (*this);
    t = a;
    size = (offset_t)t.size;
    t.detach();
  }
  assert (is_normalized());
  return *this;
}



num_t & __fastcall num_t::operator = (num_t &&that) {
  if (this == &that)  return *this;
  delete[] limbs;
  size = that.size;
  limbs = that.limbs;
  that.limbs = nullptr;
  return * this;
}



const num_t & num_t::inc (slimb_t s) {
	return ::inc (*this, *this, s);
}



const num_t & num_t::inc (limb_t s) {
	return ::inc (*this, *this, s);
}



const num_t & num_t::add (const num_t &v) {
	return ::add (*this, *this, v);
}



const num_t & num_t::dec (slimb_t s) {
  return ::dec (*this, *this, s);
}



const num_t & num_t::dec (limb_t s) {
  return ::dec (*this, *this, s);
}



const num_t & num_t::sub (const num_t &v) {
  return ::sub (*this, *this, v);
}



const num_t & __fastcall num_t::mul (limb_t v) { return ::mul (*this, *this, v); }

const num_t & __fastcall num_t::mul (slimb_t v) { return ::mul (*this, *this, v); }



const num_t & __fastcall num_t::mul (const num_t &v) {
  num_t u (*this);
  return ::mul (*this, u, v);
}



const num_t & __fastcall num_t::div (limb_t v) { return ::div (*this, *this, v); }
const num_t & __fastcall num_t::div (slimb_t v) { return ::div (*this, *this, v); }



const num_t & __fastcall num_t::mod (slimb_t v, DivisionMode dm) { return ::mod (*this, *this, v, dm); }
const num_t & __fastcall num_t::mod (limb_t v, DivisionMode dm) { return ::mod (*this, *this, v, dm); }



const num_t & __fastcall num_t::div (const num_t &v) {
  num_t u (*this);
  return ::div (*this, u, v);
}



const num_t & __fastcall num_t::mod (const num_t &v, DivisionMode dm) {
  num_t u (*this);
  return ::mod (*this, u, v, dm);
}



const num_t & __fastcall num_t::shift_left (bitcnt_t shift) {
  return ::shift_left (*this, *this, shift);
}



const num_t & __fastcall num_t::shift_right (bitcnt_t shift) {
  return ::shift_right (*this, *this, shift);
}



const num_t & num_t::operator += (slimb_t v) { return inc(v); }
const num_t & num_t::operator += (limb_t v) { return inc(v); }
const num_t & num_t::operator += (const num_t &v) { return add(v); }
const num_t & num_t::operator -= (slimb_t v) { return dec(v); }
const num_t & num_t::operator -= (limb_t v) { return dec(v); }
const num_t & num_t::operator -= (const num_t &v) { return sub(v); }
const num_t & num_t::operator *= (slimb_t v) { return mul(v); }
const num_t & num_t::operator *= (limb_t v) { return mul(v); }
const num_t & num_t::operator *= (const num_t &v) { return mul(v); }
const num_t & num_t::operator /= (slimb_t v) { return div(v); }
const num_t & num_t::operator /= (limb_t v) { return div(v); }
const num_t & num_t::operator /= (const num_t &v) { return div(v); }
const num_t & num_t::operator %= (slimb_t v) { return mod(v); }
const num_t & num_t::operator %= (limb_t v) { return mod(v); }
const num_t & num_t::operator %= (const num_t &v) { return mod(v); }

const num_t & __fastcall num_t::operator <<= (bitcnt_t shift) { return shift_left (shift); }
const num_t & __fastcall num_t::operator >>= (bitcnt_t shift) { return shift_right (shift); }



const num_t & __fastcall num_t::from_string (const char *str, u8 base) {
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



const num_t & __fastcall diff (num_t &z, const vec_t &u, limb_t a) {
  if (a == 0)  return z = u;
  if (u.is_zero()) {
    z.size = -1;
    z.limbs[0] = a;
    return z;
  }
  sign_t s = u.compare(a);
  vec_t Z(z);
  if (s == 0)  z.size = 0;
  else if (s > 0) z = ::dec (Z, u, a);
  else {	// u<a means u[0]<a;
    z.size = -1;
    z.limbs[0] = a - u.limbs[0];
  }
  Z.detach();
  return z;
}

const num_t & __fastcall diff (num_t &z, const vec_t &u, const vec_t &v) {
  sign_t s = u.compare(v);
  vec_t Z(z);
  if (s == 0)  z.size = 0;
  else if (s > 0) {
    ::sub (Z, u, v);
    z.size = (offset_t)Z.size;
  }
  else {
    ::sub (Z, v, u);
    z.size = -(offset_t)Z.size;
  }
  Z.detach();
  return z;
}



const num_t & __fastcall inc (num_t &z, const num_t &u, limb_t s) {
  vec_t Z(z), U(u);
  z.size = u.size<0 ? -::diff (z, U, s).size : (offset_t)::inc (Z, U, s).size;
  U.detach();
  Z.detach();
  return z;
}



const num_t & __fastcall inc (num_t &z, const num_t &u, slimb_t s) {
  return s>=0 ? ::inc (z, u, (limb_t)s) : ::dec (z, u, (limb_t)(-s));
}



const num_t & __fastcall add (num_t &z, const num_t &u, const num_t &v) {
  vec_t U(u), V(v), Z(z);
  if ((u.size ^ v.size) < 0)  ::diff (z, U, V);	// Opposite signs
  else  z.size = (offset_t)::add (Z, U, V).size;
  if (u.size < 0)  z.size = -z.size;
  U.detach();
  V.detach();
  Z.detach();
  return z;
}



const num_t & __fastcall dec (num_t &z, const num_t &u, limb_t s) {
  vec_t Z(z), U(u);
  z.size = u.size<0 ? -(offset_t)::inc (Z, U, s).size : ::diff (z, U, s).size;
  U.detach();
  Z.detach();
  return z;
}



const num_t & __fastcall dec (num_t &z, const num_t &u, slimb_t s) {
  return s>=0 ? ::dec (z, u, (limb_t)s) : ::inc (z, u, (limb_t)(-s));
}



const num_t & __fastcall sub (num_t &z, const num_t &u, const num_t &v) {
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



const num_t & __fastcall mul (num_t &z, const num_t &u, limb_t v) {
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



const num_t & __fastcall mul (num_t &z, const num_t &u, slimb_t v) {
  z.size = v>=0 ? ::mul(z, u, (limb_t)v).size : -::mul(z, u, (limb_t)abs(v)).size;
  return z;
}



const num_t & __fastcall mul (num_t &z, const num_t &u, const num_t &v) {
  vec_t Z(z), U(u), V(v);
  z.size = (offset_t)::mul (Z, U, V).size;
  if ((u.size ^ v.size) < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  V.detach();
  return z;
}



const num_t & __fastcall div (num_t &z, const num_t &u, limb_t v) {
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



const num_t & __fastcall div (num_t &z, const num_t &u, slimb_t v) {
  z.size = v>=0 ? ::div(z, u, (limb_t)v).size : -::div(z, u, (limb_t)abs(v)).size;
  return z;
}



const num_t & __fastcall mod (num_t &z, const num_t &u, limb_t v, DivisionMode dm) {
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



const num_t & __fastcall mod (num_t &z, const num_t &u, slimb_t v, DivisionMode dm) {
  assert (v != 0);
  ::mod (z, u, (limb_t)abs(v), dm);
  if (dm == DM_FLOORED && v < 0 && z.size>0)  ::inc (z, u, v);
  return z;
}



const num_t & __fastcall div (num_t &z, const num_t &u, const num_t &v) {
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



const num_t & __fastcall mod (num_t &z, const num_t &u, const num_t &v, DivisionMode dm) {
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



const num_t & __fastcall shift_left (num_t &z, const num_t &u, bitcnt_t shift) {
  vec_t Z(z), U(u);
  z.size = ::shift_left (Z, U, shift).size;
  if (u.size < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  return z;
}



const num_t & __fastcall shift_right (num_t &z, const num_t &u, bitcnt_t shift) {
  vec_t Z(z), U(u);
  z.size = ::shift_right (Z, U, shift).size;
  if (u.size < 0)  z.size = -z.size;
  Z.detach();
  U.detach();
  return z;
}



const num_t & __fastcall num_t::operator ++ () { return inc ((limb_t)1); }



const num_t & __fastcall num_t::operator -- () { return dec ((limb_t)1); }



const num_t __fastcall operator + (const num_t &u, const num_t &v) {
  num_t z (max (abs(u.size), abs(v.size)) + 1);
  return add (z, u, v);
}



const num_t __fastcall operator + (const num_t &u, slimb_t v) {
  num_t z (abs(u.size) + 1);
  return inc (z, u, v);
}



const num_t __fastcall operator - (const num_t &u, const num_t &v) {
  num_t z (max (abs(u.size), abs(v.size)) + 1);
  return sub (z, u, v);
}



const num_t __fastcall operator - (const num_t &u, slimb_t v) {
  num_t z (abs(u.size)+1);
  return dec (z, u, v);
}



const num_t __fastcall operator * (const num_t &u, const num_t &v) {
  num_t z (abs(u.size)+abs(v.size));
  return mul (z, u, v);
}



const num_t __fastcall operator * (const num_t &u, slimb_t v) {
  num_t z (abs(u.size)+1);
  return mul (z, u, v);
}



const num_t __fastcall operator / (const num_t &u, slimb_t v) {
  num_t z (abs(u.size));
  return div (z, u, v);
}



const num_t __fastcall operator % (const num_t &u, slimb_t v) {
  num_t z(1);
  return mod (z, u, v);
}



const num_t __fastcall operator / (const num_t &u, const num_t &v) {
  num_t z (abs(u.size));
  return div (z, u, v);
}



const num_t __fastcall operator % (const num_t &u, const num_t &v) {
  num_t z (abs(u.size));
  return mod (z, u, v);
}



const num_t __fastcall operator << (const num_t &u, bitcnt_t shift) {
  num_t z (abs(u.size) + (shift+LIMB_BITS-1)/LIMB_BITS);
  shift_left (z, u, shift);
  return z;
}



const num_t __fastcall operator >> (const num_t &u, bitcnt_t shift) {
  num_t z (abs(u.size));
  return shift_right (z, u, shift);
}
