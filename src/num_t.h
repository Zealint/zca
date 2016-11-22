// Class 'num_t' - to better work with signed vectors of limbs

#ifndef NUM_T_H
#define NUM_T_H

#pragma once

#include "misc.h"
#include "typedef.h"
#include "vec_t.h"



enum DivisionMode {
  DM_EUCLIDEAN,	// The reminder is always non-negative
  DM_TRUNCATED,	// The reminder has the same sign as dividend
  DM_FLOORED,	// The reminder has the same sign as divider
};




class num_t {
  public:
    offset_t size;  // Signed size of (not necessary normalized) vector. |size| is NEVER exceed the number of allocated limbs.
    limb_t *limbs;  // Array of limbs.



    		// Constructors and destructor
    num_t(): size(0), limbs (nullptr) {} // Empty vector is initially zero.
    explicit num_t (size_t s): size(0), limbs (new limb_t[s]) {} // Not empty vector has initially zero value.
    num_t (const num_t &that): limbs (new limb_t[abs(that.size)]) { *this = that; }
    num_t (num_t &&that): size (that.size), limbs (that.limbs) { that.limbs = nullptr; }
    num_t (const char *, u8=10);	// Constructor by string with default base (10)
    ~num_t() { delete[] limbs; }



        // Resize 'limbs'.
        // It doesn't copy old limbs to new. ALL data will be cleared.
        // Vector becomes zero.
    void __fastcall resize (size_t s);



        // Adapt 'limbs' to new size s.
        // Nothing is changed if |size| >= s.
    void __fastcall adapt (size_t s);



    		// Sign of the vector.
    sign_t __fastcall sign () const { return ::sign (size); };



        // Unsigned size of the vector.
    size_t __fastcall unsigned_size () const { return ::abs (size); };



				// Normalize vector size.
				// Return normalized size.
    offset_t __fastcall normalize();



    		// Compare with other vector with the same size.
    		// Both vectors are normalized, 'size'='that.size' and 'size'>0.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall compare_equal_size (const num_t &that) const;



    		// Compare with other vector.
    		// Both vectors are normalized.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall compare (const num_t &that) const;



        // Compare with limb and slimb.
    		// Vector is normalized.
    		// Return the result of comparison as sign_t.
    sign_t __fastcall compare (limb_t s) const;
    sign_t __fastcall compare (slimb_t s) const;




    bool __fastcall operator == (const char *str) const { num_t z(str);  return compare(z) == 0; }



    		// Get the copy of other vector without memory allocation.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    num_t & __fastcall operator = (const num_t &that);
    num_t & __fastcall operator = (const vec_t &that);



        // Get vector from decimal string.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    const num_t & __fastcall operator = (const char *str) { return from_string (str); }



        // Get vector from i64/u64 value.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    const num_t & __fastcall operator = (i64);
    const num_t & __fastcall operator = (u64);



    		// Move the other vector to itself.
        // Return the reference to itself.
        // The other vector becomes empty (that.limbs=nullptr).
    num_t & __fastcall operator = (num_t &&v);



    		// Helpers [] for mutable and constant reference.
    limb_t & __fastcall operator [] (size_t index) { return limbs[index]; }
    const limb_t & __fastcall operator [] (size_t index) const { return limbs[index]; }



        // Check if vector is zero
    bool __fastcall is_zero() const { return size == 0; }

        // Set vector to zero
    void __fastcall set_zero() { size = 0; }

        // Check if vector is normalized.
    bool __fastcall is_normalized() const { return size==0 || limbs[abs(size)-1]!=0; }



    const num_t &__fastcall neg() { size = -size;  return *this; }

    const num_t __fastcall operator - () const { num_t z(*this);  z.size = -z.size;  return z; }



        // Add to itself slimb 's'.
        // Enough memory for itself should be allocated.
        // Return constant reference to itself.
    const num_t & __fastcall inc (slimb_t s);
    const num_t & __fastcall inc (limb_t s);



        // Add to itself vector 'v'.
        // Enough memory for itself should be allocated.
        // Return constant reference to itself.
    const num_t & __fastcall add (const num_t &v);



        // Subtract from itself slimb 's'.
        // Return constant reference to itself.
    const num_t & __fastcall dec (slimb_t s);
    const num_t & __fastcall dec (limb_t s);



        // Subtract from itself vector 'v'.
        // Return constant reference to itself.
    const num_t & __fastcall sub (const num_t &v);



        // Multiply itself by slimb 'v'.
        // Itself should have enough memory.
        // Return constant reference to itself.
    const num_t & __fastcall mul (slimb_t v);
    const num_t & __fastcall mul (limb_t v);



