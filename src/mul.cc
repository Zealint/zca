// High-level vector multiplication. A description to the function is written in mul.h file.

#include "add_low.h"
#include "add.h"
#include "sub_low.h"
#include "sub.h"
#include "mul_low.h"
#include "mul.h"
#include "misc.h"



size_t mul (limb_t *c, const limb_t *a, size_t size_a, limb_t b, limb_t s) {
	assert (can_copy_up (c, a, size_a));
	assert (is_normalized (a, size_a));
	if (size_a > 0)  s = mul_n_by_1 (c, a, size_a, b, s);
  if (s>0)  c[size_a++] = s;
  return size_a;
}



size_t addmul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b, limb_t s) {
	assert (can_copy_up (c, a, size_a));
	assert (can_copy_up (c, b, size_b));
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  limb_t carry = 0;
  size_t size_c;
  if (size_a >= size_b) {
  	if (size_b > 0)  carry = addmul_n_by_1 (c, a, b, size_b, s);
    if (size_a > size_b)  carry = inc (c+size_b, a+size_b, size_a-size_b, carry);
    size_c = size_a;
  } else {
  	if (size_a > 0)  carry = addmul_n_by_1 (c, a, b, size_a, s);
    carry = mul_n_by_1 (c+size_a, b+size_a, size_b-size_a, s, carry);
    size_c = size_b;
  }
  if (carry)  c[size_c++] = carry;
  return size_c;
}



size_t submul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b, limb_t s) {
	assert (can_copy_up (c, a, size_a));
	assert (can_copy_up (c, b, size_b));
	assert (size_a >= size_b);
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  limb_t borrow = 0;
  if (size_b > 0)  borrow = submul_n_by_1 (c, a, b, size_b, s);
  if (size_a > size_b)  borrow = dec (c+size_b, a+size_b, size_a-size_b, borrow);
  return normalize_size (c, size_a);
}


size_t mul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (c>=a+size_a || c+size_a+size_b<=a);
	assert (c>=b+size_b || c+size_a+size_b<=b);
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  limb_t carry = 0;
  if (size_a>0 && size_b>0)  carry = mul_n_by_m (c, a, size_a, b, size_b);
  else  return 0;
  return size_a+size_b - (carry==0);
}
