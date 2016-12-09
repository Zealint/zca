// High-level vector division. A description to the function is written in div.h file.

#include "bit.h"
#include "add_low.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"
#include "div_low.h"
#include "misc.h"



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



static u8 __fastcall normalize_divisor (limb_t &d1, limb_t &d0, const limb_t &tail) {
	u8 shift = count_lz (d1);
	if (shift) {
		d1 = (d1<<shift) | (d0>>(LIMB_BITS-shift));
		d0 = (d0<<shift) | (tail>>(LIMB_BITS-shift));
	}
	return shift;
}



size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *a, size_t size_a, limb_t d) {
  assert (size_a>0 && d!=0);
  assert (q != r);
  size_t c = (a[size_a-1]<d);
  if (is_power_of_2(d)) {
    if (d == 1) {
      if (q != nullptr)  copy_up (q, a, size_a);
      if (r != nullptr)  *r = 0;
      return size_a - c;
    }
    const u8 shift = pos_of_high_one(d);
    limb_t R;
    if (r != nullptr)  R = a[0] & (((limb_t)1<<shift)-1);
    if (q != nullptr)  shift_right_short (q, a, size_a, shift);
    if (r != nullptr)  *r = R;
    return size_a - c;
  }
  limb_t *a_shifted, a_n=0;
  u8 shift = normalize_divisor (d);
#ifdef DIV_PREINVERSION
  limb_t v = inv_2_by_1 (d);
#endif
  if (shift > 0) {
    a_shifted = new limb_t[size_a];
    a_n = shift_left_short (a_shifted, a, size_a, shift);
    a = a_shifted;
  }
#ifdef DIV_PREINVERSION
  limb_t R = q!=nullptr ? div_n_by_1 (q, a_n, a, size_a, d, v) : mod_n_by_1 (a_n, a, size_a, d, v);
#else
  limb_t R = q!=nullptr ? div_n_by_1 (q, a_n, a, size_a, d) : mod_n_by_1 (a_n, a, size_a, d);
#endif
  if (shift > 0) {
    R >>= shift;
    delete[] a_shifted;
  }
  if (r != nullptr)  *r = R;
  return size_a - c;
}



size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *a, size_t size_a, limb_t d1, limb_t d0) {
  assert (size_a>1 && d1!=0);
  assert (q != r);
  limb_t *a_shifted, a_n=0;
  size_t c = (size_t)(a[size_a-1]<d1 || (a[size_a-1]==d1 && a[size_a-2]<d0)) + 1;
  u8 shift = normalize_divisor (d1, d0);
  if (shift > 0) {
    a_shifted = new limb_t[size_a];
    a_n = shift_left_short (a_shifted, a, size_a, shift);
    a = a_shifted;
  }
  limb_t R[2] = {0, 0};
#ifdef DIV_PREINVERSION
	if (q != nullptr)  div_n_by_2 (q, R, a_n, a, size_a, d1, d0, inv_3_by_2 (d1, d0));
	else  mod_n_by_2 (R[1], R[0], a_n, a, size_a, d1, d0, inv_3_by_2 (d1, d0));
#else
	if (q != nullptr)  div_n_by_2 (q, R, a_n, a, size_a, d1, d0);
	else  mod_n_by_2 (R[1], R[0], a_n, a, size_a, d1, d0);
#endif
  if (shift > 0) {
    if (r != nullptr)  shift_right_short (r, R, 2, shift);
    delete[] a_shifted;
  } else  copy_up (r, R, 2);
  return size_a - c;
}


#include <stdio.h>
size_t __fastcall div_qr (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d) {
	assert (size_d>0 && d[size_d-1]!=0);
  assert (q!=a && q!=d && a!=d);
  size_t initial_size_a = size_a;
  if (size_a < size_d)  return 0;
  if (size_d == 1) {
    limb_t r;
    size_t res = div_qr (q, &r, a, size_a, d[0]);
    if (r != 0) { a[0] = r;  size_a = 1; }
    else size_a = 0;
    return q != nullptr ? res : 0;
  }
  if (size_d == 2) {
    limb_t r[2];
    size_t res = div_qr (q, r, a, size_a, d[1], d[0]);
    a[0] = r[0];
    a[1] = r[1];
    size_a = normalize_size (a, 2);
    return q != nullptr ? res : 0;
  }
  limb_t d1=d[size_d-1], d0=d[size_d-2], tail=d[size_d-3];
  u8 shift = normalize_divisor (d1, d0, tail);
  size_t k = size_a - size_d;
  limb_t *a_shifted, a_m = 0;
  a_shifted = new limb_t[size_a+1];
  if (shift > 0) {
    a_m = shift_left_short (a_shifted, a, size_a, shift);
    if (a_m)  a_shifted[size_a++] = a_m;
    limb_t *d_shifted = new limb_t [size_d];
    shift_left_short (d_shifted, d, size_d, shift);
    d = d_shifted;
  }
  else copy_up (a_shifted, a, size_a);
  size_t size_q = k + 1;
#ifdef DIV_PREINVERSION
  div_m_by_n (q, a_shifted, size_a, d, size_d, inv_3_by_2 (d1, d0));
#else
  div_m_by_n (q, a_shifted, size_a, d, size_d);
#endif
  if (shift > 0) {
    if (initial_size_a<size_a) {
    	assert (false);
    	//??? shift_right_short (a, a_shifted, size_a-1, shift);
      printf ("Assertion in initial_size_a<size_a in div_qr for n>=3 case\n");
    }
    else  shift_right_short (a, a_shifted, size_a, shift);
    delete[] d;
  } else copy_up (a, a_shifted, size_a);
  delete[] a_shifted;
  size_a = normalize_size (a, size_a);
  if (q != nullptr)  size_q = normalize_size (q, size_q);
  return q != nullptr ? size_q : 0;
}
