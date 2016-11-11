// Vector subtraction. Here are prototypes with descriptions only.

#ifndef SUB_H
#define SUB_H
#pragma once

#include "sub_low.h"

    // Subtract from vector U={u, size_u} a limb s.
    // Vector U is normalized and Z should have enough allocated memory.
    // The result is Z={z, size_u}, but it may not be normalized.
    // Return true if U>=s (not borrow has accrue), return false otherwise.
bool __fastcall dec (limb_t *z, limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-place' (as if z=u).
bool __fastcall dec (limb_t *u, size_t size_u, limb_t s);

    // Subtract from vector U={u, size_u} vector V={v, size_v} with (possibly) different sizes.
    // Vector Z should have enough allocated memory, U and V are normalized, and size_u >= size_v.
    // The result is Z={z, size_u}.
		// Return true if U>=V (not borrow has accrue), return false otherwise.
bool __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // The same function as before but 'in-place' (as if z=u).
bool __fastcall sub (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
