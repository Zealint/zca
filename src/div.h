// Low-level division

#ifndef DIV_H
#define DIV_H

#pragma once

#include "consts.h"

#ifdef DIV_PREINVERSION



    // 2/1 division ('u1', 'u0') by 'd' with pre-inverted divisor.
    // 'u1' < 'd' and 'd'>0. 'd' is normalized. 'v' is a pre-inversion of 'd'.
    // Result in 'q', return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v);



    // 3/2 division ('u2', 'u1', 'u0') by ('d1', 'd0') with pre-inverted divisor.
    // ('u2', 'u1') < ('d1', 'd0'), ('d1', 'd0')>0 and is normalized. 'v' is a pre-inversion of ('d1', 'd0').
    // Result in 'q' and it will be returned.
    // Reminder in ('r1', 'r0').
limb_t __fastcall div_3_by_2_pre (limb_t &q, limb_t &r1, limb_t &r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v);



    // Calculate pre-inversion of 'd'.
    // 'd' is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_2_by_1 (limb_t d);



    // Calculate pre-inversion of ('d1', 'd0').
    // ('d1', 'd0') is normalized.
    // Return the pre-inverted element.
limb_t __fastcall inv_3_by_2 (limb_t d1, limb_t d0);



    // Inversion class
class DivInverse {
  DivInverse ( );  // Forbid empty constructor

  void __fastcall Invert1() { v = inv_2_by_1 (d1); }       // Invert limb [d1].
  void __fastcall Invert2() { v = inv_3_by_2 (d1, d0); };  // Invert vector of two limbs [d1:d0].

  void __fastcall Init (limb_t d); // Invert one limb
  void __fastcall Init (limb_t dh, limb_t dl); // Invert two limbs
  void __fastcall Init ( limb_t dh, limb_t dl, limb_t tail); // Invert two most significant limbs, when a vector contains more than two limbs.

public:
  limb_t v;      // The inversion
  limb_t d1, d0; // Normalized divisor (d0 is not used for 2/1 division).
  u8 shift;      // Shift for normalization.

  DivInverse (limb_t d) { Init (d); }
  DivInverse (limb_t d1, limb_t d0) { Init (d1, d0); }
  DivInverse (limb_t d1, limb_t d0, limb_t tail) { Init (d1, d0, tail); }
  DivInverse (const limb_t *d, size_t n) { Init (d[n-1], d[n-2], d[n-3]); }
};



#else



    // 2/1 division ('u1', 'u0') by 'd'.
    // 'u1' < 'd' and 'd'>0. 'd' is normalized.
    // Result in 'q', return the reminder.
limb_t __fastcall div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d);



    // 3/2 division ('u2', 'u1', 'u0') by ('d1', 'd0').
    // ('u2', 'u1') < ('d1', 'd0'), ('d1', 'd0')>0 and is normalized.
    // Result in 'q' and it will be returned.
    // Reminder in ('r1', 'r0').
limb_t __fastcall div_3_by_2 ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0 );



    // Normalization class
class DivNormal {
  DivNormal ( );  // Forbid empty constructor

  void __fastcall Init (limb_t d); // Normalize one limb
  void __fastcall Init (limb_t dh, limb_t dl); // Normalize two limbs
  void __fastcall Init ( limb_t dh, limb_t dl, limb_t tail); // Normalize two most significant limbs, when a vector contains more than two limbs.

public:
  limb_t d1, d0; // Normalized divisor (d0 is not used for 2/1 division).
  u8 shift;      // Shift for normalization.

  DivNormal (limb_t d) { Init (d); }
  DivNormal (limb_t d1, limb_t d0) { Init (d1, d0); }
  DivNormal (limb_t d1, limb_t d0, limb_t tail) { Init (d1, d0, tail); }
  DivNormal (const limb_t *d, size_t n) { Init (d[n-1], d[n-2], d[n-3]); }
};



#endif



    // n/1 division. (u_n, u_(n-1), ..., u_0) by d, where d>0 and d is normalized.
    // n > 0 and u_n < d.
    // Result in 'q', if q!=nullptr.
    // Return the reminder.
limb_t __fastcall div_n_by_1 (limb_t *q, const limb_t u_n, const limb_t *u, size_t n, limb_t d);



    // n/2 division. (u_n, u_(n-1), ..., u_0) by d=(d_1, d_0), where d>0 and d is normalized.
    // n > 1 and (u_n, u_(n-1)) < d.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'r', if r!=nullptr.
void __fastcall div_n_by_2 (limb_t *q, limb_t *r, const limb_t u_n, const limb_t *u, size_t n, limb_t d);



    // m/n division. (u_m, u_(m-1), ..., u_0) by d=(d_(n-1), ..., d_0), where d>0 and d is normalized.
    // n > 2.
    // Result in 'q', if q!=nullptr.
    // Reminder in 'u'.
void __fastcall div_m_by_n (limb_t *q, limb_t *u, const limb_t u_n, size_t size_u, const limb_t * d, size_t size_d, const DivInverse & inv);



  //limb_t __fastcall div_n_by_1 ( limb_t * q, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
  //void __fastcall div_n_by_2 ( limb_t * q, limb_t * r, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
  //void __fastcall div_m_by_n ( limb_t * q, limb_t * u, size_t & size_u, const limb_t * d, size_t size_d, const DivInverse & inv ) {



    // n/1 division (u_(n-1), ..., u_0) by d, where d>0 and n>0.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if 'r' != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d);



    // n/2 division (u_(n-1), ..., u_0) by d=(d1, d0), where d>0 and n>1.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if 'r' != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *r, const limb_t *u, size_t n, limb_t d1, limb_t d0);



    // m/n division (u_(m-1), ..., u_0) by d=(d_(n-1), ..., d_0), where d>0 and m>=n>=3.
    // u is normalized.
    // Result in 'q' if q != nullptr, reminder in 'r' if 'r' != nullptr.
    // Return size of quotient.
size_t __fastcall div_qr (limb_t *q, limb_t *u, size_t &size_u, const limb_t *d, size_t size_d);



#endif
