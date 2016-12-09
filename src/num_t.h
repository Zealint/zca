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



num_t & __fastcall inc (num_t &c, const num_t &a, limb_t b=1);
num_t & __fastcall dec (num_t &c, const num_t &a, limb_t b=1);
num_t & __fastcall add (num_t &, const num_t &, const num_t &);
template <typename T> inline num_t & __fastcall add (num_t &c, const num_t &a, T b);
num_t & __fastcall sub (num_t &, const num_t &, const num_t &);
template <typename T> inline num_t & __fastcall sub (num_t &c, const num_t &a, T b);
num_t & __fastcall mul (num_t &, const num_t &, const num_t &);
template <typename T> inline num_t & __fastcall mul (num_t &c, const num_t &a, T b);
num_t & __fastcall div_qr (num_t *, num_t *, const num_t &, const num_t &);
template <typename T> inline T __fastcall div_qr (num_t *q, const num_t &a, T b);
template <typename T> inline num_t & __fastcall div (num_t &q, const num_t &a, T b);
template <typename T> inline T __fastcall mod (const num_t &a, T b);
num_t & __fastcall div (num_t &c, const num_t &a, const num_t &b);
num_t & __fastcall mod (num_t &c, const num_t &a, const num_t &b);
num_t & __fastcall addmul (num_t &c, const num_t &a, const num_t &b, limb_t s);
num_t & __fastcall submul (num_t &c, const num_t &a, const num_t &b, limb_t s);
num_t & __fastcall shift_left (num_t &c, const num_t &a, bitcnt_t shift);
num_t & __fastcall shift_right (num_t &c, const num_t &a, bitcnt_t shift);



class num_t {
  public:
    offset_t size;  // Signed size of (not necessary normalized) vector. |size| is NEVER exceed the number of allocated limbs.
    limb_t *limbs;  // Array of limbs.



    		// Constructors and destructor
    num_t(): size(0), limbs (nullptr) {} // Empty vector is initially zero.
    num_t (const num_t &that): limbs (new limb_t[abs(that.size)]) { *this = that; }
    num_t (num_t &&that): size (that.size), limbs (that.limbs) { that.limbs = nullptr; }
    num_t (const char *, u8=10);	// Constructor by string with default base (10)
    template <typename T>
    num_t (T a, ConstructorInterpreter ci=CI_NUMBER): size(0) {
      if (ci == CI_NUMBER) {
        limbs = new limb_t[size_in_limbs<T> (abs(a))];
        *this = a;
      } else limbs = new limb_t[(size_t)abs(a)];
    }
    ~num_t() { delete[] limbs; }



        // Resize 'limbs'.
        // It doesn't copy old limbs to new. ALL data will be cleared.
        // Vector becomes zero.
    void __fastcall resize (size_t s);



        // Adapt 'limbs' to new size s.
        // Nothing is changed if |size| >= s.
    void __fastcall adapt (size_t s);



    num_t & __fastcall clear();



    		// Sign of the vector.
    sign_t __fastcall sign() const { return ::sign (size); };



        // Unsigned size of the vector.
    size_t __fastcall abs_size() const { return ::abs (size); };



				// Normalize vector size.
				// Return normalized size.
    offset_t __fastcall normalize();



    		// Compare with other vector with the same size.
    		// Both vectors are normalized, 'size'='that.size' and |size|>0.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall compare_equal_size (const num_t &that) const;



    		// Compare with other vector.
    		// Both vectors are normalized.
    		// Return the result of comparison as 'sign_t'.
    sign_t __fastcall compare (const num_t &that) const;



        // Compare with a value of built-in data type.
    		// Vector is normalized, 'a' is unsigned or non-negative
    		// Return the result of comparison.
    template <typename T>
    sign_t __fastcall compare (T a) const {
      assert (is_normalized());
      return compare (num_t(a));
    }



    bool __fastcall operator == (const char *str) const { num_t a(str);  return compare(a) == 0; }



