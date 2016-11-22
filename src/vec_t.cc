// Implementation of class 'vec_t'

#include "vec_t.h"
#include "num_t.h"
#include "misc.h"
#include "bit.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include <stdio.h>



vec_t::vec_t (const num_t &that): size (that.unsigned_size()), limbs (that.limbs) {}



vec_t::vec_t (const char *str, u8 base) {
  while (*str == '0')  ++str;	// Remove leading zeroes.
  size_t d=0;	// How many digits do we have?
  while (is_digit(str[d], base))  ++d;
  d += (d==0);	// In case of d=0 we will have d=1.
  const size_t N = (size_t)ceil(log((double)base)/log((double)LIMB_BITS) * d);	// Maximum number of limbs for d-digit number
  limbs = new limb_t[N];
  from_string (str, base);
}



void __fastcall vec_t::attach (const num_t &that) {
  assert (limbs == nullptr);
  limbs = that.limbs;
  size = that.unsigned_size();
}



void __fastcall vec_t::detach() { limbs = nullptr; }



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
  assert (is_normalized() && that.is_normalized());
  size_t i = size;
  while (--i>0 && limbs[i]==that.limbs[i]);
  if (limbs[i] < that.limbs[i])  return -1;
  if (limbs[i] > that.limbs[i])  return 1;
  return 0;
}



sign_t __fastcall vec_t::compare (const vec_t &that) const {
  assert (is_normalized() && that.is_normalized());
  if (size < that.size)  return -1;
  if (size > that.size)  return 1;
  if (size == 0)  return 0;
  return compare_equal_size (that);
}



sign_t __fastcall vec_t::compare (limb_t s) const {
  assert (is_normalized());
  if (size > 1)  return +1;
  if (size == 0) return 0 - (s!=0);
  if (limbs[0] < s)  return -1;
  if (limbs[0] > s)  return +1;
  return 0;
}



const vec_t & __fastcall vec_t::operator = (const vec_t &that) {
  if (this == &that)  return *this;
  size = that.size;
  if (size > 0)  copy_up (limbs, that.limbs, size);
  return *this;
}



const vec_t & __fastcall vec_t::operator = (u64 a) {
  const size_t U64_BITS = 64;
  assert (U64_BITS<LIMB_BITS || U64_BITS%LIMB_BITS==0);
  if (a == 0)  set_zero();
  else if (U64_BITS <= LIMB_BITS) {
    size = 1;
    limbs[0] = (limb_t)a;
  } else {
    const size_t S = U64_BITS / LIMB_BITS;
    for (size=0; a>0 && size<S; ++size) {
      limbs[size] = (limb_t)a;
      a >>= LIMB_BITS;	// Here LIMB_BITS < 64
    }
  }
  assert (is_normalized());
  return *this;
}



const vec_t & __fastcall vec_t::operator = (vec_t &&that) {
  if (this == &that)  return *this;
  delete[] limbs;
  size = that.size;
  limbs = that.limbs;
  that.limbs = nullptr;
  return * this;
}



const vec_t & vec_t::inc (limb_t s) {
	assert (is_normalized());
	if (s == 0)  return *this;
	size = ::inc (limbs, size, s);
	return *this;
}



const vec_t & vec_t::add (const vec_t &v) {
  assert (is_normalized() && v.is_normalized());
  if (v.is_zero())  return *this;
  if (v.size == 1)  return this->inc(v[0]);
	size = ::add (limbs, size, v.limbs, v.size);
	return *this;
}



const vec_t & vec_t::dec (limb_t s) {
  assert (is_normalized());
  if (s == 0)  return *this;
  bool is_correct = ::dec (limbs, size, s);
  assert (is_correct);
  size = normalize_size (limbs, size);
  return *this;
}



