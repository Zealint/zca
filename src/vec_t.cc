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
  size = ::mul (*this, *this, v);
  return *this;
}



const vec_t & __fastcall vec_t::mul (const vec_t &v) {
  vec_t u (*this);
  size = ::mul (*this, u, v);
  return *this;
}



const vec_t & __fastcall vec_t::add_mul (const vec_t &v, limb_t w) {
  size = ::add_mul (*this, *this, v, w);
  return *this;
}



const vec_t & __fastcall vec_t::sub_mul (const vec_t &v, limb_t w) {
  assert (::sub_mul (*this, *this, v, w));
  normalize();
  return *this;
}



const vec_t & __fastcall vec_t::div (limb_t v) {
  size = ::div (*this, *this, v);
  return *this;
}



const vec_t & __fastcall vec_t::mod (limb_t v) {
  size = ::Mod (*this, *this, v);
  return *this;
}



const vec_t & __fastcall vec_t::div (const vec_t &v) {
  vec_t u (*this);
  size = ::Div (*this, u, v);
  return *this;
}



const vec_t & __fastcall vec_t::mod (const vec_t &v) {
  vec_t u (*this);
  size = ::mod (*this, u, v);
  return *this;
}



const vec_t & __fastcall vec_t::shift_left (bitcnt_t shift) {
  size = ::shift_left (*this, *this, shift);
  return *this;
}



const vec_t & __fastcall vec_t::shift_right (bitcnt_t shift) {
  size = ::shift_right (*this, *this, shift);
  return *this;
}



const vec_t & vec_t::operator += (limb_t v) {
  size = ::inc(v);
  return *this;
}



const vec_t & vec_t::operator += (const vec_t &v) {
  size = add(v);
  return *this;
}



const vec_t & vec_t::operator -= (const vec_t &v) {
  assert (::sub(v));
  normalize();
  return *this;
}



const vec_t & vec_t::operator -= (limb_t v) {
  assert (::dec(v));
  return *this;
}



const vec_t & vec_t::operator *= (limb_t v) {
  size = ::mul(v);
  return *this;
}



const vec_t & vec_t::operator *= (const vec_t &v) {
  size = ::mul(v);
  return *this;
}



const vec_t & vec_t::operator /= (limb_t v) {
  size = ::div(v);
  return *this;
}



const vec_t & vec_t::operator %= (limb_t v) {
  size = mod(v);
  return *this;
}



const vec_t & vec_t::operator /= (const vec_t &v) {
  size = ::div(v);
  return *this;
}



const vec_t & vec_t::operator %= (const vec_t &v) {
  size = ::mod(v);
  return *this;
}



const vec_t & __fastcall vec_t::operator <<= (bitcnt_t shift) {
  size = ::shift_left (shift);
  return *this;
}



const vec_t & __fastcall vec_t::operator >>= (bitcnt_t shift) {
  size = ::shift_right (shift);
  return *this;
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
  z.zormalize();
  return z;
}


???
const vec_t & __fastcall mul (vec_t &z, const vec_t &u, limb_t v) {
  if (u.size==0 || v==0)  z.set_zero();
  else if (v==1) { if (&z != &u)  z = u; }
  else z.size = ::mul (z.limbs, u.limbs, u.size, v);
  return z;
}



const vec_t & __fastcall AddMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (u.size == 0)  return Mul1 (z, v, w);
  if (w == 1)  return Add (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    z.size = ::addmul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
  }
  return z;
}



const vec_t & __fastcall SubMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (w == 1)  return Sub (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    bool is_correct = ::submul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
    assert (is_correct);
    z.size = u.size;
    z.Normalize();
  }
  return z;
}



const vec_t & __fastcall Mul (vec_t &z, const vec_t &u, const vec_t &v) {
  if (u.size==0 || v.size==0)  z.SetZero();
  else if (u.size == 1)  return Mul1 (z, v, u[0]);
  else if (v.size == 1)  return Mul1 (z, u, v[0]);
  else z.size = ::mul (z.limbs, u.limbs, u.size, v.limbs, v.size);
  return z;
}



const vec_t & Div1 (vec_t &z, const vec_t &u, limb_t v) {
  assert (v!=0);
  if (u.size==0)  z.SetZero();
  else if (v==1) { if (&z != &u)  z = u; }
  else z.size = ::div_qr (z.limbs, nullptr, u.limbs, u.size, v);
  return z;
}



const vec_t & Mod1 (vec_t &z, const vec_t &u, limb_t v) {
  assert (v!=0);
  if (u.size==0 || v==1)  z.SetZero();
  else {
    ::div_qr (nullptr, z.limbs, u.limbs, u.size, v);
    z.size = (size_t)(z[0]!=0);
  }
  return z;
}



const vec_t & __fastcall Div (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.IsZero());
  if (u.size==0)  z.SetZero();
  else {
    vec_t r(u);
    z.size = ::div_qr (z.limbs, r.limbs, r.size, v.limbs, v.size);
  }
  return z;
}



const vec_t & __fastcall Mod (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.IsZero());
  if (u.size==0)  z.SetZero();
  else {
    z = u;
    vec_t tmp(u); // !!! nullptr doesn't work.
    ::div_qr (tmp.limbs, z.limbs, z.size, v.limbs, v.size);
  }
  return z;
}



const vec_t & __fastcall ShiftLeft (vec_t &z, const vec_t &u, bitcnt_t shift) {
  if (u.size == 0)  z.SetZero();
  else  z.size = ::shift_left_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall ShiftRight (vec_t &z, const vec_t &u, bitcnt_t shift) {
  if (u.size == 0)  z.SetZero();
  else  z.size = ::shift_right_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall vec_t::operator ++ () {
  return Inc(1);
}



const vec_t & __fastcall vec_t::operator -- () {
  return Dec(1);
}



const vec_t __fastcall operator + (const vec_t &u, const vec_t &v) {
  vec_t z (max (u.size, v.size) + 1);
  return Add (z, u, v);
}



const vec_t __fastcall operator + (const vec_t &u, limb_t v) {
  vec_t z (u.size + 1);
  return Inc (z, u, v);
}



const vec_t __fastcall operator - (const vec_t &u, const vec_t &v) {
  assert (u.size >= v.size);
  vec_t z (u.size);
  return Sub (z, u, v);
}



const vec_t __fastcall operator - (const vec_t &u, limb_t v) {
  assert (v==0||u.size>0);
  vec_t z (u.size);
  return Dec (z, u, v);
}



const vec_t __fastcall operator * (const vec_t &u, const vec_t &v) {
  vec_t z (u.size+v.size);
  return Mul (z, u, v);
}



const vec_t __fastcall operator * (const vec_t &u, limb_t v) {
  vec_t z (u.size+1);
  return Mul1 (z, u, v);
}



const vec_t __fastcall operator / (const vec_t &u, limb_t v) {
  vec_t z (u.size);
  return Div1 (z, u, v);
}



const vec_t __fastcall operator % (const vec_t &u, limb_t v) {
  vec_t z (1);
  return Mod1 (z, u, v);
}



const vec_t __fastcall operator / (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  return Div (z, u, v);
}



const vec_t __fastcall operator % (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  return Mod (z, u, v);
}



const vec_t __fastcall operator << (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + (shift+LIMB_BITS-1)/LIMB_BITS);
  return ShiftLeft (z, u, shift);
}



const vec_t __fastcall operator >> (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + shift/LIMB_BITS);
  return ShiftRight (z, u, shift);
}