    		// Get the copy of other vector without memory allocation.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    num_t & __fastcall operator = (const num_t &that);
    num_t & __fastcall operator = (const vec_t &that);



        // Get vector from decimal string.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    num_t & __fastcall operator = (const char *str) { return from_string (str); }

    // Convert vector to built-in data type.
    template <typename T> __fastcall operator T() const {
      if (is_zero())  return 0;
      sign_t s = this->sign();
      T res = 0;
      if (sizeof(T) >= sizeof(limb_t))  for (size_t i=0; i<size && i<sizeof(T)/sizeof(limb_t); ++i)  res |= ((T)limbs[i]<<(LIMB_BITS*i));
      else res = (T)limbs[0];
      return s>0 ? res : -res;
    }



        // Get vector from signed or unsigned value.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    template <typename T>
    num_t & __fastcall operator = (T a) {
      size = 0;
      split (limbs, size, abs(a));
      if (a<0)  neg();
      return *this;
    }



    		// Move the other vector to itself.
        // Return the reference to itself.
        // The other vector becomes empty (that.limbs=nullptr).
    num_t & __fastcall operator = (num_t &&that) { if (this == &that)  return *this;  return this->move (that); }
    num_t & __fastcall move (num_t &that) { return clear().swap (that); }
    num_t & __fastcall swap (num_t &that) { ::swap (size, that.size);  ::swap (limbs, that.limbs);  return *this; }



    		// Helpers [] for mutable and constant reference.
    limb_t & __fastcall operator [] (size_t index) { return limbs[index]; }
    const limb_t & __fastcall operator [] (size_t index) const { return limbs[index]; }



        // Check if vector is zero
    bool __fastcall is_zero() const { return size == 0; }

        // Set vector to zero
    num_t & __fastcall set_zero() { size = 0;  return *this; }

        // Check if vector is normalized.
    bool __fastcall is_normalized() const { return size==0 || limbs[abs(size)-1]!=0; }



    num_t & __fastcall neg() { size = -size;  return *this; }

    num_t __fastcall operator - () const { num_t c(*this);  return c.neg(); }

    num_t & __fastcall from_string (const char *, u8=10U);
    char * __fastcall to_string (char *, u8=10U) const;

            // Arithmetic functions
        // Itself is normalized for all these functions
        // Reference to itself is returned by all these functions except modulo by built-in type

    num_t & __fastcall inc (limb_t a=1) { return ::inc (*this, *this, a); }
    num_t & __fastcall dec (limb_t a=1) { return ::dec (*this, *this, a); }

    template <typename T> num_t & __fastcall add (T a) { return ::add (*this, *this, a); }
    template <typename T> num_t & __fastcall sub (T a) { return ::sub (*this, *this, a); }
    template <typename T> num_t & __fastcall mul (T a) { return ::mul (*this, *this, a); }
    template <typename T> num_t & __fastcall div (T a) { return ::div (*this, *this, a); }
    template <typename T> T __fastcall mod (T a) { T r = ::mod (*this, a);  *this = r;  return r; }

    num_t & __fastcall add (const num_t &a) { return ::add (*this, *this, a); }
    num_t & __fastcall sub (const num_t &a) { return ::sub (*this, *this, a); }
    num_t & __fastcall mul (const num_t &a) { return ::mul (*this, *this, a); }
    num_t & __fastcall div (const num_t &a) { return ::div (*this, *this, a); }
    num_t & __fastcall mod (const num_t &a) { return ::mod (*this, *this, a); }

    num_t & __fastcall addmul (const num_t &a, limb_t b) { return ::addmul (*this, *this, a, b); }
    num_t & __fastcall submul (const num_t &a, limb_t b) { return ::submul (*this, *this, a, b); }

    num_t & __fastcall shift_left (bitcnt_t shift) { return ::shift_left (*this, *this, shift); }
    num_t & __fastcall shift_right (bitcnt_t shift) { return ::shift_right (*this, *this, shift); }

        // Helpers.

