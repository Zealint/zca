// Low-level addition. Here are prototypes with descriptions only.

#ifndef ADD_LOW_H
#define ADD_LOW_H
#pragma once

#include "typedef.h"

    // Add one limb s to vector U={u, size_u} and put the result into vector Z={z, size_u}.
    // Z should have enough allocated memory (at least size_u limbs) and size_u > 0.
    // Return carry (the value of z[size_u]). Limb z[size_u] will not be touched.
limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-place' (like if z=u).
limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s);

    // Add vector U={u, size} to vector V={v, size} when they have the same size 'size'.
    // Put the result into Z={z, size}.
    // Z should have enough allocated memory (at least 'size' limbs) and 'size' > 0.
    // Return carry (i.e. z[size], but z[size] will not be touched).
limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size);

#endif
