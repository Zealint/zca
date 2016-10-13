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
