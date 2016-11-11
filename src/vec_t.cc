// Implementation of class 'vec_t'

#include "vec_t.h"
#include "misc.h"
#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include <stdio.h>



void __fastcall vec_t::resize (size_t s) {
  delete[] limbs;
  limbs = new limb_t[s];
  set_zero();
}



void __fastcall vec_t::adapt (size_t s) {
  if (size >= s)  return;
  limb_t *data = new limb_t[s];
  copy_up (data, limbs, size);
  delete[] limbs;
  limbs = data;
}



size_t __fastcall vec_t::normalize() {
  return size = normalize_size (limbs, size);
}



sign_t __fastcall vec_t::compare_equal_size (const vec_t &that) const {
  assert (size > 0);
  assert (size == that.size);
  size_t i = size;
  while (--i>0 && limbs[i]==that.limbs[i]);
  if (limbs[i] < that.limbs[i])  return -1;
  if (limbs[i] > that.limbs[i])  return 1;
  return 0;
}



sign_t __fastcall vec_t::compare (const vec_t &that) const {
  if (size < that.size)  return -1;
  if (size > that.size)  return 1;
  if (size == 0)  return 0;
  return compare_equal_size (that);
}



vec_t & __fastcall vec_t::operator = (const vec_t &that) {
  if (this == &that)  return *this;
  size = that.size;
  copy_up (limbs, that.limbs, size);
  return *this;
}



vec_t & __fastcall vec_t::operator = (vec_t &&that) {
  if (this == &that)  return *this;
  delete[] limbs;
  size = that.size;
  limbs = that.limbs;
  that.limbs = nullptr;
  return * this;
}



const vec_t & vec_t::inc (limb_t s) {
	size = ::inc (limbs, size, s);
	return *this;
}



const vec_t & vec_t::add (const vec_t &v) {
	size = ::add (limbs, size, v.limbs, v.size);
	return *this;
}



const vec_t & vec_t::dec (limb_t s) {
  assert (::dec (limbs, size, s));
  normalize();
  return *this;
}



const vec_t & vec_t::sub (const vec_t &v) {
  assert (::sub (limbs, size, v.limbs, v.size));
  normalize();
  return *this;
}



const vec_t & __fastcall vec_t::mul (limb_t v) {
  return ::mul (*this, *this, v);
}



const vec_t & __fastcall vec_t::mul (const vec_t &v) {
  vec_t u (*this);
  return ::mul (*this, u, v);
}



