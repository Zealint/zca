// Vector multiplication. Here are prototypes with descriptions only.

#ifndef MUL_H
#define MUL_H
#pragma once

#include "mul_low.h"

    // Multiply vector 'u' by vector 'v' by a trivial algorithm.
    // Put the result into 'z'.
    // &z!=&u and &z!=&v. 
    // size_u>0 and size_v>0. Vector 'z' should have enough memory.
    // Return 'z[size_u+size_v-1]'.
limb_t __fastcall mul_N_by_M (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // Multiply vector 'u' by limb 'v' and put the result into 'z'.
    // 'z' should have enough memory and 'u' is normalized.
    // Return size of 'z'.
size_t mul1 (limb_t *z, const limb_t *u, size_t size, limb_t v);

    // Calculate z = u + v*w, when u and v may have different sizes.
    // 'u' and 'v' are normalized, 'z' should have enough memory.
    // Return size of 'z'.
size_t addmul1 (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w);

    // Calculate z = u - v*w, when 'u' and 'v' may have different sizes.
    // u>=v*w, 'u' and 'v' are normalized, 'z' should have enough memory.
    // Return true if the first condition is true, else return false.
bool submul1 (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w);

    // Multiply vector 'u' by vector 'v'.
    // Put the result into 'z'.
    // &z!=&u and &z!=&v. 
    // size_u>0 and size_v>0. Vector 'z' should have enough memory.
    // Return size of 'z'.
size_t mul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
