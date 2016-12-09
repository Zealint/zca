// Low- and high-level bit operations.

#ifndef BIT_H
#define BIT_H
#pragma once

#include "typedef.h"

    // Short left shift vector A={a, size_a} by 'shift' bits and put the result into c.
    // c should have enough memory (at least 'size_a' limbs) and 0 < shift < LIMB_BITS, size_a>0.
    // Return carry (the value of c[size_a]). Limb c[size_a] will not be touched.
limb_t __fastcall shift_left_short (limb_t *c, const limb_t *a, size_t size_a, u8 shift);

    // Short right shift vector 'u' of size 'size_u' by 'shift' bits and put the result into 'z'.
    // 'z' should have enough memory and 0 < 'shift' < LIMB_BITS, 'size_u'>0.
    // Return tail - limb of that bits, what are shifted below z[0].
limb_t __fastcall shift_right_short (limb_t *z, const limb_t *u, size_t size_u, u8 shift);

    // Long left shift vector 'u' of size 'size_u' by 'shift' bits and put the result into 'z'.
    // 'z' should have enough memory (at least 'size_u' limbs), 'size_u' > 0.
    // 'u' is normalized.
    // Return new size of 'z'.
size_t __fastcall shift_left_long (limb_t *z, const limb_t *u, size_t size_u, bitcnt_t shift);

    // Long right shift vector 'u' of size 'size_u' by 'shift' bits and put the result into 'z'.
    // 'z' should have enough memory, 'size_u' > 0.
    // 'u' is normalized.
    // Return new size of 'z'.
size_t __fastcall shift_right_long (limb_t *z, const limb_t *u, size_t size_u, bitcnt_t shift);

#endif
