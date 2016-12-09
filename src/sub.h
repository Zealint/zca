// Vector subtraction.

#ifndef SUB_H
#define SUB_H
#pragma once

#include "typedef.h"

    // Subtract from vector A={a, size_a} a limb b.
    // Vector A is normalized and C should have enough allocated memory.
    // A >= b (assert inside).
    // The result is C={c, size_c}, and size_c is returned.
size_t __fastcall sub (limb_t *c, limb_t *a, size_t size_a, limb_t b);

    // The same function as before, but 'in-place' (as if c=a).
size_t __fastcall sub (limb_t *a, size_t size_a, limb_t b);

    // Subtract from vector A={a, size_a} vector B={b, size_b} with (possibly) different sizes.
    // Vector C should have enough allocated memory, A and B are normalized, and A >= B.
    // The result is C={c, size_c}, and size_c is returned.
size_t __fastcall sub (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

    // The same function as before but 'in-place' (as if c=a).
size_t __fastcall sub (limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

#endif
