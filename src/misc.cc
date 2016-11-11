#include "typedef.h"
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
  for (size_t i=0; i<size; ++i)  z[i] = u[i];
}



void __fastcall copy_down (limb_t *z, const limb_t *u, size_t size) {
  size_t i = size;
  while (i-- > 0)  z[i] = u[i];
}



void __fastcall make_zero (limb_t *z, size_t size) {
  for (size_t i=0; i<size; ++i)  z[i] = 0;
}



size_t normalize_size (const limb_t *a, size_t n) {
  while (n>0 && a[n-1]==0)  --n;
  return n;
}


