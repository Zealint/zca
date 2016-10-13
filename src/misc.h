#ifndef MISC_H
#define MISC_H

#pragma once

#include "typedef.h"



template <typename T>
inline void __fastcall swap (T &a, T &b) {
  T t = a;
  a = b;
  b = t;
}



template <typename T>
inline T __fastcall max (T a, T b) {
  if (a >= b)  return a;
  return b;
}



template <typename T>
inline T __fastcall min (T a, T b) {
  if (a <= b)  return a;
  return b;
}



inline size_t __fastcall abs (offset_t a) {
  if (a < 0)  a = -a;
  return (size_t) a;
}



inline sign_t __fastcall sign (offset_t a) {
  return sign_t (a > 0) - sign_t (a < 0);
}


    // Counting leading zeros.
u8 __fastcall count_lz (limb_t x);



#endif
