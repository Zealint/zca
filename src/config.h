// Configuration

#ifndef CONFIG_H
#define CONFIG_H

#pragma once

    // *** Uncomment to use assembler realization of low-level functions
//#define USE_ASM

#ifndef USE_ASM

    // *** Uncomment if your processor and compiler both support 64-bit integers
    // If so, many functions will have simpler implementation (carry and borrow detection, limb by limb multiply etc.)
#define USE_X64

#ifndef USE_X64

    // *** Uncomment if you prefer branch-free 32-bit hacks instead of (possibly) conditional algorithms (for ex. carry and borrow detection etc.).
//#define USE_BIT_TRICKS

#endif // NOT USE_X64

#endif // NOT USE_ASM

    // *** Uncomment if you prefer division with pre-inverted denominator. In other case it will be 64/32 processor division.
#define DIV_PREINVERSION

#endif // CONFIG_H
