// Class 'vec_t' - to better work with vectors of limbs.

#ifndef VEC_T_H
#define VEC_T_H
#pragma once

#include "typedef.h"
#include "misc.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"



    // How to interpret input parameter in constructor 'vec_t(n)'?
enum ConstructorInterpreter {
  CI_SIZE,	// As a size (in limbs).
  CI_NUMBER	// As a number.
};



class vec_t;
class num_t;


vec_t & __fastcall inc (vec_t &c, const vec_t &a, limb_t b=1);
vec_t & __fastcall dec (vec_t &c, const vec_t &a, limb_t b=1);
vec_t & __fastcall add (vec_t &, const vec_t &, const vec_t &);
template <typename T> inline vec_t & __fastcall add (vec_t &c, const vec_t &a, T b);
vec_t & __fastcall sub (vec_t &, const vec_t &, const vec_t &);
template <typename T> inline vec_t & __fastcall sub (vec_t &c, const vec_t &a, T b);
vec_t & __fastcall mul (vec_t &, const vec_t &, const vec_t &);
template <typename T> inline vec_t & __fastcall mul (vec_t &c, const vec_t &a, T b);
vec_t & __fastcall div_qr (vec_t *, vec_t *, const vec_t &, const vec_t &);
template <typename T> inline T __fastcall div_qr (vec_t *q, const vec_t &a, T b);
template <typename T> inline vec_t & __fastcall div (vec_t &q, const vec_t &a, T b);
template <typename T> inline T __fastcall mod (const vec_t &a, T b);
vec_t & __fastcall div (vec_t &c, const vec_t &a, const vec_t &b);
vec_t & __fastcall mod (vec_t &c, const vec_t &a, const vec_t &b);
vec_t & __fastcall addmul (vec_t &c, const vec_t &a, const vec_t &b, limb_t s);
vec_t & __fastcall submul (vec_t &c, const vec_t &a, const vec_t &b, limb_t s);
vec_t & __fastcall shift_left (vec_t &c, const vec_t &a, bitcnt_t shift);
vec_t & __fastcall shift_right (vec_t &c, const vec_t &a, bitcnt_t shift);


class vec_t {
  public:
    size_t size;    // Size of (not necessary normalized) vector. 'size' is NEVER exceed the number of allocated limbs.
    limb_t *limbs;  // Array of limbs.



    		// Constructors and destructor
    vec_t(): size(0), limbs (nullptr) {} // Empty vector is initially zero.
    vec_t (const vec_t &that): limbs (new limb_t[that.size]) { *this = that; }
    vec_t (vec_t &&that): size (that.size), limbs (that.limbs) { that.limbs = nullptr; }
    vec_t (const num_t &);  // Attach vector to signed long number
    vec_t (const char *, u8=10);	// Constructor by string with default base (10)
    template <typename T>
    vec_t (T a, ConstructorInterpreter ci=CI_NUMBER): size(0) {
      assert (a >= 0);	// Use this constructor only for unsigned types of non-negative numbers.
      if (ci == CI_NUMBER) {
        limbs = new limb_t[size_in_limbs<T>(a)];
        *this = a;
      } else limbs = new limb_t[(size_t)a];
    }
    ~vec_t() { delete[] limbs; }



        // Attach vector to signed long number
        // Initially this->limbs == nullptr.
    void __fastcall attach (const num_t &);

        // Detach vector from previously attached signed long number
    void __fastcall detach() { limbs = nullptr; }

        // Free memory for limbs and make this->size = 0.
        // Return reference to itself.
    vec_t & __fastcall clear() {
      size = 0;
      delete[] limbs;
      limbs = nullptr;
      return *this;
    }

        // Resize limbs.
        // It doesn't copy old limbs to new. ALL data will be cleared.
        // The old size doesn't matter.
        // Vector becomes zero.
    void __fastcall resize (size_t);

        // Adapt limbs to the given size.
        // Nothing is changed if this->size >= the given size.
    void __fastcall adapt (size_t);

				// Normalize vector size.
				// Return normalized size.
    size_t __fastcall normalize() { return size = normalize_size (limbs, size); }



