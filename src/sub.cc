// High-level vector subtraction. A description to the functions is written in sub.h file.

#include "sub_low.h"
#include "sub.h"
#include "misc.h"



size_t __fastcall sub (limb_t *c, limb_t *a, size_t size_a, limb_t b) {
	assert (can_copy_up (c, a, size_a));
	assert (is_normalized (a, size_a));
	if (size_a == 0) { assert (b == 0);  return 0; }
  limb_t borrow = dec (c, a, size_a, b);
	assert (borrow == 0);
  return normalize_size (c, size_a);
}



size_t __fastcall sub (limb_t *a, size_t size_a, limb_t b) {
	assert (is_normalized (a, size_a));
	if (size_a == 0) { assert (b == 0);  return 0; }
  limb_t borrow = dec (a, size_a, b);
  assert (borrow == 0);
  return normalize_size (a, size_a);
}



size_t __fastcall sub (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (size_a >= size_b);
	assert (can_copy_up (c, a, size_a) && can_copy_up (c, b, size_b));
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  limb_t borrow = 0;
  if (size_b > 0)  borrow = sub (c, a, b, size_b);
  if (size_a > size_b)  borrow = dec (c+size_b, a+size_b, size_a-size_b, borrow);
  assert (borrow == 0);
  return normalize_size (c, size_a);
}



size_t __fastcall sub (limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (size_a >= size_b);
	assert (can_copy_up (a, b, size_b));
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  limb_t borrow = 0;
  if (size_b > 0)  borrow = sub (a, a, b, size_b);
  if (size_a > size_b)  borrow = dec (a+size_b, size_a-size_b, borrow);
  assert (borrow == 0);
  return normalize_size (a, size_a);
}
