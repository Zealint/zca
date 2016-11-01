// Low-level subtraction. Here are prototypes with descriptions only.

#ifndef SUB_LOW_H
#define SUB_LOW_H
#pragma once

#include "config.h"
#include "typedef.h"

    // Subtract one limb 's' from vector 'u' of size 'size_u' and put the result into 'z'.
    // Vector 'z' should have enough memory.
    // Return borrow (if size_u==0 then borrow=s).
limb_t __fastcall sub (limb_t *z, const limb_t *u, size_t size_u, limb_t s);

    // The same function as before, but 'in-place' (like if z=u).
limb_t __fastcall sub (limb_t * u, size_t size_u, limb_t s);

    // Subtract from vector 'u' vector 'v' when they have the same size 'size'. Put the result into 'z'. 
    // Vector 'z' should have enough memory.
    // Return borrow.
limb_t __fastcall sub (limb_t *z, const limb_t *u, const limb_t *v, size_t size);

#endif
