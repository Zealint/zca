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



vec_t::vec_t (const num_t &that): size (that.abs_size()), limbs (that.limbs) {}



vec_t::vec_t (const char *str, u8 base) {
	const size_t D = how_many_digits (str, base);
  const size_t N = how_many_limbs (D, base);
  limbs = new limb_t[N];
  from_string (str, base);
  assert (abs(N-size) <= 1);
}



void __fastcall vec_t::attach (const num_t &that) {
  assert (limbs == nullptr);
  limbs = that.limbs;
  size = that.abs_size();
}



void __fastcall vec_t::resize (size_t s) {
  size = 0;
  delete[] limbs;
  limbs = new limb_t[s];
}



void __fastcall vec_t::adapt (size_t s) {
  if (size >= s)  return;
  limb_t *data = new limb_t[s];
  if (size>0)  copy_up (data, limbs, size);
  delete[] limbs;
  limbs = data;
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



vec_t & __fastcall vec_t::operator = (const vec_t &that) {
  if (this == &that)  return *this;
  size = that.size;
  if (size > 0)  copy_up (limbs, that.limbs, size);
  return *this;
}



vec_t & __fastcall add (vec_t &c, const vec_t &a, const vec_t &b) {
  assert (a.is_normalized() && b.is_normalized());
  if (a.is_zero())  return c = b;
  if (b.is_zero())  return c = a;
  if (a.size == 1)  return add (c, b, a[0]);
  if (b.size == 1)  return add (c, a, b[0]);
  if (c.limbs == a.limbs)  c.size = ::add (a.limbs, a.size, b.limbs, b.size); // In-place (c+=b).
  else if (c.limbs == b.limbs )  c.size = ::add (b.limbs, b.size, a.limbs, a.size);  // In-place (c+=a).
  else  c.size = ::add (c.limbs, a.limbs, a.size, b.limbs, b.size); // Not in-place (c=a+b).
  return c;
}



vec_t & __fastcall sub (vec_t &c, const vec_t &a, const vec_t &b) {
  assert (a.is_normalized() && b.is_normalized());
  if (b.is_zero())  return c = a;
  if (b.size == 1)  return sub (c, a, b[0]);
  if (c.limbs == a.limbs)  c.size = ::sub (a.limbs, a.size, b.limbs, b.size); // In-place (a-=b).
  else  c.size = ::sub (c.limbs, a.limbs, a.size, b.limbs, b.size); // Not in-place (c=a-b).
  return c;
}


vec_t & __fastcall addmul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
  assert (u.is_normalized() && v.is_normalized());
  if (u.size == 0)  return mul (z, v, w);
  if (w == 1)  return add (z, u, v);
  if (v.size==0 || w==0)  z = u;
  else  z.size = ::addmul (z.limbs, u.limbs, u.size, v.limbs, v.size, w);
  return z;
}



vec_t & __fastcall submul (vec_t &z, const vec_t &u, const vec_t &v, limb_t w) {
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



vec_t & __fastcall mul (vec_t &c, const vec_t &a, const vec_t &b) {
  assert (a.is_normalized() && b.is_normalized());
  if (a.is_zero() || b.is_zero())  return c.set_zero();
  if (a.limbs==c.limbs || b.limbs==c.limbs) { vec_t d (a.size+b.size, CI_SIZE);  return c = mul (d, a, b); }
  if (a.size == 1)  return mul (c, b, a[0]);
  if (b.size == 1)  return mul (c, a, b[0]);
  c.size = ::mul (c.limbs, a.limbs, a.size, b.limbs, b.size);
  return c;
}



vec_t & __fastcall div_qr (vec_t *q, vec_t *r, const vec_t &a, const vec_t &b) {
  assert (a.is_normalized() && b.is_normalized());
  assert (!b.is_zero());
  assert (q != r);	// It also means that either 'q' or 'r' is not nullptr.
  if (a.is_zero()) {
    if (q != nullptr)  q->set_zero();
    if (r != nullptr)  r->set_zero();
  } else if (a < b) {
    if (q != nullptr)  q->set_zero();
    if (r != nullptr)  *r = a;
  }
  else {
    vec_t R(a);
    limb_t *Q;
    if (q == nullptr)  Q = new limb_t[a.size/*-b.size+1*/];	// Here a >= b.
    else  Q = q->limbs;
    size_t size_q = ::div_qr (Q, R.limbs, R.size, b.limbs, b.size);
    if (q == nullptr)  delete[] Q;
    else q->size = size_q;
    if (r != nullptr) *r = R;
  }
  if (q != nullptr)  return *q;
  return *r;
}

vec_t & __fastcall div (vec_t &c, const vec_t &a, const vec_t &b) { return div_qr (&c, nullptr, a, b); }
vec_t & __fastcall mod (vec_t &c, const vec_t &a, const vec_t &b) { return div_qr (nullptr, &c, a, b); }

vec_t & __fastcall shift_left (vec_t &c, const vec_t &a, bitcnt_t shift) {
  assert (a.is_normalized());
  if (a.size == 0)  return c.set_zero();
  if (shift == 0)  return c = a;
  if (shift < LIMB_BITS) {
    limb_t carry = ::shift_left_short (c.limbs, a.limbs, a.size, (u8)shift);
    c.size = a.size;
    if (carry)  c[c.size++] = carry;
  } else  c.size = ::shift_left_long (c.limbs, a.limbs, a.size, shift);
  return c;
}



vec_t & __fastcall shift_right (vec_t &c, const vec_t &a, bitcnt_t shift) {
  assert (a.is_normalized());
  if (a.size==0 || shift>=length_in_bits (a.limbs, a.size))  return c.set_zero();
  if (shift == 0)  return c = a;
	if (shift < LIMB_BITS) {
    ::shift_right_short (c.limbs, a.limbs, a.size, (u8)shift);
    c.size = a.size;
    c.normalize();
  } else  c.size = ::shift_right_long (c.limbs, a.limbs, a.size, shift);
  return c;
}
