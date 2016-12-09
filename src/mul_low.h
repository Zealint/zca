// Low-level multiplication.

#ifndef MUL_LOW_H
#define MUL_LOW_H
#pragma once

#include "typedef.h"

    // Multiply vector A={a, size} by limb b and then add limb s (s=0 by default).
    // Put the result into vector C={c, size}, so C=A*b+s.
    // Vector C should have enough allocated memory (at least 'size' limbs).
    // Return carry (the value of c[size]). Limb c[size] will not be touched.
limb_t __fastcall mul_n_by_1 (limb_t *c, const limb_t *a, size_t size, limb_t b, limb_t s=0);

    // Calculate C=A+B*s, where A and B are vectors of equal size 'size', and s is a limb.
    // C should have enough allocated memory.
    // Return carry (the value of c[size]). Limb c[size] will not be touched.
limb_t __fastcall addmul_n_by_1 (limb_t *c, const limb_t *a, const limb_t *b, size_t size, limb_t s);

    // Calculate C=A-B*s, where A and B are vectors of equal size 'size', and s is a limb.
    // C should have enough allocated memory and.
    // Return borrow from c[size]. Limb c[size] will not be touched.
limb_t __fastcall submul_n_by_1 (limb_t *c, const limb_t *a, const limb_t *b, size_t size, limb_t s);

    // Multiply vector A={a, size_a} by vector B={b, size_b} by a trivial algorithm.
    // Put the result into C={c, size_a+size_b}.
    // C cannot overlap with A or B. 
    // size_a>0 and size_b>0. Vector C should have enough allocated memory (at least size_a+size_b limbs).
    // Return the value c[size_a+size_b-1].
limb_t __fastcall mul_n_by_m (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

#endif