const vec_t & __fastcall vec_t::addmul (const vec_t &v, limb_t w) {
  return ::addmul (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::submul (const vec_t &v, limb_t w) {
  return ::submul (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::div (limb_t v) {
  return ::div (*this, *this, v);
}



const vec_t & __fastcall vec_t::mod (limb_t v) {
  return ::mod (*this, *this, v);
}



const vec_t & __fastcall vec_t::div (const vec_t &v) {
  vec_t u (*this);
  return ::div (*this, u, v);
}



const vec_t & __fastcall vec_t::mod (const vec_t &v) {
  vec_t u (*this);
  return ::mod (*this, u, v);
}



const vec_t & __fastcall vec_t::shift_left (bitcnt_t shift) {
  return ::shift_left (*this, *this, shift);
}



const vec_t & __fastcall vec_t::shift_right (bitcnt_t shift) {
  return ::shift_right (*this, *this, shift);
}



const vec_t & vec_t::operator += (limb_t v) {
  return inc(v);
}



const vec_t & vec_t::operator += (const vec_t &v) {
  return add(v);
}



const vec_t & vec_t::operator -= (const vec_t &v) {
  return sub(v);
}



const vec_t & vec_t::operator -= (limb_t v) {
  return dec(v);
}



const vec_t & vec_t::operator *= (limb_t v) {
  return mul(v);
}



const vec_t & vec_t::operator *= (const vec_t &v) {
  return mul(v);
}



const vec_t & vec_t::operator /= (limb_t v) {
  return div(v);
}



const vec_t & vec_t::operator %= (limb_t v) {
  return mod(v);
}



const vec_t & vec_t::operator /= (const vec_t &v) {
  return div(v);
}



const vec_t & vec_t::operator %= (const vec_t &v) {
  return mod(v);
}



const vec_t & __fastcall vec_t::operator <<= (bitcnt_t shift) {
  return shift_left (shift);
}



const vec_t & __fastcall vec_t::operator >>= (bitcnt_t shift) {
  return shift_right (shift);
}



const vec_t & __fastcall inc (vec_t &z, const vec_t &u, limb_t s) {
  if (s==0)  return z = u;
  if (z.limbs == u.limbs)  z.size = ::inc (u.limbs, u.size, s); // In-place (u+=s).
  else  z.size = ::inc (z.limbs, u.limbs, u.size, s); // Not in-place (z=u+s).
  return z;
}



const vec_t & __fastcall add (vec_t &z, const vec_t &u, const vec_t &v) {
  if (z.limbs == u.limbs)  z.size = ::add (z.limbs, u.size, v.limbs, v.size); // In-place (u+=v).
  else if (z.limbs == v.limbs )  z.size = ::add (z.limbs, v.size, u.limbs, u.size);  // In-place (v+=u).
  else  z.size = ::add (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place (z=u+v).
  return z;
}



const vec_t & __fastcall dec (vec_t &z, const vec_t &u, limb_t s) {
  if (s == 0)  return z = u;
  bool is_correct;
  if (z.limbs == u.limbs)  is_correct = ::dec (u.limbs, u.size, s); // In-place.
  else  is_correct = ::dec (z.limbs, u.limbs, u.size, s); // Not in-place.
  assert (is_correct);
  z.size = u.size;
  z.normalize();
  return z;
}



const vec_t & __fastcall sub (vec_t &z, const vec_t &u, const vec_t &v) {
  bool is_correct;
  if (z.limbs == u.limbs)  is_correct = ::sub (z.limbs, u.size, v.limbs, v.size); // In-place.
  else  is_correct = ::sub (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place.
  assert (is_correct);
  z.size = u.size;
  z.normalize();
  return z;
}



const vec_t & __fastcall mul (vec_t &z, const vec_t &u, limb_t v) {
  if (u.size==0 || v==0)  z.set_zero();
  else if (v==1) { if (&z != &u)  z = u; }
  else z.size = ::mul (z.limbs, u.limbs, u.size, v);
  return z;
}



const vec_t & __fastcall addmul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (u.size == 0)  return mul (z, v, w);
  if (w == 1)  return add (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    z.size = ::addmul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
  }
  return z;
}



const vec_t & __fastcall submul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (w == 1)  return sub (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    assert (::submul (z.limbs, u.limbs, u.size, v.limbs, v.size, w));
    z.size = u.size;
    z.normalize();
  }
  return z;
}



const vec_t & __fastcall mul (vec_t &z, const vec_t &u, const vec_t &v) {
  if (u.size==0 || v.size==0)  z.set_zero();
  else if (u.size == 1)  return mul (z, v, u[0]);
  else if (v.size == 1)  return mul (z, u, v[0]);
  else z.size = ::mul (z.limbs, u.limbs, u.size, v.limbs, v.size);
  return z;
}



const vec_t & __fastcall div (vec_t &z, const vec_t &u, limb_t v) {
  assert (v !=0 );
  if (u.size == 0)  z.set_zero();
  else if (v == 1) { if (&z != &u)  z = u; }
  else z.size = ::div_qr (z.limbs, nullptr, u.limbs, u.size, v);
  return z;
}



const vec_t & __fastcall mod (vec_t &z, const vec_t &u, limb_t v) {
  assert (v != 0);
  if (u.size==0 || v==1)  z.set_zero();
  else {
    ::div_qr (nullptr, z.limbs, u.limbs, u.size, v);
    z.size = (size_t)(z[0]!=0);
  }
  return z;
}



const vec_t & __fastcall div (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.is_zero());
  if (u.size == 0)  z.set_zero();
  else {
    vec_t r(u);
    z.size = ::div_qr (z.limbs, r.limbs, r.size, v.limbs, v.size);
  }
  return z;
}



const vec_t & __fastcall mod (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.is_zero());
  if (u.size == 0)  z.set_zero();
  else {
    z = u;
    vec_t tmp(u); // !!! nullptr doesn't work.
    ::div_qr (tmp.limbs, z.limbs, z.size, v.limbs, v.size);
  }
  return z;
}



const vec_t & __fastcall shift_left (vec_t &z, const vec_t &u, bitcnt_t shift) {
  if (u.size == 0)  z.set_zero();
  else  z.size = ::shift_left_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall shift_right (vec_t &z, const vec_t &u, bitcnt_t shift) {
  if (u.size == 0)  z.set_zero();
  else  z.size = ::shift_right_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall vec_t::operator ++ () {
  return inc(1);
}



const vec_t & __fastcall vec_t::operator -- () {
  return dec(1);
}



const vec_t __fastcall operator + (const vec_t &u, const vec_t &v) {
  vec_t z (max (u.size, v.size) + 1);
  return add (z, u, v);
}



const vec_t __fastcall operator + (const vec_t &u, limb_t v) {
  vec_t z (u.size + 1);
  return inc (z, u, v);
}



const vec_t __fastcall operator - (const vec_t &u, const vec_t &v) {
  assert (u.size >= v.size);
  vec_t z (u.size);
  return sub (z, u, v);
}



const vec_t __fastcall operator - (const vec_t &u, limb_t v) {
  assert (v==0 || u.size>0);
  vec_t z (u.size);
  return dec (z, u, v);
}



const vec_t __fastcall operator * (const vec_t &u, const vec_t &v) {
  vec_t z (u.size+v.size);
  return mul (z, u, v);
}



const vec_t __fastcall operator * (const vec_t &u, limb_t v) {
  vec_t z (u.size+1);
  return mul (z, u, v);
}



const vec_t __fastcall operator / (const vec_t &u, limb_t v) {
  vec_t z (u.size);
  return div (z, u, v);
}



const vec_t __fastcall operator % (const vec_t &u, limb_t v) {
  vec_t z(1);
  return mod (z, u, v);
}



const vec_t __fastcall operator / (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  return div (z, u, v);
}



const vec_t __fastcall operator % (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  return mod (z, u, v);
}



const vec_t __fastcall operator << (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + (shift+LIMB_BITS-1)/LIMB_BITS);
  return shift_left (z, u, shift);
}



const vec_t __fastcall operator >> (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + shift/LIMB_BITS);
  return shift_right (z, u, shift);
}