    		// Compare with other vector with the same size.
    		// Both vectors are normalized and non-zero, also this->size == that.size.
    		// Return the result of comparison.
    sign_t __fastcall compare_equal_size (const vec_t &that) const;

    		// Compare with other vector.
    		// Both vectors are normalized.
    		// Return the result of comparison.
    sign_t __fastcall compare (const vec_t &that) const;

        // Compare with a value of built-in data type.
    		// Vector is normalized, 'a' is unsigned or non-negative
    		// Return the result of comparison.
    template <typename T>
    sign_t __fastcall compare (T a) const {
      assert (is_normalized());
      assert (a >= 0);
      if (fit_into_one_limb<T>()) {
        if (size > 1)  return +1;
        if (size == 0) return 0 - (a!=0);
        if (limbs[0] < (limb_t)a)  return -1;
        if (limbs[0] > (limb_t)a)  return +1;
        return 0;
      } else return compare (vec_t(a));
    }

    bool __fastcall operator == (const char *str) const { return compare (vec_t(str)) == 0; }

    		// Get the copy of other vector without memory allocation.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    vec_t & __fastcall operator = (const vec_t &);

        // Convert vector to built-in data type.
    template <typename T> __fastcall operator T() const {
      if (is_zero())  return 0;
      if (sizeof(T) >= sizeof(limb_t)) {
        T res = 0;
        for (size_t i=0; i<size && i<sizeof(T)/sizeof(limb_t); ++i)  res |= ((T)limbs[i]<<(LIMB_BITS*i));
        return res;
      }
      return (T)limbs[0];
    }



        // Get vector from decimal string.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    vec_t & __fastcall operator = (const char *str) { return from_string (str); }



        // Get vector from an positive signed or unsigned value.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    template <typename T>
    vec_t & __fastcall operator = (T a) {
      assert (a >= 0);
      size = 0;
      split (limbs, size, a);
      return *this;
    }



        // Move and swap with other vector
    vec_t & __fastcall move (vec_t &that) { return clear().swap (that); }
    vec_t & __fastcall swap (vec_t &that) { ::swap (size, that.size);  ::swap (limbs, that.limbs);  return *this; }

    vec_t & __fastcall from_string (const char *str, u8 base=10) { size = ::from_string (limbs, str, base);  return *this; }
    char * __fastcall to_string (char *str, u8 base=10) const { return ::to_string (str, limbs, size, base); };



    		// Move the other vector to itself.
        // Return the reference to itself.
        // The other vector becomes empty (that.limbs=nullptr).
    vec_t & __fastcall operator = (vec_t &&that) { return this->move(that); }



    		// Helpers [] for mutable and constant reference.
    limb_t & __fastcall operator [] (size_t index) { return limbs[index]; }
    const limb_t & __fastcall operator [] (size_t index) const { return limbs[index]; }



        // Check if vector is zero.
    bool __fastcall is_zero() const { return size == 0; }

        // Set vector to zero. Return reference to itself.
    vec_t & __fastcall set_zero() { size = 0;  return *this; }

        // Check if vector is normalized.
    bool __fastcall is_normalized() const { return size==0 || limbs[size-1]!=0; }



        // Arithmetic functions
        // Itself is normalized for all these functions
        // Reference to itself is returned by all these functions except modulo by built-in type

    vec_t & __fastcall inc (limb_t a=1) { return ::inc (*this, *this, a); }
    vec_t & __fastcall dec (limb_t a=1) { return ::dec (*this, *this, a); }

    template <typename T> vec_t & __fastcall add (T a) { return ::add (*this, *this, a); }
    template <typename T> vec_t & __fastcall sub (T a) { return ::sub (*this, *this, a); }
    template <typename T> vec_t & __fastcall mul (T a) { return ::mul (*this, *this, a); }
    template <typename T> vec_t & __fastcall div (T a) { return ::div (*this, *this, a); }
    template <typename T> T __fastcall mod (T a) { T r = ::mod (*this, a);  *this = r;  return r; }

