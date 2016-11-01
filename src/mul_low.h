// Low-level multiplication. Here are prototypes with descriptions only.

#ifndef MUL_LOW_H
#define MUL_LOW_H
#pragma once

#include "config.h"
#include "typedef.h"

    // Multiply vector 'u' of size 'size' by limb 'v' and add limb 'c'.
    // Put the result into vector 'z'.
    // Vector 'z' should have enough memory (at least 'size' limbs).
    // Return carry.
limb_t __fastcall mul_N_by_1 (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c);

    // Calculate z=u+v*w, where 'u' and 'v' are vectors of equal size 'size', and 'w' is a limb.
    // 'z' should have enough memory.
    // Return carry.
limb_t __fastcall addmul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w);

    // Calculate z=u-v*w, where 'u' and 'v' are vectors of equal size 'size', and 'w' is a limb.
    // 'z' should have enough memory.
    // Return borrow.
limb_t __fastcall submul_N_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w);

#endif