        // Multiply itself by vector 'v'.
        // Itself should have enough memory.
        // Return constant reference to itself.
    const num_t & __fastcall mul (const num_t &v);



        // Divide itself by slimb 'v'.
        // Return constant reference to itself.
    const num_t & __fastcall div (limb_t v);
    const num_t & __fastcall div (slimb_t v);



        // Reminder of division itself by slimb 'v'.
        // Return constant reference to itself.
    const num_t & __fastcall mod (limb_t v, DivisionMode=DM_EUCLIDEAN);
    const num_t & __fastcall mod (slimb_t v, DivisionMode=DM_EUCLIDEAN);



        // Divide itself by vector 'v'.
        // Return constant reference to itself.
    const num_t & __fastcall div (const num_t &v);



        // Reminder of division itself by vector 'v'.
        // Return constant reference to itself.
    const num_t & __fastcall mod (const num_t &v, DivisionMode=DM_EUCLIDEAN);



        // Shift itself left by 'shift' bits.
        // Itself should have enough memory and is normalized.
        // Return constant reference to itself.
    const num_t & __fastcall shift_left (bitcnt_t shift);



        // Shift itself right by 'shift' bits.
        // Itself is normalized.
        // Return constant reference to itself.
    const num_t & __fastcall shift_right (bitcnt_t shift);



        // Prefix operators
    const num_t & __fastcall operator ++ ();
    const num_t & __fastcall operator -- ();



        // Helpers
    const num_t & __fastcall operator+= (slimb_t);
    const num_t & __fastcall operator+= (limb_t);
    const num_t & __fastcall operator+= (const num_t &);
    const num_t & __fastcall operator-= (slimb_t);
    const num_t & __fastcall operator-= (limb_t);
    const num_t & __fastcall operator-= (const num_t &);
    const num_t & __fastcall operator*= (slimb_t);
    const num_t & __fastcall operator*= (limb_t);
    const num_t & __fastcall operator*= (const num_t &);
    const num_t & __fastcall operator/= (slimb_t);
    const num_t & __fastcall operator/= (limb_t);
    const num_t & __fastcall operator/= (const num_t &);
    const num_t & __fastcall operator%= (slimb_t);
    const num_t & __fastcall operator%= (limb_t);
    const num_t & __fastcall operator%= (const num_t &);

    const num_t & __fastcall operator <<= (bitcnt_t);
    const num_t & __fastcall operator >>= (bitcnt_t);

    const num_t & __fastcall from_string (const char *, u8=10U);
    char * __fastcall to_string (char *, u8=10U) const;
};

inline const bool __fastcall operator < (const num_t &u, limb_t v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const num_t &u, limb_t v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const num_t &u, limb_t v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const num_t &u, limb_t v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const num_t &u, limb_t v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const num_t &u, limb_t v) {  return u.compare(v) != 0; }

inline const bool __fastcall operator < (limb_t u, const num_t &v) { return v.compare(u) > 0; }
inline const bool __fastcall operator > (limb_t u, const num_t &v) {  return v.compare(u) < 0; }
inline const bool __fastcall operator <= (limb_t u, const num_t &v) {  return v.compare(u) >= 0; }
inline const bool __fastcall operator >= (limb_t u, const num_t &v) {  return v.compare(u) <= 0; }
inline const bool __fastcall operator == (limb_t u, const num_t &v) {  return v.compare(u) == 0; }
inline const bool __fastcall operator != (limb_t u, const num_t &v) {  return v.compare(u) != 0; }

inline const bool __fastcall operator < (const num_t &u, slimb_t v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const num_t &u, slimb_t v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const num_t &u, slimb_t v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const num_t &u, slimb_t v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const num_t &u, slimb_t v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const num_t &u, slimb_t v) {  return u.compare(v) != 0; }

inline const bool __fastcall operator < (slimb_t u, const num_t &v) { return v.compare(u) > 0; }
inline const bool __fastcall operator > (slimb_t u, const num_t &v) {  return v.compare(u) < 0; }
inline const bool __fastcall operator <= (slimb_t u, const num_t &v) {  return v.compare(u) >= 0; }
inline const bool __fastcall operator >= (slimb_t u, const num_t &v) {  return v.compare(u) <= 0; }
inline const bool __fastcall operator == (slimb_t u, const num_t &v) {  return v.compare(u) == 0; }
inline const bool __fastcall operator != (slimb_t u, const num_t &v) {  return v.compare(u) != 0; }



