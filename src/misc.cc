#include "consts.h"
#include "misc.h"

u8 __fastcall count_lz (limb_t x) {
  u8 count = 0;
  if ((x & 0xFFFF0000) == 0)  count += 16;
  else x >>= 16;
  if ((x & 0xFF00) == 0)  count += 8;
  else x >>= 8;
  if ((x & 0xF0 ) == 0)  count += 4;
  else x >>= 4;
  if ((x & 0xC ) == 0)  count += 2;
  else x >>= 2;
  if ((x & 2) == 0)  count += 1;
  else x >>= 1;
  if (x == 0 )  count += 1;
  return count;
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



