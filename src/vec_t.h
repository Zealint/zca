// Class 'vec_t' - to better work with vectors of limbs

#ifndef VEC_T_H
#define VEC_T_H

#pragma once

#include "consts.h"
#include "add.h"
#include "sub.h"



class vec_t {
  public:
    size_t size;    // Size of (not necessary normalized) vector. 'size' is NEVER exceed the number of allocated limbs.
    limb_t *limbs;  // Array of limbs.



    		// Constructors and destructor
    vec_t(): size(0), limbs (nullptr) {} // Empty vector is initially zero.
    explicit vec_t (size_t s): size(0), limbs (new limb_t[s]) {} // Not empty vector has initially zero value.
    vec_t (const vec_t &that): limbs(new limb_t[that.size]) { *this = that; }
    vec_t (vec_t &&that): size (that.size), limbs (that.limbs) { that.limbs = nullptr; }
    ~vec_t() { delete[] limbs; }



        // Resize 'limbs'.
        // It doesn't copy old limbs to new. ALL data will be cleared.
        // Vector becomes zero.
    void __fastcall Resize (size_t s);



        // Adapt 'limbs' to new size 's'.
        // Nothing is changed if 'size' >= 's'.
    void __fastcall Adapt (size_t s);



				// Normalize vector size.
				// Return normalized size.
    size_t __fastcall Normalize();



    		// Compare with other vector with the same size.
    		// Both vectors are normalized, 'size'='that.size' and 'size'>0.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall CompareEqualSize (const vec_t &that) const;



    		// Compare with other vector.
    		// Both vectors are normalized.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall Compare (const vec_t &that) const;



    		// Get the copy of other vector without memory allocation.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    vec_t & __fastcall operator = (const vec_t &that);



    		// Move the other vector to itself.
        // Return the reference to itself.
        // The other vector becomes empty (that.limbs=nullptr).
    vec_t & __fastcall operator = (vec_t &&v);



    		// Helpers [] for mutable and constant reference.
    limb_t & __fastcall operator [] (size_t index) { return limbs[index]; }
    const limb_t & __fastcall operator [] (size_t index) const { return limbs[index]; }



        // Check if vector is zero
    bool __fastcall IsZero() const { return size == 0; }



        // Set vector to zero
    void __fastcall SetZero() { size = 0; }



        // Add to itself limb 's'.
        // Enough memory for itself should be allocated.
        // Return constant reference to itself.
    const vec_t & __fastcall Inc (limb_t s);



        // Add to itself vector 'v'.
        // Enough memory for itself should be allocated.
        // Return constant reference to itself.
    const vec_t & __fastcall Add (const vec_t &v);



        // Subtract from itself limb 's'.
        // Itself >= 's'.
        // Return constant reference to itself.
    const vec_t & __fastcall Dec (limb_t s);



        // Subtract from itself vector 'v'.
        // Itself >= 'v'.
        // Return constant reference to itself.
    const vec_t & __fastcall Sub (const vec_t &v);



        // Multiply itself by limb 'v'.
        // Itself should have enough memory.
        // Return constant reference to itself.
    const vec_t & __fastcall Mul1 (limb_t v);



        // Multiply itself by vector 'v'.
        // Itself should have enough memory.
        // Return constant reference to itself.
    const vec_t & __fastcall Mul (const vec_t &v);



        // Divide itself by limb 'v'.
        // Return constant reference to itself.
    const vec_t & __fastcall Div1 (limb_t v);



        // Reminder of division itself by limb 'v'.
        // Return constant reference to itself.
    const vec_t & __fastcall Mod1 (limb_t v);



        // Divide itself by vector 'v'.
        // Return constant reference to itself.
    const vec_t & __fastcall Div (const vec_t &v);



        // Reminder of division itself by vector 'v'.
        // Return constant reference to itself.
    const vec_t & __fastcall Mod (const vec_t &v);



        // Add to itself 'v'*'w'.
        // Itself should have enough memory.
        // Return constant reference to itself.
    const vec_t & __fastcall AddMul1 (const vec_t &v, limb_t w);



        // Subtract from itself 'v'*'w'.
        // Itself >= 'v'*'w'.
        // Return constant reference to itself.
    const vec_t & __fastcall SubMul1 (const vec_t &v, limb_t w);



        // Shift itself left by 'shift' bits.
        // Itself should have enough memory and is normalized.
        // Return constant reference to itself.
    const vec_t & __fastcall ShiftLeft (bitcnt_t shift);



        // Shift itself right by 'shift' bits.
        // Itself is normalized.
        // Return constant reference to itself.
    const vec_t & __fastcall ShiftRight (bitcnt_t shift);



        // Prefix operators
    const vec_t & __fastcall operator ++ ();
    const vec_t & __fastcall operator -- ();