    num_t & __fastcall operator ++ () { return this->inc(); }
    num_t & __fastcall operator -- () { return this->dec(); }

    template <typename T> const num_t & __fastcall operator += (T a) { return add (a); }
    template <typename T> const num_t & __fastcall operator -= (T a) { return sub (a); }
    template <typename T> const num_t & __fastcall operator *= (T a) { return mul (a); }
    template <typename T> const num_t & __fastcall operator /= (T a) { return div (a); }
    template <typename T> T __fastcall operator %= (T a) { return mod (a); }

    const num_t & __fastcall operator += (const num_t &a) { return add (a); }
    const num_t & __fastcall operator -= (const num_t &a) { return sub (a); }
    const num_t & __fastcall operator *= (const num_t &a) { return mul (a); }
    const num_t & __fastcall operator /= (const num_t &a) { return div (a); }
    const num_t & __fastcall operator %= (const num_t &a) { return mod (a); }

    const num_t & __fastcall operator <<= (bitcnt_t shift) { return shift_left (shift); }
    const num_t & __fastcall operator >>= (bitcnt_t shift) { return shift_right (shift); }

};



template <typename T> inline const bool __fastcall operator <  (const num_t &a, T b) { return a.compare(b) < 0; }
template <typename T> inline const bool __fastcall operator >  (const num_t &a, T b) { return a.compare(b) > 0; }
template <typename T> inline const bool __fastcall operator <= (const num_t &a, T b) { return a.compare(b) <= 0; }
template <typename T> inline const bool __fastcall operator >= (const num_t &a, T b) { return a.compare(b) >= 0; }
template <typename T> inline const bool __fastcall operator != (const num_t &a, T b) { return a.compare(b) != 0; }
template <typename T> inline const bool __fastcall operator == (const num_t &a, T b) { return a.compare(b) == 0; }

template <typename T> inline const bool __fastcall operator <  (T a, const num_t &b) { return b.compare(a) > 0; }
template <typename T> inline const bool __fastcall operator >  (T a, const num_t &b) { return b.compare(a) < 0; }
template <typename T> inline const bool __fastcall operator <= (T a, const num_t &b) { return b.compare(a) >= 0; }
template <typename T> inline const bool __fastcall operator >= (T a, const num_t &b) { return b.compare(a) <= 0; }
template <typename T> inline const bool __fastcall operator != (T a, const num_t &b) { return b.compare(a) != 0; }
template <typename T> inline const bool __fastcall operator == (T a, const num_t &b) { return b.compare(a) == 0; }

inline const bool __fastcall operator <  (const num_t &a, const num_t &b) { return a.compare(b) < 0; }
inline const bool __fastcall operator >  (const num_t &a, const num_t &b) { return a.compare(b) > 0; }
inline const bool __fastcall operator <= (const num_t &a, const num_t &b) { return a.compare(b) <= 0; }
inline const bool __fastcall operator >= (const num_t &a, const num_t &b) { return a.compare(b) >= 0; }
inline const bool __fastcall operator == (const num_t &a, const num_t &b) { return a.compare(b) == 0; }
inline const bool __fastcall operator != (const num_t &a, const num_t &b) { return a.compare(b) != 0; }



    // Auxiliary function for arithmetic operations.
    // In all cases 'c' should have enough memory space, 'a' and 'b' are normalized.
    // Return reference to 'c' in all cases except modulo by built-in data types.



num_t & __fastcall diff (num_t &c, const vec_t &a, const vec_t &b);

template <typename T>
inline num_t & __fastcall diff (num_t &c, const vec_t &a, T b) {
  assert (b >= 0);	// The function is only for unsigned data types
  if (b == 0)  return c = a;
  if (a.is_zero())  return ((c=b).neg());
  sign_t s = a.compare(b);
  if (s == 0)  return c.set_zero();
  vec_t C(c);
  if (s > 0)  c.size = (offset_t)::sub (C, a, b).size;
  else if (fit_into_one_limb<T>()) {	// a<b means a[0]<b;
    c.size = -1;
    c.limbs[0] = b - c.limbs[0];
  } else  c.size = -(offset_t)::sub (C, vec_t(b), a);
  C.detach();
  return c;
}



