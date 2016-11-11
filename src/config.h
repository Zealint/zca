// Configuration file.
// Here you can choose a variant of implementation of low-level functions and also a limb size.
// You can change only ***-marked definitions.
// USE_ASM - assembler implementation of low-level functions.
// USE_DLIMB - use data type which is double to limb_t to work with limbs - very convenient and fast if you processor supports 64-bit integers in case of limb_t is 32 bits (for example 64/32 division).
// If it is not defined then hlimb_t (half-limb) is supported.

#ifndef CONFIG_H
#define CONFIG_H
#pragma once

    // *** Define a limb size (8, 16, 32, or 64 bits) . It will automatically change some types in "typedef.h"
#define LIMB_BITS 32



    // *** Uncomment to use assembler realization of low-level functions
    // !!! IS NOT IMPLEMENTED YET, SO SHOULD BE COMMENTED !!!
//#define USE_ASM



    // *** Uncomment if your processor and compiler both support dlimb_t integers
    // If so, many functions will have simpler implementation (carry and borrow detection, limb by limb multiply etc.) Otherwise, you will have hlimb_t for half-limb, to use Lo_Hi-technic
#define USE_DLIMB



    // *** Uncomment if you prefer division with pre-inverted denominator. In other case it will use usual division.
#define DIV_PREINVERSION



#endif // CONFIG_H
