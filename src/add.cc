// High-level addition.
// A description to the function is written in add.h file.

#include "add.h"
#include "add_low.h"
#include "misc.h"



size_t __fastcall inc (limb_t *z, limb_t *u, size_t size_u, limb_t s) {
  limb_t carry = add (z, u, size_u, s);
  if (carry)  z[size_u++] = carry;
  return size_u;
}



size_t __fastcall inc (limb_t *u, size_t size_u, limb_t s) {
  limb_t carry = add (u, size_u, s);
  if (carry)  u[size_u++] = carry;
  return size_u;
}



size_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
  // If size of 'u' less than size of 'v', swap them to avoid symmetric case.
  if (size_u < size_v) {
    swap (size_u, size_v);
    swap (u, v);
  }
  limb_t carry = add (z, u, v, size_v);
  if (size_u > size_v)  carry = add (z + size_v, u + size_v, size_u - size_v, carry);
  if (carry)  z[size_u++] = carry;
  return size_u;
}



size_t __fastcall add (limb_t *u, size_t size_u, const limb_t *v, size_t size_v) {
  // If size of 'u' less than size of 'v' then we have to copy 'tail' of 'v' into 'u', and add common parts.
  if (size_u < size_v) {
    limb_t carry = add (u, u, v, size_u);
    for (size_t i = size_u; i<size_v; ++i)  u[i] = v[i];
    if (carry) {
      carry = add (u + size_u, size_v - size_u, carry);
      if (carry)  u[size_v++] = carry;
    }
    return size_v;
  }
  // Here 'size_u' >= 'size_v'
  limb_t carry = add (u, u, v, size_v);
  if (size_u > size_v)  carry = add (u + size_v, size_u - size_v, carry);
  if (carry)  u[size_u++] = carry;
  return size_u;
}
