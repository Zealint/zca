// High-level addition. A description to the function is written in add.h file.

#include "add.h"
#include "add_low.h"
#include "misc.h"	// swap



size_t __fastcall inc (limb_t *z, limb_t *u, size_t size_u, limb_t s) {
  assert (can_copy_up (z, u, size_u));
  limb_t carry = s;
  if (size_u > 0)  carry = add (z, u, size_u, carry);
  if (carry)  z[size_u++] = carry;
  return size_u;
}



size_t __fastcall inc (limb_t *u, size_t size_u, limb_t s) {
	limb_t carry = s;
	if (size_u > 0)  carry = add (u, size_u, carry);
  if (carry)  u[size_u++] = carry;
  return size_u;
}



size_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (can_copy_up (z, u, size_u) && can_copy_up (z, v, size_v));
  // If size of 'u' less than size of 'v', swap them to avoid symmetric case.
  if (size_u < size_v) {
    swap (size_u, size_v);
    swap (u, v);
  }
  limb_t carry = 0;
  if (size_v > 0)  carry = add (z, u, v, size_v);	// Add {u, size_v}+{v, size_v} (equal length), 'cause size_v<size_u.
  if (size_u > size_v)  carry = add (z+size_v, u+size_v, size_u-size_v, carry);	// Add 'tail' of u.
  if (carry)  z[size_u++] = carry;
  return size_u;
}



size_t __fastcall add (limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
	assert (can_copy_up (u, v, size_v));
  // If size_u less than size_v then we have to copy 'tail' of v into u (if no carry when add common parts).
  limb_t carry = 0;
  if (size_u < size_v) {
    if (size_u > 0)  carry = add (u, u, v, size_u);	// {u, size_u} += {v, size_u}, 'cause size_u<size_v.
    if (carry) {
      carry = add (u+size_u, v+size_u, size_v-size_u, carry);	// Add carry to the 'tail' of v.
    } else {
    	copy_up (u+size_u, v+size_u, size_v-size_u);	// Here we can just copy 'tail' of v.
    	carry = 0;
    }
    if (carry)  u[size_v++] = carry;
    return size_v;
  }
  // Here size_u >= size_v.
  if (size_v > 0)  carry = add (u, u, v, size_v);
  if (size_u > size_v)  carry = add (u+size_v, size_u-size_v, carry);	// in-place addition.
  if (carry)  u[size_u++] = carry;
  return size_u;
}