const vec_t & vec_t::sub (const vec_t &v) {
  assert (is_normalized() && v.is_normalized());
  if (v.is_zero())  return *this;
  if (v.size == 1)  return this->dec(v[0]);
  bool is_correct = ::sub (limbs, size, v.limbs, v.size);
  assert (is_correct);
  size = normalize_size (limbs, size);
  return *this;
}



const vec_t & __fastcall vec_t::mul (limb_t v) {
  assert (is_normalized());
  return ::mul (*this, *this, v);
}



const vec_t & __fastcall vec_t::mul (const vec_t &v) {
  assert (is_normalized() && v.is_normalized());
  vec_t u (*this);
  return ::mul (*this, u, v);
}



const vec_t & __fastcall vec_t::addmul (const vec_t &v, limb_t w) {
  assert (is_normalized() && v.is_normalized());
  return ::addmul (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::submul (const vec_t &v, limb_t w) {
  assert (is_normalized() && v.is_normalized());
  return ::submul (*this, *this, v, w);
}



const vec_t & __fastcall vec_t::div (limb_t v) {
  assert (v != 0);
  assert (is_normalized());
  return ::div (*this, *this, v);
}



limb_t __fastcall vec_t::mod (limb_t v) {
  assert (v != 0);
  assert (is_normalized());
  if (is_zero())  return 0;
  limbs[0] = ::mod (*this, v);
  size = 1;
  normalize();
  return limbs[0];
}



const vec_t & __fastcall vec_t::div (const vec_t &v) {
  assert (!v.is_zero());
  assert (is_normalized() && v.is_normalized());
  vec_t u (*this);
  return ::div (*this, u, v);
}



const vec_t & __fastcall vec_t::mod (const vec_t &v) {
  assert (!v.is_zero());
  assert (is_normalized() && v.is_normalized());
  vec_t u (*this);
  return ::mod (*this, u, v);
}



const vec_t & __fastcall vec_t::shift_left (bitcnt_t shift) {
  assert (is_normalized());
  return ::shift_left (*this, *this, shift);
}



const vec_t & __fastcall vec_t::shift_right (bitcnt_t shift) {
  assert (is_normalized());
  return ::shift_right (*this, *this, shift);
}



const vec_t & vec_t::operator += (limb_t v) { return inc(v); }
const vec_t & vec_t::operator -= (limb_t v) { return dec(v); }
const vec_t & vec_t::operator *= (limb_t v) { return mul(v); }
const vec_t & vec_t::operator /= (limb_t v) { return div(v); }
limb_t vec_t::operator %= (limb_t v) { return mod(v); }

const vec_t & vec_t::operator += (const vec_t &v) { return add(v); }
const vec_t & vec_t::operator -= (const vec_t &v) { return sub(v); }
const vec_t & vec_t::operator *= (const vec_t &v) { return mul(v); }
const vec_t & vec_t::operator /= (const vec_t &v) { return div(v); }
const vec_t & vec_t::operator %= (const vec_t &v) { return mod(v); }



const vec_t & __fastcall vec_t::operator <<= (bitcnt_t shift) { return shift_left (shift); }
const vec_t & __fastcall vec_t::operator >>= (bitcnt_t shift) { return shift_right (shift); }



const vec_t & __fastcall vec_t::from_string (const char *str, u8 base) {
  size = ::from_string (limbs, str, base);
  return *this;
}



char * __fastcall vec_t::to_string (char *str, u8 base) const {
  return ::to_string (str, limbs, size, base);
}



const vec_t & __fastcall inc (vec_t &z, const vec_t &u, limb_t s) {
  //printf ("HERE u[0]=%x, s=%x\n", u[0], s);
  assert (u.is_normalized());
  if (s==0)  return z = u;
  if (z.limbs == u.limbs)  z.size = ::inc (u.limbs, u.size, s); // In-place (u+=s).
  else {
    z.size = ::inc (z.limbs, u.limbs, u.size, s); // Not in-place (z=u+s).
    //printf ("HERE z[0]=%x, size=%u, u[0]=%x\n", z[0], z.size, u[0]);
  }
  return z;
}



const vec_t & __fastcall add (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (u.is_normalized() && v.is_normalized());
  if (u.is_zero())  return z = v;
  if (v.is_zero())  return z = u;
  if (u.size == 1)  return inc (z, v, u[0]);
  if (v.size == 1)  return inc (z, u, v[0]);
  if (z.limbs == u.limbs)  z.size = ::add (z.limbs, u.size, v.limbs, v.size); // In-place (u+=v).
  else if (z.limbs == v.limbs )  z.size = ::add (z.limbs, v.size, u.limbs, u.size);  // In-place (v+=u).
  else  z.size = ::add (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place (z=u+v).
  return z;
}



const vec_t & __fastcall dec (vec_t &z, const vec_t &u, limb_t s) {
  assert (u.is_normalized());
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
  assert (u.is_normalized() && v.is_normalized());
  if (v.is_zero())  return z = u;
  if (v.size == 1)  return dec (z, u, v[0]);
  bool is_correct;
  if (z.limbs == u.limbs)  is_correct = ::sub (z.limbs, u.size, v.limbs, v.size); // In-place.
  else  is_correct = ::sub (z.limbs, u.limbs, u.size, v.limbs, v.size); // Not in-place.
  assert (is_correct);
  z.size = u.size;
  z.normalize();
  return z;
}



const vec_t & __fastcall mul (vec_t &z, const vec_t &u, limb_t v) {
  assert (u.is_normalized());
  if (u.size==0 || v==0)  z.set_zero();
  else if (v == 1)  z = u;
  else if (is_power_of_2(v))  return shift_left (z, u, LIMB_BITS-1 - count_lz(v));
  else z.size = ::mul (z.limbs, u.limbs, u.size, v);
  return z;
}



const vec_t & __fastcall addmul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  assert (u.is_normalized() && v.is_normalized());
  if (u.size == 0)  return mul (z, v, w);
  if (w == 1)  return add (z, u, v);
  if (v.size==0 || w==0)  z = u;
  else  z.size = ::addmul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
  return z;
}



const vec_t & __fastcall submul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  assert (u.is_normalized() && v.is_normalized());
  if (w == 1)  return sub (z, u, v);
  if (v.size==0 || w==0)  z = u;
  else {
    bool is_correct = ::submul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
    assert (is_correct);
    z.size = u.size;
    z.normalize();
  }
  return z;
}



const vec_t & __fastcall mul (vec_t &z, const vec_t &u, const vec_t &v) {
  if (u.limbs == z.limbs) { vec_t tmp_u(u); return mul (z, tmp_u, v); }
  if (v.limbs == z.limbs) { vec_t tmp_v(v); return mul (z, u, tmp_v); }
  assert (u.is_normalized() && v.is_normalized());
  if (u.size==0 || v.size==0)  z.set_zero();
  else if (u.size == 1)  return mul (z, v, u[0]);
  else if (v.size == 1)  return mul (z, u, v[0]);
  else z.size = ::mul (z.limbs, u.limbs, u.size, v.limbs, v.size);
  return z;
}



const vec_t & __fastcall div (vec_t &z, const vec_t &u, limb_t v) {
  assert (v != 0);
  assert (u.is_normalized());
  if (u.size == 0)  z.set_zero();
  else if (v == 1)  z = u;
  else z.size = ::div_qr (z.limbs, nullptr, u.limbs, u.size, v);
  return z;
}



limb_t __fastcall mod (const vec_t &u, limb_t v) {
  assert (v != 0);
  assert (u.is_normalized());
  if (u.size==0 || v==1)  return 0;
  else {
    limb_t r;
    ::div_qr (nullptr, &r, u.limbs, u.size, v);
    return r;
  }
}



const vec_t & __fastcall div (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.is_zero());
  assert (u.is_normalized() && v.is_normalized());
  if (u.size == 0)  z.set_zero();
  else {
    vec_t r(u);
    //vec_t * r = new vec_t(u);
    z.size = ::div_qr (z.limbs, r.limbs, r.size, v.limbs, v.size);
    //printf ("Before div %u %u\n", r, r->limbs);
    //z.size = ::div_qr (z.limbs, r->limbs, r->size, v.limbs, v.size);
    //printf ("After div %u %u\n", r, r->limbs);
    //printf ("Before delete\n");
    //delete r;
    //printf ("After delete\n");
  }
  //printf ("Before return\n");
  return z;
}



