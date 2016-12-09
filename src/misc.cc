#include <cmath>
#include "typedef.h"
#include "add.h"
#include "div.h"
#include "mul.h"
#include "misc.h"



u8 __fastcall count_lz (u8 x) {
  u8 count = 0;
  if ((x & 0xF0 ) == 0)  count += 4;
  else x >>= 4;
  if ((x & 0xC ) == 0)  count += 2;
  else x >>= 2;
  if ((x & 2) == 0)  count += 1;
  else x >>= 1;
  count += (1-x);
  return count;
}

u8 __fastcall count_lz (u16 x) {
  u8 count=0;
  if ((x&0xFF00UL)==0)  count = 8;
  else  x >>= 8;
  return count + count_lz (u8(x));
}

u8 __fastcall count_lz (u32 x) {
  u8 count=0;
  if ((x&0xFFFF0000UL)==0)  count = 16;
  else  x >>= 16;
  return count + count_lz (u16(x));
}

u8 __fastcall count_lz (u64 x) {
  u8 count=0;
  if ((x&0xFFFFFFFF00000000ULL)==0)  count = 32;
  else  x >>= 32;
  return count + count_lz (u32(x));
}



bool __fastcall can_copy_up (const limb_t *z, const limb_t *u, size_t size) {
	return z<=u || z>=u+size;
}



void __fastcall copy_up (limb_t *z, const limb_t *u, size_t size) {
  assert (size > 0);
  for (size_t i=0; i<size; ++i)  z[i] = u[i];
}



void __fastcall copy_down (limb_t *z, const limb_t *u, size_t size) {
  assert (size > 0);
  size_t i = size;
  while (i-- > 0)  z[i] = u[i];
}



void __fastcall set_zero (limb_t *a, size_t size) {
  assert (size > 0);
  for (size_t i=0; i<size; ++i)  a[i] = 0;
}

void __fastcall set_value (limb_t *a, size_t size, limb_t v) {
  assert (size > 0);
  for (size_t i=0; i<size; ++i)  a[i] = v;
}

/*
bool __fastcall is_equal (const limb_t *a, const limb_t* b, size_t size) {
  assert (size > 0);
  for (size_t i=0; i<size; ++i)  if (a[i] != b[i])  return false;
  return true;
}
*/




size_t normalize_size (const limb_t *a, size_t n) {
  while (n>0 && a[n-1]==0)  --n;
  return n;
}

bool __fastcall is_normalized (const limb_t *a, size_t n) { return n==0 || a[n-1]!=0; }


char __fastcall char_by_digit (u8 d) {
  if (d<10)  return (char)(d+'0');
  return (char)(d-10+'A');
}

u8 __fastcall digit_by_char (char c) {
  if ('0'<=c && c<='9')  return (u8)(c-'0');
  if ('a'<=c && c<='z')  return (u8)(c-'a'+10);
  if ('A'<=c && c<='Z')  return (u8)(c-'A'+10);
  return (u8)(-1);
}

bool __fastcall is_digit (u8 c, u8 base) {
  return digit_by_char(c) < base;
}



char * __fastcall to_string (char *str, const limb_t *u, size_t size_u, u8 base) {
  if (size_u == 0) {
    str[0] = '0';
    str[1] = '\0';
    return str;
  }
  size_t n = 0;
  if (base == 16) {
    for (size_t i=0; i<size_u; ++i) {
      limb_t a = u[i];
      for (size_t j=0; j<LIMB_BITS/4 && (a>0 || i<size_u-1); ++j, a>>=4)  str[n++] = char_by_digit((u8)(a&0xF));
    }
  } else {
    limb_t *z = new limb_t[size_u];
    copy_up (z, u, size_u);
    while (size_u > 0) {
      limb_t r;
      size_u = ::div_qr (z, &r, z, size_u, (limb_t)base);
      str[n++] = char_by_digit(r);
    }
    delete[] z;
  }
  str[n] = '\0';
  size_t i=0, j=n-1;	// Reverse string
  while (i<j)  swap (str[i++], str[j--]);
  return str;
}



size_t __fastcall from_string (limb_t *z, const char *str, u8 base) {
  while (*str == '0')  ++str;	// Remove leading zeroes
  if (!is_digit (*str, base))  return 0;
  size_t size = 0;
  if (base == 16) {	// Simple case of hexadecimal numbers
    size_t k = 0, n = 0;
    limb_t a = 0;
    while (is_digit (str[n], base))  ++n;	// Get the length of the string
    while (n-- > 0) {
      a |= (limb_t)digit_by_char (str[n]) << (4*k);
      if (++k == LIMB_BITS/4) {	// Enough digits are 'packed' into limb
        z[size++] = a;
        k = 0;
        a = 0;
      }
    }
    if (a > 0)  z[size++] = a;
  } else {
    while (is_digit (*str, base)) {
      size = ::mul (z, z, size, (limb_t)base, (limb_t)digit_by_char(*str));
      ++str;
    }
  }
  return size;
}

size_t how_many_limbs (size_t d, u8 base) {
	// !!! It is wrong (too many) !!!
	return (size_t)ceil(log((double)base)/log(pow(2.0, LIMB_BITS)) * d);
}

size_t how_many_digits (const char *str, u8 base) {
  //sign_t s = 1;
  while (*str == '0')  ++str;	// Remove leading zeroes.
	//if (*str == '-')  s = -1;
  if (*str=='-' || *str=='+')  ++str;	// Remove possible unary plus or minus.
  size_t d=0;	// How many digits do we have?
  while (is_digit(str[d], base))  ++d;
  d += (d==0);	// In case of d=0 we will have d=1.
	//return s>0 ? d : -d;
	return d;
}

