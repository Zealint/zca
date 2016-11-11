// Low-level multiplication. Here are prototypes with descriptions only.

#ifndef MUL_LOW_H
#define MUL_LOW_H
#pragma once

#include "typedef.h"

    // Multiply vector U={u,size} by limb v and then add limb c (c=0 by default).
    // Put the result into vector Z={z, size}, so Z=U*v+c.
    // Vector Z should have enough allocated memory (at least 'size' limbs).
    // Return carry (the value of z[size]). Limb z[size] will not be touched.
limb_t __fastcall mul_n_by_1 (limb_t *z, const limb_t *u, size_t size, limb_t v, limb_t c=0);

    // Calculate Z=U+V*w, where U and V are vectors of equal size 'size', and 'w' is a limb.
    // Z should have enough allocated memory.
    // Return carry (the value of z[size]). Limb z[size] will not be touched.
limb_t __fastcall addmul_n_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w);

    // Calculate Z=U-V*w, where U and V are vectors of equal size 'size', and 'w' is a limb.
    // Z should have enough allocated memory and size_u >= size_v.
    // Return borrow from z[size]. Limb z[size] will not be touched.
limb_t __fastcall submul_n_by_1 (limb_t *z, const limb_t *u, const limb_t *v, size_t size, limb_t w);

#endif