inline num_t & __fastcall inc (num_t &c, const num_t &a, limb_t b) {
  assert (a.is_normalized());
  if (a.size >= 0) {
    if (c.limbs == a.limbs)  c.size = (offset_t)::add (a.limbs, (size_t)a.size, b); // In-place (++a).
    else  c.size = (offset_t)::add (c.limbs, a.limbs, (size_t)a.size, b); // Not in-place (c=a+1).
  } else  diff (c, vec_t(a), b).neg();
  return c;
}

inline num_t & __fastcall dec (num_t &c, const num_t &a, limb_t b) {
  assert (a.is_normalized());
  if (a.size < 0) {
    if (c.limbs == a.limbs)  c.size = -(offset_t)::add (a.limbs, (size_t)(-a.size), b); // In-place (--a).
    else  c.size = -(offset_t)::add (c.limbs, a.limbs, (size_t)(-a.size), b); // Not in-place (c=a-1).
  } else  diff (c, vec_t(a), b);
  return c;
}



num_t & __fastcall add (num_t &, const num_t &, const num_t &);

template <typename T>
inline num_t & __fastcall add (num_t &c, const num_t &a, T b) {
  assert (a.is_normalized());
  if (b == 0)  return c = a;
  if (fit_into_one_limb<T>())  add (c, a, num_t(b));	// !!! Not optimal
  else  add (c, a, num_t(b));
  return c;
}



num_t & __fastcall sub (num_t &, const num_t &, const num_t &);

template <typename T>
inline num_t & __fastcall sub (num_t &c, const num_t &a, T b) {
  assert (a.is_normalized());
  if (b == 0)  return c = a;
  if (fit_into_one_limb<T>())  sub (c, a, num_t(b));	// !!! Not optimal
  else  sub (c, a, num_t(b));
  return c;
}

num_t & __fastcall mul (num_t &, const num_t &, const num_t &);

template <typename T>
inline num_t & __fastcall mul (num_t &c, const num_t &a, T b) {
  assert (a.is_normalized());
  if (a.is_zero() || b == 0)  return c.set_zero();
  if (b == 1)  return c = a;
  //if (is_power_of_2(b))  return shift_left (c, a, pos_of_high_one(b));
  if (fit_into_one_limb<T>())  mul (c, a, num_t(b));	// !!! Not optimal
  else  mul (c, a, num_t(b));
  return c;
}



num_t & __fastcall div_qr (num_t *, num_t *, const num_t &, const num_t &);

template <typename T>
inline T __fastcall div_qr (num_t *q, const num_t &a, T b) {
  assert (a.is_normalized());
  assert (b > 0);
  if (a.is_zero()) { if (q != nullptr)  q->set_zero();  return 0; }
  if (b == 1) { if (q != nullptr)  *q = a;  return 0; }
  if (a < b) { if (q != nullptr) q->set_zero();  return (T)a; }
  T r = 0;	// The resulting reminder to return.
  /*if (is_power_of_2(b)) {
    if (q != nullptr)  shift_right (*q, a, pos_of_high_one(b));
    if (fit_into_one_limb<T>())  r = (T)a[0];
    else  for (size_t i=0; i<a.size && i<limbs_in_<T>(); ++i)  r |= a[i]<<(LIMB_BITS*i);
    return r & (b-1);
  }*/
  if (fit_into_one_limb<T>()) {	// !!! Not optimal
    num_t R (size_in_limbs<T>(), CI_SIZE);
    div_qr (q, &R, a, num_t(b));
    r = R;
  } else {
    num_t R (limbs_in_<T>(), CI_SIZE);
    div_qr (q, &R, a, num_t(b));
    r = R;
  }
  return r;
}

template <typename T>
inline num_t & __fastcall div (num_t &q, const num_t &a, T b) { div_qr (&q, a, b);  return q; }

