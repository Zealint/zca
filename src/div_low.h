// Low-level division.

#ifndef DIV_LOW_H
#define DIV_LOW_H
#pragma once

#include "typedef.h"

    // 2/1 division (a1, a0) by d.
    // a1 < d and d>0. Also d is normalized.
    // Result in q, return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t a1, limb_t a0, limb_t d);

    // 3/2 division (a2, a1, a0) by (d1, d0).
    // (a2, a1) < (d1, d0), (d1, d0)>0 and is normalized.
    // &q != &r1 and &q != &r0, otherwise the result may be unpredictable.
    // Result in q. Reminder in (r1, r0).
void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t a2, limb_t a1, limb_t a0, limb_t d1, limb_t d0);

		// Division with pre-inversion
#ifdef DIV_PREINVERSION

    // Calculate pre-inversion of d.
    // d is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_2_by_1 (limb_t d);

    // Calculate pre-inversion of (d1, d0).
    // (d1, d0) is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0);

    // 2/1 division (a1, a0) by d with pre-inverted divisor v.
    // a1 < d and d>0. d is normalized.
    // Result in q, return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t a1, limb_t a0, limb_t d, limb_t v);

    // 3/2 division (a2, a1, a0) by (d1, d0) with pre-inverted divisor v.
    // (a2, a1) < (d1, d0), (d1, d0)>0 and is normalized.
    // &q != &r1 and &q != &r0, otherwise the result may be unpredictable.
    // Result in q. Reminder in (r1, r0).
void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v);

#endif

    // n/1 division. (a_n, a_(n-1), ..., a_0) by d, where d>0 and d is normalized. v is pre-inversion of d.
    // n > 0 and a_n < d.
    // q should have enought allocated memory.
    // Result in q. Return the reminder.
#ifdef DIV_PREINVERSION
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t a_n, const limb_t *a, size_t n, limb_t d, limb_t v);
#else
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t a_n, const limb_t *a, size_t n, limb_t d);
#endif

    // n/1 reminder. (a_n, a_(n-1), ..., a_0) by d, where d>0 and d is normalized. v is pre-inversion of d.
    // n > 0 and a_n < d.
    // Return the reminder.
#ifdef DIV_PREINVERSION
limb_t __fastcall mod_n_by_1 (const limb_t a_n, const limb_t *a, size_t n, limb_t d, limb_t v);
#else
limb_t __fastcall mod_n_by_1 (const limb_t a_n, const limb_t *a, size_t n, limb_t d);
#endif

    // n/2 division. (a_n, a_(n-1), ..., a_0) by d=(d1, d0), where d>0 and d is normalized. v is pre-inversion of (d1, d0).
    // n > 1 and (a_n, a_(n-1)) < (d1, d0).
    // Result in q, it should have enought allocated memory.
    // Reminder in r, if r!=nullptr.
#ifdef DIV_PREINVERSION
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0, limb_t v);
#else
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0);
#endif

    // n/2 reminder. (a_n, a_(n-1), ..., a_0) by d=(d1, d0), where d>0 and d is normalized. v is pre-inversion of (d1, d0).
    // n > 1 and (a_n, a_(n-1)) < (d1, d0).
    // Reminder in (r1, r0).
#ifdef DIV_PREINVERSION
void __fastcall mod_n_by_2 (limb_t &r1, limb_t &r0, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0, limb_t v);
#else
void __fastcall mod_n_by_2 (limb_t &r1, limb_t &r0, const limb_t a_n, const limb_t *a, size_t n, limb_t d1, limb_t d0);
#endif

    // m/n division. a=(a_(m-1), ..., a_0) by d=(d_(n-1), ..., d_0), where d>0 and d is normalized.
    // m >= n > 2. m=size_a, n=size_d. v is pre-inversion of d.
    // {a, size_a} is normalized.
    // Result in q, if q!=nullptr.
    // Reminder in 'a'.
#ifdef DIV_PREINVERSION
void __fastcall div_m_by_n (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d, limb_t v);
#else
void __fastcall div_m_by_n (limb_t *q, limb_t *a, size_t &size_a, const limb_t *d, size_t size_d);
#endif

#endif