        // Helpers
    const vec_t & __fastcall operator+= (limb_t);
    const vec_t & __fastcall operator+= (const vec_t &);
    const vec_t & __fastcall operator-= (limb_t);
    const vec_t & __fastcall operator-= (const vec_t &);
    const vec_t & __fastcall operator*= (limb_t);
    const vec_t & __fastcall operator*= (const vec_t &);
    const vec_t & __fastcall operator/= (limb_t);
    const vec_t & __fastcall operator/= (const vec_t &);
    const vec_t & __fastcall operator%= (limb_t);
    const vec_t & __fastcall operator%= (const vec_t &);

    const vec_t & __fastcall operator <<= (bitcnt_t);
    const vec_t & __fastcall operator >>= (bitcnt_t);
};



inline const bool __fastcall operator < (const vec_t &u, const vec_t &v) { return u.Compare(v) < 0; }
inline const bool __fastcall operator > (const vec_t &u, const vec_t &v) {  return u.Compare(v) > 0; }
inline const bool __fastcall operator <= (const vec_t &u, const vec_t &v) {  return u.Compare(v) <= 0; }
inline const bool __fastcall operator >= (const vec_t &u, const vec_t &v) {  return u.Compare(v) >= 0; }
inline const bool __fastcall operator == (const vec_t &u, const vec_t &v) {  return u.Compare(v) == 0; }
inline const bool __fastcall operator != (const vec_t &u, const vec_t &v) {  return u.Compare(v) != 0; }



    // Auxiliary function for vectors increment.
    // 'z' should have enough memory space, 'u' is normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const vec_t & __fastcall Inc (vec_t &z, const vec_t &u, limb_t s);



    // Auxiliary function for vectors addition.
    // 'z' should have enough memory space, 'u' and 'v' are normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const vec_t & __fastcall Add (vec_t &z, const vec_t &u, const vec_t &v);



    // Auxiliary function for vectors decrement.
    // 'u'>='s', 'u' is normalized.
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const vec_t & __fastcall Dec (vec_t &z, const vec_t &u, limb_t s);



    // Auxiliary function for vectors subtraction.
    // 'u'>='v', 'z' should have enough memory space, 'u' and 'v' are normalized..
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const vec_t & __fastcall Sub (vec_t &z, const vec_t &u, const vec_t &v);



    // 'z' = 'u'*'v'.
    // 'z' should have enough memory, 'u' is normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Mul1 (vec_t &z, const vec_t &u, limb_t v);



    // 'z' = 'u'*'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Mul (vec_t &z, const vec_t &u, const vec_t &v);



    // 'z' = 'u'/'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Div1 (vec_t &z, const vec_t &u, limb_t v);



    // 'z' = 'u' mod 'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Mod1 (vec_t &z, const vec_t &u, limb_t v);



    // 'z' = 'u'/'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Div (vec_t &z, const vec_t &u, const vec_t &v);



    // 'z' = 'u'%'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall Mod (vec_t &z, const vec_t &u, const vec_t &v);



    // 'z' = 'u' + 'v'*'w'.
    // 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall AddMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w);



    // 'z' = 'u' - 'v'*'w'.
    // 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const vec_t & __fastcall SubMul1 (vec_t &z, const vec_t &u, const vec_t &v, limb_t w);



    // 'z' = 'u' << shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const vec_t & __fastcall ShiftLeft (vec_t &z, const vec_t &u, bitcnt_t shift);



    // 'z' = 'u' >> shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const vec_t & __fastcall ShiftRight (vec_t &z, const vec_t &u, bitcnt_t shift);



    // Add u + v.
    // Necessary memory allocated here.
    // Before call: 'u' and 'v' are normalized.
    // Return the resulting vector.
const vec_t __fastcall operator + (const vec_t &u, const vec_t &v);



    // The same function, but 'v' is a limb.
const vec_t __fastcall operator + (const vec_t &u, limb_t v);



    // Subtract u - v.
    // Necessary memory allocated here.
    // 'u'>='v' and 'u' and 'v' are normalized.
    // Returns the resulting vector.
const vec_t __fastcall operator - (const vec_t &u, const vec_t &v);



    // The same function, but 'v' is a limb.
const vec_t __fastcall operator - (const vec_t &u, limb_t v);



    // Multiply u * v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized.
    // Returns the resulting vector.
const vec_t __fastcall operator * (const vec_t &u, const vec_t &v);



    // The same function, but 'v' is a limb.
const vec_t __fastcall operator * (const vec_t &u, limb_t v);



    // Divide u / v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized vectors.
    // Returns the resulting vector.
const vec_t __fastcall operator / (const vec_t &u, const vec_t &v);



    // The same function, but 'v' is a limb.
const vec_t __fastcall operator / (const vec_t &u, limb_t v);



    // Reminder of division (u mod v).
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized vectors.
    // Returns the resulting vector.
const vec_t __fastcall operator % (const vec_t &u, const vec_t &v);



    // The same function, but 'v' is a limb.
const vec_t __fastcall operator % (const vec_t &u, limb_t v);



    // Return 'u' << shift.
    // Necessary memory allocated here.
const vec_t __fastcall operator << (const vec_t &u, bitcnt_t shift);



    // Return 'u' >> shift.
    // Necessary memory allocated here.
const vec_t __fastcall operator >> (const vec_t &u, bitcnt_t shift);



#endif
