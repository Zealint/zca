// Low-level addition.

#ifndef ADD_LOW_H
#define ADD_LOW_H
#pragma once

#include "typedef.h"

    // Add one limb b to vector A={a, size_a} and put the result into vector C={c, size_a}.
    // C should have enough allocated memory (at least size_a limbs) and size_a > 0.
    // Return carry (the value of c[size_a]). Limb c[size_a] will not be touched.
limb_t __fastcall inc (limb_t *c, const limb_t *a, size_t size_a, limb_t b);

    // The same function as before, but 'in-place' (like if c=a).
limb_t __fastcall inc (limb_t *a, size_t size_a, limb_t b);

    // Add vector A={a, size} to vector B={b, size} when they have the same size 'size'.
    // Put the result into C={c, size}.
    // C should have enough allocated memory (at least 'size' limbs) and 'size' > 0.
    // Return carry (i.e. c[size], but c[size] will not be touched).
limb_t __fastcall add (limb_t *c, const limb_t *a, const limb_t *b, size_t size);

#endif
