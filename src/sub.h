// Low-level subtraction

#ifndef SUB_H
#define SUB_H

#pragma once

#include "consts.h"

    // Subtract one limb 's' from vector 'u' of size 'size_u' and put the result into 'z'.
    // Return borrow (if 'size_u'==0 then borrow='s').
limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s);



    // The same function as before, but 'in-place' (like if 'z'='u').
limb_t __fastcall sub (limb_t * u, size_t size_u, limb_t s);



    // Subtract from vector 'u' vector 'v' when they have the same size 'size'.
    // Put the result into 'z'.
    // Return borrow.
limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size);



    // Decrement 'u' by 's', result in 'z'.
    // 'u' is normalized and 'u' >= 's'.
    // Return 'true' is 'u' >= 's' and 'false' in other cases.
bool __fastcall dec (limb_t *z, limb_t *u, size_t size_u, limb_t s);



    // The same function 'in-pace
bool __fastcall dec (limb_t *u, size_t size_u, limb_t s);



    // Subtract from vector 'u' vector 'v' with (possibly) different sizes. Put the result into 'z'.
    // 'u' >= 'v' and 'z' should have enough allocated memory.
    // Return 'true' if 'u' >= 'v' and 'false' in other cases (in this case 'z' is unpredictable).
bool __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);



    // The same function but 'in-place' (as if 'z'='u').
bool __fastcall sub (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
