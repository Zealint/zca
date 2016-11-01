// Vector division. Here are prototypes with descriptions only.

#ifndef DIV_H
#define DIV_H
#pragma once

#include "config.h"
#include "typedef.h"

#ifdef DIV_PREINVERSION

    // Calculate pre-inversion of d.
    // d is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_2_by_1 (limb_t d);

    // Calculate pre-inversion of (d1, d0).
    // (d1, d0) is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0);

    // n/1 division. (u_n, u_(n-1), ..., u_0) by d, where d>0 and d is normalized.
    // n > 0 and u_n < d.
    // Result in q, if q!=nullptr.
    // Return the reminder.
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, limb_t d, limb_t v);

    // n/2 division. (u_n, u_(n-1), ..., u_0) by d=(d1, d0), where d>0 and d is normalized.
    // n > 1 and (u_n, u_(n-1)) < d.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'r', if r!=nullptr.
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, limb_t d, limb_t v);

    // m/n division. (u_m, u_(m-1), ..., u_0) by d=(d_(n-1), ..., d_0), where d>0 and d is normalized.
    // n > 2.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'u'.
void __fastcall div_m_by_n (limb_t *q, limb_t *u, const limb_t u_n, size_t size_u, const limb_t * d, size_t size_d, limb_t d1, limb_t d0, limb_t v);

#else

    // n/1 division. (u_n, u_(n-1), ..., u_0) by d, where d>0 and d is normalized.
    // n > 0 and u_n < d.
    // Result in q, if q!=nullptr.
    // Return the reminder.
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, limb_t d);

    // n/2 division. (u_n, u_(n-1), ..., u_0) by d=(d1, d0), where d>0 and d is normalized.
    // n > 1 and (u_n, u_(n-1)) < d.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'r', if r!=nullptr.
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, limb_t d);

    // m/n division. (u_m, u_(m-1), ..., u_0) by d=(d_(n-1), ..., d_0), where d>0 and d is normalized.
    // n > 2.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'u'.
void __fastcall div_m_by_n (limb_t *q, limb_t *u, const limb_t u_n, size_t size_u, const limb_t * d, size_t size_d, limb_t d1, limb_t d0);

#endif

    // n/1 division (u_(n-1), ..., u_0) by d, where d>0 and n>0.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if r != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d);

    // n/2 division (u_(n-1), ..., u_0) by d=(d1, d0), where d>0 and n>1.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if 'r' != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d1, limb_t d0);

    // m/n division (u_(m-1), ..., u_0) by d=(d_(n-1), ..., d_0), where d>0 and m>=n>=3.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if r != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d);

#endif
