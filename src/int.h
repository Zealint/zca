// Class 'Int' - an arbitrary precision integers

#ifndef INT_H
#define INT_H
#pragma once

#include "typedef.h"
#include "num_t.h"

/*

class Int;

template <typename T> inline size_t size_after_assign () {
  if (sizeof (T) <= sizeof (limb_t))  return 1;
  return sizeof (T) / sizeof (limb_t);
}



inline size_t size_after_add (const Int &a, limb_t b);
inline size_t size_after_add (const Int &a, slimb_t b);
inline size_t size_after_add (const Int &a, const Int &b);
inline size_t size_after_sub (const Int &a, limb_t b);
inline size_t size_after_sub (const Int &a, slimb_t b);
inline size_t size_after_sub (const Int &a, const Int &b);
inline size_t size_after_mul (const Int &a, limb_t b);
inline size_t size_after_mul (const Int &a, slimb_t b);
inline size_t size_after_mul (const Int &a, const Int &b);
inline size_t size_after_div (const Int &a, limb_t b);
inline size_t size_after_div (const Int &a, slimb_t b);
inline size_t size_after_div (const Int &a, const Int &b);
inline size_t size_after_mod (const Int &a, limb_t b);
inline size_t size_after_mod (const Int &a, slimb_t b);
inline size_t size_after_mod (const Int &a, const Int &b);
inline size_t size_after_shift_left (const Int &a, bitcnt_t s);
inline size_t size_after_shift_right (const Int &a, bitcnt_t s);



class Int {
    num_t data;	// A long number, always normalized.
    size_t allocated;  // How many limbs are allocated. |data.size| is NEVER exceed the number of allocated limbs.

  public:

    		// Constructors and destructor
    Int(): data(), allocated(0) {} // Empty vector is initially zero.
    explicit Int (size_t s) : data(s), allocated(s) {};
    Int (const Int &that): data (that.data), allocated (that.size()) {}
    Int (Int &&that): data (that.data), allocated (that.allocated) {}
    Int (const char *str, u8 base=10): data (str, base) {	// Constructor by string with default base (10)
      allocated = data.unsigned_size();
    }

    explicit Int (i8 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (u8 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (i16 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (u16 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (i32 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (u32 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (i64 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }
    explicit Int (u64 a, ConstructorInterpreter ci) { if (ci == CI_NUMBER) *this = a;  else adapt (a); }


    ~Int() {}



        // Resize data.limbs.
        // It doesn't copy old limbs to new. ALL data will be cleared.
        // Number becomes zero.
    void __fastcall resize (size_t s) { data.resize(s);  allocated = s; }

        // Adapt 'data.limbs' to new size s.
        // Nothing is changed if 'allocated' >= s.
    void __fastcall adapt (size_t s) { if (allocated < s) { data.adapt(s);  allocated = s; } }

    void __fastcall clear() { data.clear();  allocated = 0; }

    void __fastcall shrink() {
    	size_t s = size();
    	if (allocated == s)  return;
    	num_t tmp (data);
    	clear();
    	if (s > 0) {
      	data.move (tmp);
      	allocated = size();
      }
    }



    size_t __fastcall allocated_limbs() const { return allocated; }



    		// Sign of the number.
    sign_t __fastcall sign() const { return data.sign(); }

        // Unsigned size of the number (how many limbs the number covers).
    size_t __fastcall size() const { return data.unsigned_size(); }

        // Comparison
    sign_t __fastcall compare (const Int &that) const { return data.compare (that.data); };
    sign_t __fastcall compare (limb_t a) const { return data.compare(a); }
    sign_t __fastcall compare (slimb_t a) const { return data.compare(a); }
    bool __fastcall operator == (const char *str) const { return data.compare (str) == 0; }


        // Get the number by different ways.
    Int & __fastcall operator = (const Int &that) {
      if (this != &that) {
        adapt (that.size());
        data = that.data;
      }
      return *this;
    }

    const Int & __fastcall operator = (const char *str) { adapt (how_many_digits (str));  data = str;  return *this; }

    const Int & __fastcall operator = (i8 a) { adapt (size_after_assign<i8>());  data = a;  return *this; }
    const Int & __fastcall operator = (u8 a) { adapt (size_after_assign<u8>());  data = a;  return *this; }
    const Int & __fastcall operator = (i16 a) { adapt (size_after_assign<i16>());  data = a;  return *this; }
    const Int & __fastcall operator = (u16 a) { adapt (size_after_assign<u16>());  data = a;  return *this; }
    const Int & __fastcall operator = (i32 a) { adapt (size_after_assign<i32>());  data = a;  return *this; }
    const Int & __fastcall operator = (u32 a) { adapt (size_after_assign<u32>());  data = a;  return *this; }
    const Int & __fastcall operator = (i64 a) { adapt (size_after_assign<i64>());  data = a;  return *this; }
    const Int & __fastcall operator = (u64 a) { adapt (size_after_assign<u64>());  data = a;  return *this; }

    //const Int & __fastcall operator = (i64 a) { adapt (LIMBS_IN_U64);  data = a;  return *this; }
    //const Int & __fastcall operator = (u64 a) { adapt (LIMBS_IN_U64);  data = a;  return *this; }

    const Int & __fastcall move (Int &that) {
      clear();
      return this->swap(that);
    }



    const Int & __fastcall swap (Int &that) {
      data.swap (that.data);
      ::swap (allocated, that.allocated);
    }



    		// Move the other vector to itself.
        // Return the reference to itself.
        // The other vector becomes empty (that.limbs=nullptr).
    Int & __fastcall operator = (Int &&a) { data = (num_t &&)a.data;  allocated = a.allocated;  return *this; }



        // Working with string
    const Int & __fastcall from_string (const char *, u8=10U);
    char * __fastcall to_string (char *, u8=10U) const;



    		// Helpers [] for mutable and constant reference.
    limb_t & __fastcall operator [] (size_t index) { assert (index < allocated); return data.limbs[index]; }
    const limb_t & __fastcall operator [] (size_t index) const { assert (index < allocated); return data.limbs[index]; }



        // Compare itself with zero.
    bool __fastcall is_zero() const { return data.size == 0; }
    bool __fastcall is_positive() const { return data.size > 0; }
    bool __fastcall is_negative() const { return data.size < 0; }

        // Set itself to zero.
    void __fastcall set_zero() { data.set_zero(); }



        // Negate itself.
    const Int &__fastcall neg() { data.neg();  return *this; }

    		// Unary minus.
    const Int __fastcall operator - () const { Int z (*this);  return z.neg(); }



        // Arithmetic functions
    const Int & __fastcall add (limb_t a) { adapt (size_after_add (*this, a));  data.inc(a);  return *this; }
    const Int & __fastcall add (slimb_t a) { adapt (size_after_add (*this, a));  data.inc(a);  return *this; }
    const Int & __fastcall add (const Int &a) { adapt (size_after_add (*this, a));  data.add(a.data);  return *this; }

    const Int & __fastcall sub (limb_t a) { adapt (size_after_sub (*this, a));  data.dec(a);  return *this; }
    const Int & __fastcall sub (slimb_t a) { adapt (size_after_sub (*this, a));  data.dec(a);  return *this; }
    const Int & __fastcall sub (const Int &a) { adapt (size_after_sub (*this, a));  data.sub(a.data);  return *this; }

    const Int & __fastcall mul (limb_t a) { adapt (size_after_mul (*this, a));  data.mul(a);  return *this; }
    const Int & __fastcall mul (slimb_t a) { adapt (size_after_mul (*this, a));  data.mul(a);  return *this; }
    const Int & __fastcall mul (const Int &a) { adapt (size_after_mul (*this, a));  data.mul(a.data);  return *this; }

    const Int & __fastcall div (limb_t a) { adapt (size_after_div (*this, a));  data.div(a);  return *this; }
    const Int & __fastcall div (slimb_t a) { adapt (size_after_div (*this, a));  data.div(a);  return *this; }
    const Int & __fastcall div (const Int &a) { adapt (size_after_div (*this, a));  data.div(a.data);  return *this; }

    const Int & __fastcall mod (limb_t a, DivisionMode=DM_EUCLIDEAN) { adapt (size_after_mod (*this, a));  data.mod(a);  return *this; }
    const Int & __fastcall mod (slimb_t a, DivisionMode=DM_EUCLIDEAN) { adapt (size_after_mod (*this, a));  data.mod(a);  return *this; }
    const Int & __fastcall mod (const Int &a, DivisionMode=DM_EUCLIDEAN) { adapt (size_after_mod (*this, a));  data.mod(a.data);  return *this; }



        // Bit operations
    const Int & __fastcall shift_left (bitcnt_t shift) { adapt (size_after_shift_left (*this, shift));  data.shift_left (shift);  return *this; }
    const Int & __fastcall shift_right (bitcnt_t shift) { adapt (size_after_shift_right (*this, shift));  data.shift_right (shift);  return *this; }



        // Helpers

        // Prefix operators of increment and decrement
    const Int & __fastcall operator ++ () { return this->add((limb_t)1); }
    const Int & __fastcall operator -- () { return this->sub((limb_t)1); }

    const Int & __fastcall operator+= (limb_t a) { return this->add(a); }
    const Int & __fastcall operator+= (slimb_t a) { return this->add(a); }
    const Int & __fastcall operator+= (const Int & a) { return this->add(a); }

    const Int & __fastcall operator-= (limb_t a) { return this->sub(a); }
    const Int & __fastcall operator-= (slimb_t a) { return this->sub(a); }
    const Int & __fastcall operator-= (const Int & a) { return this->sub(a); }

    const Int & __fastcall operator*= (limb_t a) { return this->mul(a); }
    const Int & __fastcall operator*= (slimb_t a) { return this->mul(a); }
    const Int & __fastcall operator*= (const Int & a) { return this->mul(a); }

    const Int & __fastcall operator/= (limb_t a) { return this->div(a); }
    const Int & __fastcall operator/= (slimb_t a) { return this->div(a); }
    const Int & __fastcall operator/= (const Int &a) { return this->div(a); }

    const Int & __fastcall operator%= (limb_t a) { return this->mod(a); }
    const Int & __fastcall operator%= (slimb_t a) { return this->mod(a); }
    const Int & __fastcall operator%= (const Int &a) { return this->mod(a); }

    const Int & __fastcall operator <<= (bitcnt_t shift) { return this->shift_left (shift); }
    const Int & __fastcall operator >>= (bitcnt_t shift) { return this->shift_right (shift); }

    friend const Int & __fastcall add (Int &, const Int &, limb_t);
    friend const Int & __fastcall add (Int &, const Int &, slimb_t);
    friend const Int & __fastcall add (Int &, const Int &, const Int &);

    friend const Int & __fastcall sub (Int &, const Int &, limb_t);
    friend const Int & __fastcall sub (Int &, const Int &, slimb_t);
    friend const Int & __fastcall sub (Int &, const Int &, const Int &);

    friend const Int & __fastcall mul (Int &, const Int &, limb_t);
    friend const Int & __fastcall mul (Int &, const Int &, slimb_t);
    friend const Int & __fastcall mul (Int &, const Int &, const Int &);

    friend const Int & __fastcall div (Int &, const Int &, limb_t);
    friend const Int & __fastcall div (Int &, const Int &, slimb_t);
    friend const Int & __fastcall div (Int &, const Int &, const Int &);

    friend const Int & __fastcall mod (Int &, const Int &, limb_t, DivisionMode);
    friend const Int & __fastcall mod (Int &, const Int &, slimb_t, DivisionMode);
    friend const Int & __fastcall mod (Int &, const Int &, const Int &, DivisionMode);

    friend const Int & __fastcall shift_left (Int &, const Int &, bitcnt_t);
    friend const Int & __fastcall shift_right (Int &, const Int &, bitcnt_t);
};



    // Suffix '_dec' for decimal numbers (begins with digit from 1 to 9).
inline const Int __fastcall operator"" _dec (const char *str) { return Int (str); }
    // Suffix '_hex' for hexadecimal numbers (begins with 0x).
inline const Int __fastcall operator"" _hex (const char *str) { assert (str[0]=='0' && (str[1]=='x'||str[1]=='X'));  return Int (str+2, 16); }
    // Suffix '_bin' for binary numbers (begins with 0).
inline const Int __fastcall operator"" _bin (const char *str) { assert (str[0]=='0');  return Int (str+1, 2); }



// !!! Optimize them (more accurately)

inline size_t size_after_add (const Int &a, limb_t b) { return a.size() + 1; }
inline size_t size_after_add (const Int &a, slimb_t b) { return a.size() + 1; }
inline size_t size_after_add (const Int &a, const Int &b) { return max (a.size(), b.size()) + 1; }
inline size_t size_after_sub (const Int &a, limb_t b) { return a.size() + 1; }
inline size_t size_after_sub (const Int &a, slimb_t b) { return a.size() + 1; }
inline size_t size_after_sub (const Int &a, const Int &b) { return max (a.size(), b.size()) + 1; }
inline size_t size_after_mul (const Int &a, limb_t b) { return a.size() + 1; }
inline size_t size_after_mul (const Int &a, slimb_t b) { return a.size() + 1; }
inline size_t size_after_mul (const Int &a, const Int &b) { return a.size() + b.size(); }
inline size_t size_after_div (const Int &a, limb_t b) { return a.size(); }
inline size_t size_after_div (const Int &a, slimb_t b) { return a.size(); }
inline size_t size_after_div (const Int &a, const Int &b) { return a.size(); }
inline size_t size_after_mod (const Int &a, limb_t b) { return 1; }
inline size_t size_after_mod (const Int &a, slimb_t b) { return 2; }
inline size_t size_after_mod (const Int &a, const Int &b) { return b.size() + 1; }
inline size_t size_after_shift_left (const Int &a, bitcnt_t s) { return a.size() + (s+LIMB_BITS-1)/LIMB_BITS; }
inline size_t size_after_shift_right (const Int &a, bitcnt_t s) { return a.size(); }



inline const bool __fastcall operator < (const Int &u, limb_t v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const Int &u, limb_t v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const Int &u, limb_t v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const Int &u, limb_t v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const Int &u, limb_t v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const Int &u, limb_t v) {  return u.compare(v) != 0; }

inline const bool __fastcall operator < (limb_t u, const Int &v) { return v.compare(u) > 0; }
inline const bool __fastcall operator > (limb_t u, const Int &v) {  return v.compare(u) < 0; }
inline const bool __fastcall operator <= (limb_t u, const Int &v) {  return v.compare(u) >= 0; }
inline const bool __fastcall operator >= (limb_t u, const Int &v) {  return v.compare(u) <= 0; }
inline const bool __fastcall operator == (limb_t u, const Int &v) {  return v.compare(u) == 0; }
inline const bool __fastcall operator != (limb_t u, const Int &v) {  return v.compare(u) != 0; }

inline const bool __fastcall operator < (const Int &u, slimb_t v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const Int &u, slimb_t v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const Int &u, slimb_t v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const Int &u, slimb_t v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const Int &u, slimb_t v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const Int &u, slimb_t v) {  return u.compare(v) != 0; }

inline const bool __fastcall operator < (slimb_t u, const Int &v) { return v.compare(u) > 0; }
inline const bool __fastcall operator > (slimb_t u, const Int &v) {  return v.compare(u) < 0; }
inline const bool __fastcall operator <= (slimb_t u, const Int &v) {  return v.compare(u) >= 0; }
inline const bool __fastcall operator >= (slimb_t u, const Int &v) {  return v.compare(u) <= 0; }
inline const bool __fastcall operator == (slimb_t u, const Int &v) {  return v.compare(u) == 0; }
inline const bool __fastcall operator != (slimb_t u, const Int &v) {  return v.compare(u) != 0; }



inline const bool __fastcall operator < (const Int &u, const Int &v) { return u.compare(v) < 0; }
inline const bool __fastcall operator > (const Int &u, const Int &v) {  return u.compare(v) > 0; }
inline const bool __fastcall operator <= (const Int &u, const Int &v) {  return u.compare(v) <= 0; }
inline const bool __fastcall operator >= (const Int &u, const Int &v) {  return u.compare(v) >= 0; }
inline const bool __fastcall operator == (const Int &u, const Int &v) {  return u.compare(v) == 0; }
inline const bool __fastcall operator != (const Int &u, const Int &v) {  return u.compare(v) != 0; }



    // Auxiliary function for vectors increment.
    // 'z' should have enough memory space, 'u' is normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const Int & __fastcall add (Int &z, const Int &u, slimb_t s) { z.adapt (size_after_add (u, s));  ::inc (z.data, u.data, s);  return z; }
const Int & __fastcall add (Int &z, const Int &u, limb_t s) { z.adapt (size_after_add (u, s));  ::inc (z.data, u.data, s);  return z; }



    // Auxiliary function for vectors addition.
    // 'z' should have enough memory space, 'u' and 'v' are normalized.
    // Based on functions from "add.cc".
    // Return constant reference to 'z'.
const Int & __fastcall add (Int &z, const Int &u, const Int &v) { z.adapt (size_after_add (u, v));  ::add (z.data, u.data, v.data);  return z; }



    // Auxiliary function for vectors decrement.
    // 'u'>='s', 'u' is normalized.
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const Int & __fastcall sub (Int &z, const Int &u, slimb_t s) { z.adapt (size_after_sub (u, s));  ::dec (z.data, u.data, s);  return z; }
const Int & __fastcall sub (Int &z, const Int &u, limb_t s) { z.adapt (size_after_sub (u, s));  ::dec (z.data, u.data, s);  return z; }



    // Auxiliary function for vectors subtraction.
    // 'u'>='v', 'z' should have enough memory space, 'u' and 'v' are normalized..
    // Based on functions from "sub.cc".
    // Return constant reference to 'z'.
const Int & __fastcall sub (Int &z, const Int &u, const Int &v) { z.adapt (size_after_sub (u, v));  ::sub (z.data, u.data, v.data);  return z; }



    // 'z' = 'u'*'v'.
    // 'z' should have enough memory, 'u' is normalized.
    // Return constant reference to 'z'.
const Int & __fastcall mul (Int &z, const Int &u, limb_t v) { z.adapt (size_after_mul (u, v));  ::mul (z.data, u.data, v);  return z; }
const Int & __fastcall mul (Int &z, const Int &u, slimb_t v) { z.adapt (size_after_mul (u, v));  ::mul (z.data, u.data, v);  return z; }



    // 'z' = 'u'*'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const Int & __fastcall mul (Int &z, const Int &u, const Int &v) { z.adapt (size_after_mul (u, v));  ::mul (z.data, u.data, v.data);  return z; }



    // 'z' = 'u'/'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const Int & __fastcall div (Int &z, const Int &u, limb_t v) { z.adapt (size_after_div (u, v));  ::div (z.data, u.data, v);  return z; }
const Int & __fastcall div (Int &z, const Int &u, slimb_t v) { z.adapt (size_after_div (u, v));  ::div (z.data, u.data, v);  return z; }



    // 'z' = 'u' mod 'v'.
    // 'u' is normalized.
    // Return constant reference to 'z'.
const Int & __fastcall mod (Int &z, const Int &u, slimb_t v, DivisionMode dm=DM_EUCLIDEAN) { z.adapt (size_after_mod (u, v));  ::mod (z.data, u.data, v, dm);  return z; }
const Int & __fastcall mod (Int &z, const Int &u, limb_t v, DivisionMode dm=DM_EUCLIDEAN) { z.adapt (size_after_mod (u, v));  ::mod (z.data, u.data, v, dm);  return z; }



    // 'z' = 'u'/'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const Int & __fastcall div (Int &z, const Int &u, const Int &v) { z.adapt (size_after_div (u, v));  ::div (z.data, u.data, v.data);  return z; }



    // 'z' = 'u'%'v'.
    // '&z' != '&u', '&z' != '&v', 'z' should have enough memory, 'u' and 'v' are normalized.
    // Return constant reference to 'z'.
const Int & __fastcall mod (Int &z, const Int &u, const Int &v, DivisionMode dm=DM_EUCLIDEAN) { z.adapt (size_after_mod (u, v));  ::mod (z.data, u.data, v.data, dm);  return z; }



    // 'z' = 'u' << shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const Int & __fastcall shift_left (Int &z, const Int &u, bitcnt_t shift) { z.adapt (size_after_shift_left (u, shift));  ::shift_left (z.data, u.data, shift);  return z; }



    // 'z' = 'u' >> shift.
    // 'u' is normalized and 'z' should have enough memory.
    // Return constant reference to 'z'.
const Int & __fastcall shift_right (Int &z, const Int &u, bitcnt_t shift) { z.adapt (size_after_shift_right (u, shift));  ::shift_right (z.data, u.data, shift);  return z; }



    // Add u + v.
    // Necessary memory allocated here.
    // Before call: 'u' and 'v' are normalized.
    // Return the resulting vector.
const Int __fastcall operator + (const Int &u, const Int &v) { Int z (size_after_add (u, v));  return add (z, u, v); }



    // The same function, but 'v' is a slimb.
const Int __fastcall operator + (const Int &u, limb_t v) { Int z (size_after_add (u, v));  return add (z, u, v); }
const Int __fastcall operator + (const Int &u, slimb_t v) { Int z (size_after_add (u, v));  return add (z, u, v); }




    // Subtract u - v.
    // Necessary memory allocated here.
    // 'u'>='v' and 'u' and 'v' are normalized.
    // Returns the resulting vector.
const Int __fastcall operator - (const Int &u, const Int &v) { Int z (size_after_sub (u, v));  return sub (z, u, v); }



    // The same function, but 'v' is a slimb.
const Int __fastcall operator - (const Int &u, limb_t v) { Int z (size_after_add (u, v));  return sub (z, u, v); }
const Int __fastcall operator - (const Int &u, slimb_t v) { Int z (size_after_add (u, v));  return sub (z, u, v); }



    // Multiply u * v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized.
    // Returns the resulting vector.
const Int __fastcall operator * (const Int &u, const Int &v) { Int z (size_after_mul (u, v));  return mul (z, u, v); }



    // The same function, but 'v' is a slimb.
const Int __fastcall operator * (const Int &u, limb_t v) { Int z (size_after_mul (u, v));  return mul (z, u, v); }
const Int __fastcall operator * (const Int &u, slimb_t v) { Int z (size_after_mul (u, v));  return mul (z, u, v); }



    // Divide u / v.
    // Necessary memory allocated here.
    // 'u' and 'v' are normalized vectors.
    // Returns the resulting vector.
const Int __fastcall operator / (const Int &u, const Int &v) { Int z (size_after_div (u, v));  return div (z, u, v); }



    // The same function, but 'v' is a slimb.
const Int __fastcall operator / (const Int &u, limb_t v) { Int z (size_after_div (u, v));  return div (z, u, v); }
const Int __fastcall operator / (const Int &u, slimb_t v) { Int z (size_after_div (u, v));  return div (z, u, v); }


const Int __fastcall operator % (const Int &u, slimb_t v) { Int z (size_after_mod (u, v));  return mod (z, u, v, DM_EUCLIDEAN); }
const Int __fastcall operator % (const Int &u, limb_t v) { Int z (size_after_mod (u, v));  return mod (z, u, v, DM_EUCLIDEAN); }
const Int __fastcall operator % (const Int &u, const Int &v) { Int z (size_after_mod (u, v));  return mod (z, u, v, DM_EUCLIDEAN); }




    // Return 'u' << shift.
    // Necessary memory allocated here.
const Int __fastcall operator << (const Int &u, bitcnt_t shift) { Int z (size_after_shift_left (u, shift));  return shift_left (z, u, shift); }



    // Return 'u' >> shift.
    // Necessary memory allocated here.
const Int __fastcall operator >> (const Int &u, bitcnt_t shift) { Int z (size_after_shift_right (u, shift));  return shift_right (z, u, shift); }

*/

#endif