const vec_t & __fastcall mod (vec_t &z, const vec_t &u, const vec_t &v) {
  assert (!v.is_zero());
  assert (u.is_normalized() && v.is_normalized());
  if (u.size == 0)  z.set_zero();
  else {
    z = u;
    vec_t tmp(u); // !!! nullptr doesn't work.
    ::div_qr (tmp.limbs, z.limbs, z.size, v.limbs, v.size);
  }
  return z;
}



const vec_t & __fastcall shift_left (vec_t &z, const vec_t &u, bitcnt_t shift) {
  assert (u.is_normalized());
  if (u.size == 0)  z.set_zero();
  else if (shift == 0)  z = u;
  else if (shift < LIMB_BITS) {
    limb_t carry = ::shift_left_short (z.limbs, u.limbs, u.size, (u8)shift);
    z.size = u.size;
    if (carry)  z[z.size++] = carry;
  }
  else  z.size = ::shift_left_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall shift_right (vec_t &z, const vec_t &u, bitcnt_t shift) {
  assert (u.is_normalized());
  if (u.size == 0)  z.set_zero();
  else if (shift == 0)  z = u;
	else if (shift < LIMB_BITS) {
    ::shift_right_short (z.limbs, u.limbs, u.size, (u8)shift);
    z.size = u.size;
    z.normalize();
  }
  else  z.size = ::shift_right_long (z.limbs, u.limbs, u.size, shift);
  return z;
}



const vec_t & __fastcall vec_t::operator ++ () { return inc((limb_t)1); }
const vec_t & __fastcall vec_t::operator -- () { return dec((limb_t)1); }



const vec_t __fastcall operator + (const vec_t &u, limb_t v) {
  vec_t z (u.size + 1);
  inc (z, u, v);
  return z;
}



const vec_t __fastcall operator + (const vec_t &u, const vec_t &v) {
  vec_t z (max (u.size, v.size) + 1);
  add (z, u, v);
  return z;
}



const vec_t __fastcall operator - (const vec_t &u, limb_t v) {
  vec_t z (u.size);
  dec (z, u, v);
  return z;
}



const vec_t __fastcall operator - (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  sub (z, u, v);
  return z;
}



const vec_t __fastcall operator * (const vec_t &u, limb_t v) {
  vec_t z (u.size+1);
  mul (z, u, v);
  return z;
}



const vec_t __fastcall operator * (const vec_t &u, const vec_t &v) {
  vec_t z (u.size+v.size);
  mul (z, u, v);
  return z;
}



const vec_t __fastcall operator / (const vec_t &u, limb_t v) {
  vec_t z (u.size);
  div (z, u, v);
  return z;
}



limb_t __fastcall operator % (const vec_t &u, limb_t v) { return mod (u, v); }



const vec_t __fastcall operator / (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  div (z, u, v);
  return z;
}



const vec_t __fastcall operator % (const vec_t &u, const vec_t &v) {
  vec_t z (u.size);
  mod (z, u, v);
  return z;
}



const vec_t __fastcall operator << (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + (shift+LIMB_BITS-1)/LIMB_BITS);
  shift_left (z, u, shift);
  return z;
}



const vec_t __fastcall operator >> (const vec_t &u, bitcnt_t shift) {
  vec_t z (u.size + shift/LIMB_BITS);
  shift_right (z, u, shift);
  return z;
}
