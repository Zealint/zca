// Low-level division without preinversion. Here are prototypes with descriptions only.

#ifndef DIV_LOW_H
#define DIV_LOW_H
#pragma once

#include "config.h"
#include "typedef.h"

#ifndef DIV_PREINVERSION


    // 2/1 division (u1, u0) by d.
    // u1 < d and d>0. Also d is normalized.
    // Result in q, return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d);

    // 3/2 division (u2, u1, u0) by (d1, d0).
    // (u2, u1) < (d1, d0), (d1, d0)>0 and it is normalized.
    // Result in q. Reminder in (r1, r0).
void __fastcall div_3_by_2 (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0);

#endif
#endif
