// Low-level bit operations

#include <assert.h>
#include "bit.h"



void __fastcall copy_up (limb_t *z, const limb_t *u, size_t size) {
  for (size_t i=0; i<size; ++i)  z[i] = u[i];
}



void __fastcall copy_down (limb_t *z, const limb_t *u, size_t size) {
  size_t i = size;
  while (i-- > 0)  z[i] = u[i];
}



void __fastcall make_zero (limb_t *z, size_t size) {
  for (size_t i=0; i<size; ++i)  z[i] = 0;
}



limb_t __fastcall shift_left_short (limb_t *z, const limb_t *u, size_t size_u, u8 shift) {
  assert (size_u>0 && 0<shift && shift<LIMB_BITS);

  z += size_u;
  u += size_u;
  u8 tail = LIMB_BITS - shift;
  limb_t l = *--u;
  limb_t h = l << shift;
  limb_t res = l >> tail;

  while (--size_u != 0) {
    l = *--u;
    *--z = h | (l >> tail);
    h = l << shift;
  }
  *--z = h;

  return res;
}



limb_t __fastcall shift_right_short (limb_t *z, const limb_t *u, size_t size_u, u8 shift) {
  assert (size_u > 0 && 0 < shift && shift < LIMB_BITS);

  u8 tail = LIMB_BITS - shift;
  limb_t h = *u++;
  limb_t l = h >> shift;
  limb_t res = h << tail;

  while (--size_u != 0) {
    h = *u++;
    *z++ = l | (h << tail);
    l = h >> shift;
  }
  *z = l;

  return res;
}



size_t __fastcall shift_left_long (limb_t *z, const limb_t *u, size_t size_u, bitcnt_t shift) {
  assert (size_u > 0);
  size_t long_shift = shift/LIMB_BITS;
  u8 short_shift = shift%LIMB_BITS;
  limb_t carry = 0;
  if (short_shift > 0)  carry = shift_left_short (z+long_shift, u, size_u, short_shift);
  else  copy_down (z+long_shift, u, size_u);
  make_zero (z, long_shift);
  size_t size_z = size_u + long_shift;
  if (carry)  z[size_z++] = carry;
  return size_z;
}



size_t __fastcall shift_right_long (limb_t *z, const limb_t *u, size_t size_u, bitcnt_t shift) {
  assert (size_u > 0);
  size_t long_shift = shift/LIMB_BITS;
  u8 short_shift = shift%LIMB_BITS;
  if (short_shift > 0)  shift_right_short (z, u+long_shift, size_u-long_shift, short_shift);
  else  copy_up (z+long_shift, u, size_u);
  size_t size_z = size_u - long_shift;
  if (size_z>0 && z[size_z-1]==0)  --size_z;
  return size_z;
}