    vec_t & __fastcall add (const vec_t &a) { return ::add (*this, *this, a); }
    vec_t & __fastcall sub (const vec_t &a) { return ::sub (*this, *this, a); }
    vec_t & __fastcall mul (const vec_t &a) { return ::mul (*this, *this, a); }
    vec_t & __fastcall div (const vec_t &a) { return ::div (*this, *this, a); }
    vec_t & __fastcall mod (const vec_t &a) { return ::mod (*this, *this, a); }

    vec_t & __fastcall addmul (const vec_t &a, limb_t b) { return ::addmul (*this, *this, a, b); }
    vec_t & __fastcall submul (const vec_t &a, limb_t b) { return ::submul (*this, *this, a, b); }

    vec_t & __fastcall shift_left (bitcnt_t shift) { return ::shift_left (*this, *this, shift); }
    vec_t & __fastcall shift_right (bitcnt_t shift) { return ::shift_right (*this, *this, shift); }

        // Helpers.

    vec_t & __fastcall operator ++ () { return this->inc(); }
    vec_t & __fastcall operator -- () { return this->dec(); }

    template <typename T> const vec_t & __fastcall operator += (T a) { return add (a); }
    template <typename T> const vec_t & __fastcall operator -= (T a) { return sub (a); }
    template <typename T> const vec_t & __fastcall operator *= (T a) { return mul (a); }
    template <typename T> const vec_t & __fastcall operator /= (T a) { return div (a); }
    template <typename T> T __fastcall operator %= (T a) { return mod (a); }

    const vec_t & __fastcall operator += (const vec_t &a) { return add (a); }
    const vec_t & __fastcall operator -= (const vec_t &a) { return sub (a); }
    const vec_t & __fastcall operator *= (const vec_t &a) { return mul (a); }
    const vec_t & __fastcall operator /= (const vec_t &a) { return div (a); }
    const vec_t & __fastcall operator %= (const vec_t &a) { return mod (a); }

    const vec_t & __fastcall operator <<= (bitcnt_t shift) { return shift_left (shift); }
    const vec_t & __fastcall operator >>= (bitcnt_t shift) { return shift_right (shift); }

};



template <typename T> inline const bool __fastcall operator <  (const vec_t &a, T b) { return a.compare(b) < 0; }
template <typename T> inline const bool __fastcall operator >  (const vec_t &a, T b) { return a.compare(b) > 0; }
template <typename T> inline const bool __fastcall operator <= (const vec_t &a, T b) { return a.compare(b) <= 0; }
template <typename T> inline const bool __fastcall operator >= (const vec_t &a, T b) { return a.compare(b) >= 0; }
template <typename T> inline const bool __fastcall operator != (const vec_t &a, T b) { return a.compare(b) != 0; }
template <typename T> inline const bool __fastcall operator == (const vec_t &a, T b) { return a.compare(b) == 0; }

template <typename T> inline const bool __fastcall operator <  (T a, const vec_t &b) { return b.compare(a) > 0; }
template <typename T> inline const bool __fastcall operator >  (T a, const vec_t &b) { return b.compare(a) < 0; }
template <typename T> inline const bool __fastcall operator <= (T a, const vec_t &b) { return b.compare(a) >= 0; }
template <typename T> inline const bool __fastcall operator >= (T a, const vec_t &b) { return b.compare(a) <= 0; }
template <typename T> inline const bool __fastcall operator != (T a, const vec_t &b) { return b.compare(a) != 0; }
template <typename T> inline const bool __fastcall operator == (T a, const vec_t &b) { return b.compare(a) == 0; }

inline const bool __fastcall operator <  (const vec_t &a, const vec_t &b) { return a.compare(b) < 0; }
inline const bool __fastcall operator >  (const vec_t &a, const vec_t &b) { return a.compare(b) > 0; }
inline const bool __fastcall operator <= (const vec_t &a, const vec_t &b) { return a.compare(b) <= 0; }
inline const bool __fastcall operator >= (const vec_t &a, const vec_t &b) { return a.compare(b) >= 0; }
inline const bool __fastcall operator == (const vec_t &a, const vec_t &b) { return a.compare(b) == 0; }
inline const bool __fastcall operator != (const vec_t &a, const vec_t &b) { return a.compare(b) != 0; }



    // Auxiliary function for arithmetic operations.
    // In all cases 'c' should have enough memory space, 'a' and 'b' are normalized.
    // Return reference to 'c' in all cases except modulo by built-in data types.



