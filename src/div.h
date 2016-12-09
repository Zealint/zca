// Vector division.

#ifndef DIV_H
#define DIV_H
#pragma once

#include "typedef.h"

    // n/1 division (a_(n-1), ..., a_0) by d, where d>0 and n>0.
    // {a, size_a} is normalized, size_a == m. q != r.
    // Result in q if q != nullptr, reminder in r if r != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *a, size_t size_a, limb_t d);

    // n/2 division (a_(n-1), ..., a_0) by d=(d1, d0), where d>0 and n>1.
    // {a, size_a} is normalized, size_a == m. q != r.
    // Result in 'q' if q != nullptr, reminder in 'r' if 'r' != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *a, size_t size_a, limb_t d1, limb_t d0);

    // m/n division (a_(m-1), ..., a_0) by d=(d_(n-1), ..., d_0), where d>0 and m>=n>=3.
    // {a, size_a} is normalized, size_a == m. q != a.
    // Result in 'q' if q != nullptr, reminder in {a, size_a}.
    // Return size of quotient if q != nullptr. Otherwise return 0.
size_t __fastcall div_qr (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d);

#endif
