// Implementation of class 'vec_t'

#include <assert.h>
#include "vec_t.h"
#include "misc.h"
#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include <stdio.h>



void __fastcall vec_t::Resize (size_t s) {
  delete[] limbs;
  limbs = new limb_t[s];
  SetZero();
}



void __fastcall vec_t::Adapt (size_t s) {
  if (size >= s)  return;
  limb_t *data = new limb_t[s];
  for (size_t i=0; i<size; ++i)  data[i] = limbs[i];
  delete[] limbs;
  limbs = data;
}



size_t __fastcall vec_t::Normalize() {
  while (size>0 && limbs[size-1]==0)  --size;
  return size;
}



sign_t __fastcall vec_t::CompareEqualSize (const vec_t &that) const {
  assert (size>0);
  size_t i = size;
  while (--i>0 && limbs[i]==that.limbs[i]);
  if (limbs[i] < that.limbs[i])  return -1;
  if (limbs[i] > that.limbs[i])  return 1;
  return 0;
}



sign_t __fastcall vec_t::Compare (const vec_t &that) const {
  if (size < that.size)  return -1;
  if (size > that.size)  return 1;
  if (size == 0)  return 0;
  return CompareEqualSize (that);
}



vec_t & __fastcall vec_t::operator = (const vec_t &that) {
  if (this == &that)  return *this;
  size = that.size;
  for (size_t i=0; i<size; ++i)  limbs[i] = that.limbs[i];
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



const vec_t & vec_t::Inc (limb_t s) {
	size = ::inc (limbs, size, s);
	return *this;
}



const vec_t & vec_t::Add (const vec_t &v) {
	size = ::add (limbs, size, v.limbs, v.size);
	return *this;
}



const vec_t & vec_t::Dec (limb_t s) {
	bool is_correct (::dec (limbs, size, s));
  assert (is_correct);
  Normalize();
  return *this;
}



const vec_t & vec_t::Sub (const vec_t &v) {
	bool is_correct (::sub (limbs, size, v.limbs, v.size));
  assert (is_correct);
  Normalize();
  return *this;
}



const vec_t & __fastcall vec_t::Mul1 (limb_t v) {
  return ::Mul1 (*this, *this, v);
}



const vec_t & __fastcall vec_t::Mul (const vec_t &v) {
  vec_t u(*this);
  return ::Mul (*this, u, v);
}



const vec_t & __fastcall vec_t::AddMul1 (const vec_t &v, limb_t w) {
  return ::AddMul1 (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::SubMul1 (const vec_t &v, limb_t w) {
  return ::SubMul1 (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::Div1 (limb_t v) {
  return ::Div1 (*this, *this, v);
}



const vec_t & __fastcall vec_t::Mod1 (limb_t v) {
  return ::Mod1 (*this, *this, v);
}



const vec_t & __fastcall vec_t::Div (const vec_t &v) {
  vec_t u(*this);
  return ::Div (*this, u, v);
}



const vec_t & __fastcall vec_t::Mod (const vec_t &v) {
  vec_t u(*this);
  return ::Mod (*this, u, v);
}



const vec_t & __fastcall vec_t::ShiftLeft (bitcnt_t shift) {
  return ::ShiftLeft (*this, *this, shift);
}



const vec_t & __fastcall vec_t::ShiftRight (bitcnt_t shift) {
  return ::ShiftRight (*this, *this, shift);
}



const vec_t & vec_t::operator += (limb_t v) {
  return Inc(v);
}



const vec_t & vec_t::operator += (const vec_t &v) {
  return Add(v);
}



const vec_t & vec_t::operator -= (const vec_t &v) {
  return Sub(v);
}



const vec_t & vec_t::operator -= (limb_t v) {
  return Dec(v);
}



const vec_t & vec_t::operator *= (limb_t v) {
  return Mul1(v);
}



const vec_t & vec_t::operator *= (const vec_t &v) {
  return Mul(v);
}



const vec_t & vec_t::operator /= (limb_t v) {
  return Div1(v);
}



const vec_t & vec_t::operator %= (limb_t v) {
  return Mod1(v);
}



const vec_t & vec_t::operator /= (const vec_t &v) {
  return Div(v);
}



const vec_t & vec_t::operator %= (const vec_t &v) {
  return Mod(v);
}



const vec_t & __fastcall vec_t::operator <<= (bitcnt_t shift) {
  return ShiftLeft (shift);
}



const vec_t & __fastcall vec_t::operator >>= (bitcnt_t shift) {
  return ShiftRight (shift);
}



const vec_t & __fastcall Inc (vec_t &z, const vec_t &u, limb_t s) {
  if (s==0)  return z = u;
  if (z.limbs == u.limbs)  z.size = ::inc (u.limbs, u.size, s); // In-place (u+=s).
  else  z.size = ::inc (z.limbs, u.limbs, u.size, s); // Not in-place (z=u+s).
  return z;
}



const vec_t & __fastcall Add (vec_t &z, const vec_t &u, const vec_t &v) {
  if (z.limbs == u.limbs)  z.size = ::add (z.limbs, u.size, v.limbs, v.size); // In-place (u+=v).
  else if (z.limbs == v.limbs )  z.size = ::add (z.limbs, v.size, u.limbs, u.size);  // In-place (v+=u).
  else  z.size = ::add (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place (z=u+v).
  return z;
}



const vec_t & __fastcall Dec (vec_t &z, const vec_t &u, limb_t s) {
  if (s == 0)  return z = u;
  bool is_correct;
  if (z.limbs == u.limbs)  is_correct = ::dec (u.limbs, u.size, s); // In-place.
  else  is_correct = ::dec (z.limbs, u.limbs, u.size, s); // Not in-place.
  assert (is_correct);
  z.size = u.size;
  z.Normalize();
  return z;
}



const vec_t & __fastcall Sub (vec_t &z, const vec_t &u, const vec_t &v) {
  bool is_correct;
  if (z.limbs == u.limbs)  is_correct = ::sub (z.limbs, u.size, v.limbs, v.size); // In-place.
  else  is_correct = ::sub (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place.
  assert (is_correct);
  z.size = u.size;
  z.Normalize();
  return z;
}



const vec_t & __fastcall Mul1 (vec_t &z, const vec_t &u, limb_t v) {
  if (u.size==0 || v==0)  z.SetZero();
  else if (v==1) { if (&z != &u)  z = u; }
  else z.size = ::mul1 (z.limbs, u.limbs, u.size, v);
  return z;
}



const vec_t & __fastcall AddMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (u.size == 0)  return Mul1 (z, v, w);
  if (w == 1)  return Add (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    z.size = ::addmul1 (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
  }
  return z;
}



const vec_t & __fastcall SubMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  if (w == 1)  return Sub (z, u, v);
  if (v.size==0 || w==0) {
    if (&z != &u)  z = u;	// Copy without memory allocation.
  } else {
    bool is_correct = ::submul1 (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
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
    z.size = (limb_t)(z[0]!=0);
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
