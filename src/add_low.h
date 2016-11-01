// Low-level addition. Here are prototypes with descriptions only.

#ifndef ADD_LOW_H
#define ADD_LOW_H
#pragma once

#include "config.h"
#include "typedef.h"

    // Add one limb 's' to vector 'u' of size 'size_u' and put the result into vector 'z'.
    // 'z' should have enough memory (at least 'size_u' limbs).
    // Return carry (if size_u==0 then carry=s).
limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-place' (like if z=u).
limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s);

    // Add vector 'u' to vector 'v' when they have the same size 'size'.
    // Put the result into 'z'.
    // 'z' should have enough memory (at least 'size' limbs).
    // Return carry (i.e. 'z[size]', but 'z[size]' is not actually written).
limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size);

#endif