inline vec_t & __fastcall inc (vec_t &c, const vec_t &a, limb_t b) {
  assert (a.is_normalized());
  if (c.limbs == a.limbs)  c.size = ::add (a.limbs, a.size, b); // In-place (++a).
  else  c.size = ::add (c.limbs, a.limbs, a.size, b); // Not in-place (c=a+1).
  return c;
}

inline vec_t & __fastcall dec (vec_t &c, const vec_t &a, limb_t b) {
  assert (a.is_normalized());
  if (c.limbs == a.limbs)  c.size = ::sub (a.limbs, a.size, b); // In-place (--a).
  else  c.size = ::sub (c.limbs, a.limbs, a.size, b); // Not in-place (c=a-1).
  return c;
}



vec_t & __fastcall add (vec_t &, const vec_t &, const vec_t &);

template <typename T>
inline vec_t & __fastcall add (vec_t &c, const vec_t &a, T b) {
  assert (a.is_normalized());
  assert (b >= 0);
  if (b == 0)  return c = a;
  if (fit_into_one_limb<T>())  inc (c, a, (limb_t)b);
  else  add (c, a, vec_t(b));
  return c;
}



vec_t & __fastcall sub (vec_t &, const vec_t &, const vec_t &);

template <typename T>
inline vec_t & __fastcall sub (vec_t &c, const vec_t &a, T b) {
  assert (a.is_normalized());
  assert (b >= 0);
  if (b == 0)  return c = a;
  if (fit_into_one_limb<T>())  dec (c, a, (limb_t)b);
  else  sub (c, a, vec_t(b));
  return c;
}

vec_t & __fastcall mul (vec_t &, const vec_t &, const vec_t &);

template <typename T>
inline vec_t & __fastcall mul (vec_t &c, const vec_t &a, T b) {
  assert (a.is_normalized());
  assert (b >= 0);
  if (a.is_zero() || b == 0)  return c.set_zero();
  if (b == 1)  return c = a;
  if (is_power_of_2(b))  return shift_left (c, a, pos_of_high_one(b));
  if (fit_into_one_limb<T>())  c.size = ::mul (c.limbs, a.limbs, a.size, (limb_t)b);
  else  mul (c, a, vec_t(b));
  return c;
}



vec_t & __fastcall div_qr (vec_t *, vec_t *, const vec_t &, const vec_t &);

template <typename T>
inline T __fastcall div_qr (vec_t *q, const vec_t &a, T b) {
  assert (a.is_normalized());
  assert (b > 0);
  if (a.is_zero()) { if (q != nullptr)  q->set_zero();  return 0; }
  if (b == 1) { if (q != nullptr)  *q = a;  return 0; }
  if (a < b) { if (q != nullptr) q->set_zero();  return (T)a; }
  T r = 0;	// The resulting reminder to return.
  if (is_power_of_2(b)) {
    if (q != nullptr)  shift_right (*q, a, pos_of_high_one(b));
    if (fit_into_one_limb<T>())  r = (T)a[0];
    else  for (size_t i=0; i<a.size && i<limbs_in_<T>(); ++i)  r |= a[i]<<(LIMB_BITS*i);
    return r & (b-1);
  }
  if (fit_into_one_limb<T>()) {
    limb_t R;
    if (q == nullptr)  ::div_qr (nullptr, &R, a.limbs, a.size, (limb_t)b);
    else  q->size = ::div_qr (q->limbs, &R, a.limbs, a.size, (limb_t)b);
    r = (T)R;
  } else {
    vec_t R (limbs_in_<T>(), CI_SIZE);
    div_qr (q, &R, a, vec_t(b));
    r = R;
  }
  return r;
}

