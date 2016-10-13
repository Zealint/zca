// Low-level addition

#include "add.h"
#include "misc.h"

#ifndef USE_ASM

#ifdef USE_X64

// Here we use double limb (64 bit) to detect carry

limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; i<size_u; ++i) {
    t += u[i];
    z[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  return (limb_t)t;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
  dlimb_t t = (dlimb_t)s;
  for (size_t i=0; t>0 && i<size_u; ++i) {
    t += u[i];
    u[i] = (limb_t)t;
    t >>= LIMB_BITS;
  }
  return (limb_t)t;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  dlimb_t s = 0;
  for (size_t i=0; i<size; ++i) {
    s += u[i];
    s += v[i];
    z[i] = (limb_t)s;
    s >>= LIMB_BITS;
  }
  return (limb_t)s;
}

#else
#ifdef USE_BIT_TRICKS

// Here we use bit tricks to detect carry

limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i];
    limb_t r = s + t;
    s = ((s&t) | ((s|t)&(~r))) >> (LIMB_BITS-1);
    z[i] = r;
  }
  return s;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
  for ( size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i];
    limb_t r = s + t;
    s = ((s&t) | ((s|t)&(~r))) >> (LIMB_BITS-1);
    u[i] = r;
  }
  return s;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  limb_t carry = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i], t = v[i];
    limb_t r = s + t + carry;
    carry = ((s&t) | ((s|t)&(~r))) >> (LIMB_BITS-1);
    z[i] = r;
  }
  return carry;
}

#else // USE_BIT_TRICKS

// Here we use comparison to detect carry

limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t < s);
    z[i] = t;
  }
  return s;
}



limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s) {
  for (size_t i=0; s>0 && i<size_u; ++i) {
    limb_t t = u[i] + s;
    s = (t<s);
    u[i] = t;
  }
  return s;
}



limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size) {
  limb_t carry = 0;
  for (size_t i=0; i<size; ++i) {
    limb_t s = u[i] + carry;
    carry = (s<carry);
    limb_t t = s + v[i];
    carry += (t<s);
    z[i] = t;
  }
  return carry;
}

#endif // USE_BIT_THICKS

#endif // USE_X64

#endif // USE_ASM



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
