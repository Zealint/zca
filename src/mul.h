// Vector multiplication. Here are prototypes with descriptions only.

#ifndef MUL_H
#define MUL_H
#pragma once

#include "typedef.h"

    // Multiply vector A={a, size_a} by limb b, add limb s (s=0 by default) and put the result into C={c, size_c}.
    // C should have enough allocated memory and A is normalized.
    // Return size_c.
size_t mul (limb_t *c, const limb_t *a, size_t size_a, limb_t b, limb_t s=0);

    // Calculate C = A + B*s.
    // Vectors A and B are normalized, C should have enough allocated memory.
    // Return size of C.
size_t addmul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b, limb_t s);

    // Calculate C = A - B*s.
    // Vectors A and B are normalized, C should have enough allocated memory, also A >= B*s.
    // Return size of C.
size_t submul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b, limb_t s);

    // Multiply vector A by vector B.
    // Put the result into C.
    // C cannot overlap with A or B. 
    // Vector C should have enough memory.
    // Return size of C.
size_t mul (limb_t *c, const limb_t *a, size_t size_a, const limb_t *b, size_t size_b);

#endif
