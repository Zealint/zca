// High-level multiplication. A description to the function is written in mul.h file.

#include "add.h"
#include "add_low.h"
#include "sub.h"
#include "misc.h"
#include "mul.h"



limb_t __fastcall mul_n_by_m (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (size_u>0 && size_v>0);
  limb_t s = mul_n_by_1 (z, u, size_u, *v, 0);
  z[size_u] = s;
  while (--size_v > 0) {
    ++v;
    ++z;
    s = addmul_n_by_1 (z, z, u, size_u, *v);
    z[size_u] = s;
  }
  return s;
}



size_t mul (limb_t *z, const limb_t *u, size_t size, limb_t v) {
	assert (can_copy_up (z, u, size));
	limb_t carry = 0;
	if (size > 0)  carry = mul_n_by_1 (z, u, size, v);
  if (carry)  z[size++] = carry;
  return size;
}



size_t addmul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w) {
	assert (can_copy_up (z, u, size_u));
	assert (can_copy_up (z, v, size_v));
  limb_t carry = 0;
  size_t size_z;
  if (size_u >= size_v) {
  	if (size_v > 0)  carry = addmul_n_by_1 (z, u, v, size_v, w);
    if (size_u > size_v)  carry = add (z+size_v, u+size_v, size_u-size_v, carry);
    size_z = size_u;
  } else {
  	if (size_u > 0)  carry = addmul_n_by_1 (z, u, v, size_u, w);
    carry = mul_n_by_1 (z+size_u, v+size_u, size_v-size_u, w, carry);
    size_z = size_v;
  }
  if (carry)  z[size_z++] = carry;
  return size_z;
}



bool submul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w) {
	assert (can_copy_up (z, u, size_u));
	assert (can_copy_up (z, v, size_v));
	assert (size_u >= size_v);
  limb_t borrow = 0;
  if (size_v > 0)  borrow = submul_n_by_1 (z, u, v, size_v, w);
  if (size_u > size_v)  borrow = sub (z+size_v, u+size_v, size_u-size_v, borrow);
  return borrow == 0;
}


size_t mul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (z>=u+size_u || z+size_u+size_v<=u);
	assert (z>=v+size_v || z+size_u+size_v<=v);
  limb_t carry;
  if (size_u>0 && size_v>0)  carry = mul_n_by_m (z, u, size_u, v, size_v);
  else  return 0;
  return size_u+size_v - (carry==0);
}
