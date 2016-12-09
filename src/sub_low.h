// Low-level subtraction.

#ifndef SUB_LOW_H
#define SUB_LOW_H
#pragma once

#include "typedef.h"

    // Subtract one limb b from vector A={a, size_a} and put the result into vector C={c, size_c}.
    // C should have enough allocated memory (size_a limbs) and size_a > 0.
    // Return borrow from c[size_a]. Limb c[size_a] will not be touched.
limb_t __fastcall dec (limb_t *c, const limb_t *a, size_t size_a, limb_t b);

    // The same function as before, but 'in-place' (like if c=a).
limb_t __fastcall dec (limb_t * a, size_t size_a, limb_t b);

    // Subtract from vector A={a, size} vector B={b, size} when they have the same size 'size'.
    // Put the result into C={c, size}.
    // C should have enough allocated memory ('size' limbs) and 'size' > 0.
    // Return borrow from c[size], but c[size] will not be touched.
limb_t __fastcall sub (limb_t *c, const limb_t *a, const limb_t *b, size_t size);

#endif
