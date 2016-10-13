// Low-level addition

#ifndef ADD_H
#define ADD_H

#pragma once

#include "consts.h"



    // Add one limb 's' to vector 'u' of size 'size_u' and put the result into 'z'.
    // 'z' should have enough memory (at least 'size_u' limbs)
    // Return carry (if 'size_u'==0 then carry='s').
limb_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, limb_t s);



    // The same function as before, but 'in-place' (like if 'z'='u').
limb_t __fastcall add (limb_t *u, size_t size_u, limb_t s);



    // Add vector 'u' to vector 'v' when they have the same size 'size'.
    // Put the result into 'z'.
    // 'z' should have enough memory (at least 'size' limbs).
    // Return carry (i.e. 'z[size]', but 'z[size]' is not actually written).
limb_t __fastcall add (limb_t *z, const limb_t *u, const limb_t *v, size_t size);



    // Increment 'u' by 's', result in 'z'.
    // 'u' normalized and 'z' should have enough memory.
    // Return size of 'z'.
size_t __fastcall inc (limb_t *z, limb_t *u, size_t size_u, limb_t s);



    // The same function 'in-pace'.
size_t __fastcall inc (limb_t *u, size_t size_u, limb_t s);



    // Add vector 'u' to vector 'v' with (possibly) different sizes. Put the result into 'z'.
    // 'z' should have enough allocated memory, 'u' and 'v' are normalized.
    // Return size of the result.
size_t __fastcall add (limb_t *z, const limb_t *u, size_t size_u, const limb_t *v, size_t size_v);



    // The same function but 'in-place' (as if 'z'='u').
size_t __fastcall add (limb_t *u, size_t size_u, const limb_t *v, size_t size_v);

#endif
