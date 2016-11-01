// Configuration file.
// Here you can choose a variant of implementation of low-level functions.
// USE_ASM - 32-bit assembler implementation. If is not defined, then
//   USE_X64 - use 'unsigned long long' data type to work with limbs - very convenient and fast if you processor supports 64-bit integers (for example 64/32 division). If is not defined then
//     USE_BIT_TRICKS - use some branch free bit hacks to detect carry or borrow and in some other cases. May be slow on some processors, but fast on the others.
//     If bit tricks are not defined, then usual 'if'-algorithm will be used instead of bit-tricks.

#ifndef CONFIG_H
#define CONFIG_H
#pragma once

    // *** Uncomment to use assembler realization of low-level functions
//#define USE_ASM
#ifndef USE_ASM

      // *** Uncomment if your processor and compiler both support 64-bit integers
      // If so, many functions will have simpler implementation (carry and borrow detection, limb by limb multiply etc.)
  //#define USE_X64
  #ifndef USE_X64

        // *** Uncomment if you prefer branch-free 32-bit hacks instead of (possibly) conditional algorithms (for ex. carry and borrow detection etc.).
    #define USE_BIT_TRICKS

  #endif // #ifndef USE_X64

#endif // #ifndef USE_ASM

    // *** Uncomment if you prefer division with pre-inverted denominator. In other case it will use processor division.
#define DIV_PREINVERSION

#endif // CONFIG_H