template <typename T>
inline T __fastcall mod (const num_t &a, T b) { return div_qr (nullptr, a, b); }


    // Helpers



template <typename T> inline size_t size_after_add (const num_t &a, T b) { return max (a.abs_size(), size_in_limbs<T>(b)) + 1; }
template <typename T> inline size_t size_after_sub (const num_t &a, T b) { return max (a.abs_size(), size_in_limbs<T>(b)) + 1; }
template <typename T> inline size_t size_after_mul (const num_t &a, T b) { return a.abs_size() + size_in_limbs<T>(b); }
template <typename T> inline size_t size_after_div (const num_t &a, T b) { return a.abs_size()>=size_in_limbs<T>(b) ? a.abs_size() - size_in_limbs<T>(b) + 1 : 1; }

inline size_t size_after_add (const num_t &a, const num_t &b) { return max (a.abs_size(), b.abs_size()) + 1; }
inline size_t size_after_sub (const num_t &a, const num_t &b) { return max (a.abs_size(), b.abs_size()) + 1; }
inline size_t size_after_mul (const num_t &a, const num_t &b) { return a.abs_size() + b.abs_size(); }
inline size_t size_after_div (const num_t &a, const num_t &b) { return a.abs_size()>=b.abs_size() ? a.abs_size() - b.abs_size() + 1 : 1; }
inline size_t size_after_mod (const num_t &a, const num_t &b) { return b.abs_size(); }
inline size_t size_after_shift_left (const num_t &a, bitcnt_t shift) { return a.abs_size() + (shift+LIMB_BITS-1)/LIMB_BITS; }
inline size_t size_after_shift_right (const num_t &a, bitcnt_t shift) { return a.abs_size(); }

template <typename T> inline num_t __fastcall operator + (const num_t &a, T b) { num_t c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
template <typename T> inline num_t __fastcall operator - (const num_t &a, T b) { num_t c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
template <typename T> inline num_t __fastcall operator * (const num_t &a, T b) { num_t c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
template <typename T> inline num_t __fastcall operator / (const num_t &a, T b) { num_t c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
template <typename T> inline T __fastcall operator % (const num_t &a, T b) { return mod (a, b); }

template <typename T> inline num_t __fastcall operator + (T a, const num_t &b) { num_t c (size_after_add (b, a), CI_SIZE);  return add (c, b, a); }
template <typename T> inline num_t __fastcall operator - (T a, const num_t &b) { num_t A(a), c (size_after_sub (A, b), CI_SIZE);  return sub (c, A, b); }
template <typename T> inline num_t __fastcall operator * (T a, const num_t &b) { num_t c (size_after_mul (b, a), CI_SIZE);  return mul (c, b, a); }
template <typename T> inline num_t __fastcall operator / (T a, const num_t &b) { num_t A(a), c (size_after_div (A, b), CI_SIZE);  return div (c, A, b); }
template <typename T> inline num_t __fastcall operator % (T a, const num_t &b) { num_t A(a), c (size_after_mod (A, b), CI_SIZE); return mod (c, A, b); }

inline num_t __fastcall operator + (const num_t &a, const num_t &b) { num_t c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
inline num_t __fastcall operator - (const num_t &a, const num_t &b) { num_t c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
inline num_t __fastcall operator * (const num_t &a, const num_t &b) { num_t c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
inline num_t __fastcall operator / (const num_t &a, const num_t &b) { num_t c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
inline num_t __fastcall operator % (const num_t &a, const num_t &b) { num_t c (size_after_mod (a, b), CI_SIZE);  return mod (c, a, b); }

inline num_t __fastcall operator << (const num_t &a, bitcnt_t shift) { num_t c (size_after_shift_left (a, shift), CI_SIZE);  return shift_left (c, a, shift); }
inline num_t __fastcall operator >> (const num_t &a, bitcnt_t shift) { num_t c (size_after_shift_right (a, shift), CI_SIZE);  return shift_right (c, a, shift); }



#endif
