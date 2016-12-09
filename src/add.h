// Vector addition. 

#ifndef ADD_H
#define ADD_H
#pragma once

#include "typedef.h"

    // Add A={a, size_a} and a limb b.
    // The result is in C={c, size_c}, where size_c is returned.
    // Vector A is normalized and C should have enough allocated memory.
size_t __fastcall add (limb_t *c, limb_t *a, size_t size_a, limb_t b);

    // The same function as before, but 'in-pace' (as if c=a).
size_t __fastcall add (limb_t *a, size_t size_a, limb_t b);

    // Add vector A={a, size_a} to vector B={b, size_b} with (possibly) different sizes.
    // The result is in C={c, size_c}, where size_c is returned.
    // Vector C should have enough allocated memory, A and B are normalized.
size_t __fastcall add (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

    // The same function as before, but 'in-place' (as if c=a).
size_t __fastcall add (limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

#endif
