// High-level subtraction. A description to the functions is written in sub.h file.

#include "sub.h"
#include "misc.h"



bool __fastcall dec (limb_t *z, limb_t *u, size_t size_u, limb_t s) {
	assert (can_copy_up (z, u, size_u));
	if (size_u == 0)  return s == 0;
  return sub (z, u, size_u, s) == 0;
}



bool __fastcall dec (limb_t *u, size_t size_u, limb_t s) {
	if (size_u == 0)  return s == 0;
  return sub (u, size_u, s) == 0;
}



bool __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (size_u>=size_v && can_copy_up (z, u, size_u) && can_copy_up (z, v, size_v));
  limb_t borrow = 0;
  if (size_v > 0)  borrow = sub (z, u, v, size_v);
  if (size_u > size_v)  borrow = sub (z+size_v, u+size_v, size_u-size_v, borrow);
  return borrow == 0;
}



bool __fastcall sub (limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (size_u>=size_v && can_copy_up (u, v, size_v));
  limb_t borrow = 0;
  if (size_v > 0)  borrow = sub (u, u, v, size_v);
  if (size_u > size_v)  borrow = sub (u+size_v, size_u-size_v, borrow);
  return borrow == 0;
}
