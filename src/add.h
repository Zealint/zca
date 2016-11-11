// Vector addition. Here are prototypes with descriptions only.

#ifndef ADD_H
#define ADD_H
#pragma once

#include "typedef.h"

    // Add U={u, size_u} and a limb s.
    // The result is in Z={z, size_z}, where size_z is returned.
    // Vector U is normalized and Z should have enough allocated memory.
size_t __fastcall inc (limb_t *z, limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-pace' (as if z=u).
size_t __fastcall inc (limb_t *u, size_t size_u, limb_t s);

    // Add vector U={u, size_u} to vector V={v, size_v} with (possibly) different sizes.
    // The result is in Z={z, size_z}, where size_z is returned.
    // Vector Z should have enough allocated memory, U and V are normalized.
size_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // The same function as before, but 'in-place' (as if z=u).
size_t __fastcall add (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