inline const bool __fastcall operator < (const num_t &u, const num_t &v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const num_t &u, const num_t &v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const num_t &u, const num_t &v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const num_t &u, const num_t &v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const num_t &u, const num_t &v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const num_t &u, const num_t &v) {  return u.compare(v) != 0; }



    // Auxiliary function for unsigned vectors difference.
    // 'z' should have enough memory space, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall diff (num_t &z, const vec_t &u, limb_t a);
const num_t & __fastcall diff (num_t &z, const vec_t &u, const vec_t &v);



    // Auxiliary function for vectors increment.
    // 'z' should have enough memory space, 'u' is normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const num_t & __fastcall inc (num_t &z, const num_t &u, slimb_t s);
const num_t & __fastcall inc (num_t &z, const num_t &u, limb_t s);



    // Auxiliary function for vectors addition.
    // 'z' should have enough memory space, 'u' and 'v' are normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const num_t & __fastcall add (num_t &z, const num_t &u, const num_t &v);



    // Auxiliary function for vectors decrement.
    // 'u'>='s', 'u' is normalized.
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const num_t & __fastcall dec (num_t &z, const num_t &u, slimb_t s);
const num_t & __fastcall dec (num_t &z, const num_t &u, limb_t s);



    // Auxiliary function for vectors subtraction.
    // 'u'>='v', 'z' should have enough memory space, 'u' and 'v' are normalized..
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const num_t & __fastcall sub (num_t &z, const num_t &u, const num_t &v);



    // 'z' = 'u'*'v'.
    // 'z' should have enough memory, 'u' is normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall mul (num_t &z, const num_t &u, slimb_t v);



    // 'z' = 'u'*'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall mul (num_t &z, const num_t &u, const num_t &v);



    // 'z' = 'u'/'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall div (num_t &z, const num_t &u, slimb_t v);



    // 'z' = 'u' mod 'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall mod (num_t &z, const num_t &u, slimb_t v, DivisionMode=DM_EUCLIDEAN);
const num_t & __fastcall mod (num_t &z, const num_t &u, limb_t v, DivisionMode=DM_EUCLIDEAN);



    // 'z' = 'u'/'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall div (num_t &z, const num_t &u, const num_t &v);



    // 'z' = 'u'%'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall mod (num_t &z, const num_t &u, const num_t &v, DivisionMode=DM_EUCLIDEAN);



    // 'z' = 'u' + 'v'*'w'.
    // 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall addmul (num_t &z, const num_t &u, const num_t &v, slimb_t w);



    // 'z' = 'u' - 'v'*'w'.
    // 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const num_t & __fastcall submul (num_t &z, const num_t &u, const num_t &v, slimb_t w);



    // 'z' = 'u' << shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const num_t & __fastcall shift_left (num_t &z, const num_t &u, bitcnt_t shift);



    // 'z' = 'u' >> shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const num_t & __fastcall shift_right (num_t &z, const num_t &u, bitcnt_t shift);



    // Add u + v.
    // Necessary memory allocated here.
    // Before call: 'u' and 'v' are normalized.
    // Return the resulting vector.
const num_t __fastcall operator + (const num_t &u, const num_t &v);



    // The same function, but 'v' is a slimb.
const num_t __fastcall operator + (const num_t &u, slimb_t v);



    // Subtract u - v.
    // Necessary memory allocated here.
    // 'u'>='v' and 'u' and 'v' are normalized.
    // Returns the resulting vector.
const num_t __fastcall operator - (const num_t &u, const num_t &v);



    // The same function, but 'v' is a slimb.
const num_t __fastcall operator - (const num_t &u, slimb_t v);



    // Multiply u * v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized.
    // Returns the resulting vector.
const num_t __fastcall operator * (const num_t &u, const num_t &v);



    // The same function, but 'v' is a slimb.
const num_t __fastcall operator * (const num_t &u, slimb_t v);



    // Divide u / v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized vectors.
    // Returns the resulting vector.
const num_t __fastcall operator / (const num_t &u, const num_t &v);



    // The same function, but 'v' is a slimb.
const num_t __fastcall operator / (const num_t &u, slimb_t v);



    // Reminder of division (u mod v).
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized vectors.
    // Returns the resulting vector.
const num_t __fastcall operator % (const num_t &u, const num_t &v);



    // The same function, but 'v' is a slimb.
const num_t __fastcall operator % (const num_t &u, slimb_t v);



    // Return 'u' << shift.
    // Necessary memory allocated here.
const num_t __fastcall operator << (const num_t &u, bitcnt_t shift);



    // Return 'u' >> shift.
    // Necessary memory allocated here.
const num_t __fastcall operator >> (const num_t &u, bitcnt_t shift);



#endif