template <typename T>
inline vec_t & __fastcall div (vec_t &q, const vec_t &a, T b) { div_qr (&q, a, b);  return q; }

template <typename T>
inline T __fastcall mod (const vec_t &a, T b) { return div_qr (nullptr, a, b); }


    // Helpers



template <typename T> inline size_t size_after_add (const vec_t &a, T b) { return max (a.size, size_in_limbs<T>(b)) + 1; }
template <typename T> inline size_t size_after_sub (const vec_t &a, T b) { return a.size; }
template <typename T> inline size_t size_after_mul (const vec_t &a, T b) { return a.size + size_in_limbs<T>(b); }
template <typename T> inline size_t size_after_div (const vec_t &a, T b) { return a.size>=size_in_limbs<T>(b) ? a.size - size_in_limbs<T>(b) + 1 : 1; }

inline size_t size_after_add (const vec_t &a, const vec_t &b) { return max (a.size, b.size) + 1; }
inline size_t size_after_sub (const vec_t &a, const vec_t &b) { return a.size; }
inline size_t size_after_mul (const vec_t &a, const vec_t &b) { return a.size + b.size; }
inline size_t size_after_div (const vec_t &a, const vec_t &b) { return a.size>=b.size ? a.size - b.size + 1 : 1; }
inline size_t size_after_mod (const vec_t &a, const vec_t &b) { return b.size; }
inline size_t size_after_shift_left (const vec_t &a, bitcnt_t shift) { return a.size + (shift+LIMB_BITS-1)/LIMB_BITS; }
inline size_t size_after_shift_right (const vec_t &a, bitcnt_t shift) { return a.size; }

template <typename T> inline vec_t __fastcall operator + (const vec_t &a, T b) { vec_t c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
template <typename T> inline vec_t __fastcall operator - (const vec_t &a, T b) { vec_t c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
template <typename T> inline vec_t __fastcall operator * (const vec_t &a, T b) { vec_t c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
template <typename T> inline vec_t __fastcall operator / (const vec_t &a, T b) { vec_t c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
template <typename T> inline T __fastcall operator % (const vec_t &a, T b) { return mod (a, b); }

template <typename T> inline vec_t __fastcall operator + (T a, const vec_t &b) { vec_t c (size_after_add (b, a), CI_SIZE);  return add (c, b, a); }
template <typename T> inline vec_t __fastcall operator - (T a, const vec_t &b) { vec_t A(a), c (size_after_sub (A, b), CI_SIZE);  return sub (c, A, b); }
template <typename T> inline vec_t __fastcall operator * (T a, const vec_t &b) { vec_t c (size_after_mul (b, a), CI_SIZE);  return mul (c, b, a); }
template <typename T> inline vec_t __fastcall operator / (T a, const vec_t &b) { vec_t A(a), c (size_after_div (A, b), CI_SIZE);  return div (c, A, b); }
template <typename T> inline vec_t __fastcall operator % (T a, const vec_t &b) { vec_t A(a), c (size_after_mod (A, b), CI_SIZE); return mod (c, A, b); }

inline vec_t __fastcall operator + (const vec_t &a, const vec_t &b) { vec_t c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
inline vec_t __fastcall operator - (const vec_t &a, const vec_t &b) { vec_t c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
inline vec_t __fastcall operator * (const vec_t &a, const vec_t &b) { vec_t c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
inline vec_t __fastcall operator / (const vec_t &a, const vec_t &b) { vec_t c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
inline vec_t __fastcall operator % (const vec_t &a, const vec_t &b) { vec_t c (size_after_mod (a, b), CI_SIZE);  return mod (c, a, b); }

inline vec_t __fastcall operator << (const vec_t &a, bitcnt_t shift) { vec_t c (size_after_shift_left (a, shift), CI_SIZE);  return shift_left (c, a, shift); }
inline vec_t __fastcall operator >> (const vec_t &a, bitcnt_t shift) { vec_t c (size_after_shift_right (a, shift), CI_SIZE);  return shift_right (c, a, shift); }

#endif
