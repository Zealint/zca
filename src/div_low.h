// Low-level division without preinversion. Here are prototypes with descriptions only.

#ifndef DIV_LOW_H
#define DIV_LOW_H
#pragma once

#include "typedef.h"

    // 2/1 division (u1, u0) by d.
    // u1 < d and d>0. Also d is normalized.
    // Result in q, return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d);

    // 3/2 division (u2, u1, u0) by (d1, d0).
    // (u2, u1) < (d1, d0), (d1, d0)>0 and is normalized.
    // Result in q. Reminder in (r1, r0).
void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0);

		// Division with pre-inversion
#ifdef DIV_PREINVERSION

    // 2/1 division (u1, u0) by d with pre-inverted divisor v.
    // u1 < d and d>0. d is normalized.
    // Result in q, return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v);

    // 3/2 division (u2, u1, u0) by (d1, d0) with pre-inverted divisor v.
    // (u2, u1) < (d1, d0), (d1, d0)>0 and is normalized.
    // Result in q. Reminder in (r1, r0).
void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v);

#endif

#endif
