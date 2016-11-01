// Vector addition. Here are prototypes with descriptions only.

#ifndef ADD_H
#define ADD_H
#pragma once

#include "add_low.h"

    // Increment vector 'u' by limb 's'. The result in vector 'z'.
    // Vector 'u' is normalized and 'z' should have enough memory.
    // Return the resulting size of 'z'.
size_t __fastcall inc (limb_t *z, limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-pace' (as if z=u).
size_t __fastcall inc (limb_t *u, size_t size_u, limb_t s);

    // Add vector 'u' to vector 'v' with (possibly) different sizes. Put the result into 'z'.
    // Vector 'z' should have enough allocated memory, 'u' and 'v' are normalized.
    // Return size of the result.
size_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // The same function as before, but 'in-place' (as if z=u).
size_t __fastcall add (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
