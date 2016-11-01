// High-level multiplication.
// A description to the function is written in mul.h file.

#include "add.h"
#include "sub.h"
#include "misc.h"
#include "mul.h"



limb_t __fastcall mul_N_by_M (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
  limb_t s = mul_N_by_1 (z, u, size_u, *v, 0);
  z[size_u] = s;
  const limb_t *w = v;
  while (--size_v > 0) {
    ++v;
    ++z;
    s = addmul_N_by_1 (z, z, u, size_u, *v);
    z[size_u] = s;
  }
  return s;
}



size_t mul1 (limb_t *z, const limb_t *u, size_t size, limb_t v) {
  limb_t carry = mul_N_by_1 (z, u, size, v, 0);
  if (carry)  z[size++] = carry;
  return size;
}



size_t addmul1 (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w) {
  limb_t carry;
  size_t size;
  if (size_u >= size_v) {
    carry = addmul_N_by_1 (z, u, v, size_v, w);
    if (size_u > size_v)  carry = add (z + size_v, u + size_v, size_u - size_v, carry);
    size = size_u;
  } else {
    carry = addmul_N_by_1 (z, u, v, size_u, w);
    carry = mul_N_by_1 (z + size_u, v + size_u, size_v - size_u, w, carry);
    size = size_v;
  }
  if (carry)  z[size++] = carry;
  return size;
}



bool submul1 (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w) {
  limb_t borrow = submul_N_by_1 (z, u, v, size_v, w);
  if (size_u > size_v)  borrow = sub (z + size_v, u + size_v, size_u - size_v, borrow);
  return borrow == 0;
}


size_t mul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
  limb_t carry = mul_N_by_M (z, u, size_u, v, size_v);
  return size_u + size_v - size_t(carry == 0);
}
