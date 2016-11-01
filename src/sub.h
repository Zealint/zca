// Vector subtraction. Here are prototypes with descriptions only.

#ifndef SUB_H
#define SUB_H
#pragma once

#include "sub_low.h"

    // Decrement vector 'u' by limb 's', result in vector 'z'.
    // Vector 'u' is normalized and u >= s.
    // Return true if u >= s and false in other case (in this case the result in 'z' is unpredictable).
bool __fastcall dec (limb_t *z, limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-place' (as if z=u).
bool __fastcall dec (limb_t *u, size_t size_u, limb_t s);

    // Subtract from vector 'u' vector 'v' with (possibly) different sizes. Put the result into vector 'z'.
    // u >= v and 'z' should have enough allocated memory.
    // Return 'true' if u >= v and 'false' in other case (in this case 'z' is unpredictable).
bool __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

    // The same function as before but 'in-place' (as if z=u).
bool __fastcall sub (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
