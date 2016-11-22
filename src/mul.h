// Vector multiplication. Here are prototypes with descriptions only.

#ifndef MUL_H
#define MUL_H
#pragma once

#include "mul_low.h"

    // Multiply vector U={u, size_u} by vector V={v, size_v} by a trivial algorithm.
    // Put the result into Z={z, size_u+size_v}.
    // Z, U and V cannot overlap.
    // size_u>0 and size_v>0. Vector Z should have enough allocated memory (at least size_u+size_v limbs).
    // Return the value z[size_u+size_v-1].
limb_t __fastcall mul_n_by_m (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // Multiply vector U={u, size_u} by limb v, add limb c and put the result into Z={z, size_z}.
    // Z should have enough allocated memory and U is normalized.
    // Return size_z.
size_t mul (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c=0);

    // Calculate Z = U + V*w, when U and V may have different sizes.
    // Vectors U and V are normalized, Z should have enough allocated memory.
    // Return size of Z.
size_t addmul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w);

    // Calculate Z = U - V*w, when U and V may have different sizes.
    // Vectors U and V are normalized, Z should have enough allocated memory, also size_u >= size_v.
    // Return size of Z.
bool submul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v, limb_t w);

    // Multiply vector 'u' by vector 'v'.
    // Put the result into 'z'.
    // &z!=&u and &z!=&v.
    // size_u>0 and size_v>0. Vector 'z' should have enough memory.
    // Return size of 'z'.
size_t mul (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
