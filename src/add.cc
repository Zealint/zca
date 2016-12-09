// High-level vector addition. A description to the function is written in add.h file.

#include "add_low.h"
#include "add.h"
#include "misc.h"



size_t __fastcall add (limb_t *c, limb_t *a, size_t size_a, limb_t b) {
  assert (can_copy_up (c, a, size_a));
  assert (is_normalized (a, size_a));
  limb_t carry = b;
  if (size_a > 0)  carry = inc (c, a, size_a, carry);
  if (carry)  c[size_a++] = carry;
  return size_a;
}



size_t __fastcall add (limb_t *a, size_t size_a, limb_t b) {
	assert (is_normalized (a, size_a));
	limb_t carry = b;
	if (size_a > 0)  carry = inc (a, size_a, carry);
  if (carry)  a[size_a++] = carry;
  return size_a;
}



size_t __fastcall add (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (can_copy_up (c, a, size_a) && can_copy_up (c, b, size_b));
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  // If size of 'a' less than size of 'b', swap them to avoid symmetric case.
  if (size_a < size_b) {
    swap (size_a, size_b);
    swap (a, b);
  }
  limb_t carry = 0;
  if (size_a > 0)  carry = add (c, a, b, size_b);	// Add {a, size_b}+{b, size_b} (equal length), 'cause size_b<=size_a.
  if (size_a > size_b)  carry = inc (c+size_b, a+size_b, size_a-size_b, carry);	// Add 'tail' of a.
  if (carry)  c[size_a++] = carry;
  return size_a;
}



size_t __fastcall add (limb_t *a, size_t size_a, const limb_t *b, size_t size_b) {
	assert (can_copy_up (a, b, size_b));
	assert (is_normalized (a, size_a));
	assert (is_normalized (b, size_b));
  // If size_a less than size_b then we have to copy 'tail' of b into a (if no carry when add common parts).
  limb_t carry = 0;
  if (size_a < size_b) {
    if (size_a > 0)  carry = add (a, a, b, size_a);	// {a, size_a} += {b, size_a}, 'cause size_a<size_b.
    if (carry)  carry = inc (a+size_a, b+size_a, size_b-size_a, carry);	// Add carry to the 'tail' of b.
    else  copy_up (a+size_a, b+size_a, size_b-size_a);	// Here we can just copy 'tail' of v.
    if (carry)  a[size_b++] = carry;
    return size_b;
  }
  // Here size_u >= size_v.
  if (size_b > 0)  carry = add (a, a, b, size_b);
  if (size_a > size_b)  carry = inc (a+size_b, size_a-size_b, carry);	// in-place addition.
  if (carry)  a[size_a++] = carry;
  return size_a;
}
