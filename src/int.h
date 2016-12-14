// Class 'Int' - an arbitrary precision integers

#ifndef INT_H
#define INT_H
#pragma once

#include "typedef.h"
#include "num_t.h"



class Int;



template <typename T> inline size_t size_after_add (const Int &a, T b);
template <typename T> inline size_t size_after_sub (const Int &a, T b);
template <typename T> inline size_t size_after_mul (const Int &a, T b);
template <typename T> inline size_t size_after_div (const Int &a, T b);
template <typename T> inline size_t size_after_mod (const Int &a, T b);

inline size_t size_after_add (const Int &a, const Int &b);
inline size_t size_after_sub (const Int &a, const Int &b);
inline size_t size_after_mul (const Int &a, const Int &b);
inline size_t size_after_div (const Int &a, const Int &b);
inline size_t size_after_mod (const Int &a, const Int &b);

inline size_t size_after_shift_left (const Int &a, bitcnt_t s);
inline size_t size_after_shift_right (const Int &a, bitcnt_t s);



Int & __fastcall inc (Int &c, const Int &a, limb_t b=1);
Int & __fastcall dec (Int &c, const Int &a, limb_t b=1);
Int & __fastcall add (Int &, const Int &, const Int &);
template <typename T> inline Int & __fastcall add (Int &c, const Int &a, T b);
Int & __fastcall sub (Int &, const Int &, const Int &);
template <typename T> inline Int & __fastcall sub (Int &c, const Int &a, T b);
Int & __fastcall mul (Int &, const Int &, const Int &);
template <typename T> inline Int & __fastcall mul (Int &c, const Int &a, T b);
Int & __fastcall div_qr (Int *, Int *, const Int &, const Int &, DivisionMode=DM_EUCLIDEAN);
template <typename T> inline T __fastcall div_qr (Int *q, const Int &a, T b, DivisionMode=DM_EUCLIDEAN);
template <typename T> inline Int & __fastcall div (Int &q, const Int &a, T b, DivisionMode=DM_EUCLIDEAN);
template <typename T> inline T __fastcall mod (const Int &a, T b, DivisionMode=DM_EUCLIDEAN);
Int & __fastcall div (Int &c, const Int &a, const Int &b, DivisionMode=DM_EUCLIDEAN);
Int & __fastcall mod (Int &c, const Int &a, const Int &b, DivisionMode=DM_EUCLIDEAN);
Int & __fastcall shift_left (Int &c, const Int &a, bitcnt_t shift);
Int & __fastcall shift_right (Int &c, const Int &a, bitcnt_t shift);



class Int {
    num_t data;	// A long number, always normalized.
    size_t allocated;  // How many limbs are allocated. |data.size| is NEVER exceed the number of allocated limbs.

  public:

    		// Constructors and destructor
    Int(): data(), allocated(0) {} // Empty vector is initially zero.
    Int (const Int &that): data (that.data), allocated (that.size()) {}
    Int (Int &&that): data (that.data), allocated (that.allocated) {}
    Int (const char *str, u8 base=10): data (str, base) {	// Constructor by string with default base (10)
      allocated = data.abs_size();
    }
    template <typename T>
    Int (T a, ConstructorInterpreter ci=CI_NUMBER): allocated(0) {
      if (ci == CI_NUMBER)  *this = a;
      else  adapt((size_t)a);
    }
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
    size_t __fastcall size() const { return data.abs_size(); }

        // Comparison
    sign_t __fastcall compare (const Int &that) const { return data.compare (that.data); };
    template <typename T>  sign_t __fastcall compare (T a) const { return compare (Int(a)); }
    bool __fastcall operator == (const char *str) const { return data.compare (str) == 0; }




        // Get the number by different ways.
    Int & __fastcall operator = (const Int &that) {
      if (this != &that) {
        adapt (that.size());
        data = that.data;
      }
      return *this;
    }

    Int & __fastcall operator = (const char *str) { adapt (how_many_digits (str));  data = str;  return *this; }



    // Convert number to built-in data type.
    template <typename T> __fastcall operator T() const { return (T)data; }



        // Get vector from signed or unsigned value.
    		// The enough memory for itself should be allocated.
        // Return the reference to itself.
    template <typename T>
    Int & __fastcall operator = (T a) {
      adapt (size_in_limbs<T> (a));
      data = a;
    }



    Int & __fastcall move (Int &that) {
      clear();
      return this->swap(that);
    }



    Int & __fastcall swap (Int &that) {
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
    Int &__fastcall neg() { data.neg();  return *this; }

    		// Unary minus.
    Int __fastcall operator - () const { Int z (*this);  return z.neg(); }



        // Arithmetic functions
        // Itself is normalized for all these functions
        // Reference to itself is returned by all these functions except modulo by built-in type



    Int & __fastcall inc (limb_t a=1) { return ::inc (*this, *this, a); }
    Int & __fastcall dec (limb_t a=1) { return ::dec (*this, *this, a); }

    template <typename T> Int & __fastcall add (T a) { return ::add (*this, *this, a); }
    template <typename T> Int & __fastcall sub (T a) { return ::sub (*this, *this, a); }
    template <typename T> Int & __fastcall mul (T a) { return ::mul (*this, *this, a); }
    template <typename T> Int & __fastcall div (T a, DivisionMode dm=DM_EUCLIDEAN) { return ::div (*this, *this, a, dm); }
    template <typename T> T __fastcall mod (T a, DivisionMode dm=DM_EUCLIDEAN) { T r = ::mod (*this, a, dm);  *this = r;  return r; }

    Int & __fastcall add (const Int &a) { return ::add (*this, *this, a); }
    Int & __fastcall sub (const Int &a) { return ::sub (*this, *this, a); }
    Int & __fastcall mul (const Int &a) { return ::mul (*this, *this, a); }
    Int & __fastcall div (const Int &a, DivisionMode dm=DM_EUCLIDEAN) { return ::div (*this, *this, a, dm); }
    Int & __fastcall mod (const Int &a, DivisionMode dm=DM_EUCLIDEAN) { return ::mod (*this, *this, a, dm); }

    Int & __fastcall shift_left (bitcnt_t shift) { return ::shift_left (*this, *this, shift); }
    Int & __fastcall shift_right (bitcnt_t shift) { return ::shift_right (*this, *this, shift); }

        // Helpers.

    Int & __fastcall operator ++ () { return this->inc(); }
    Int & __fastcall operator -- () { return this->dec(); }

    template <typename T> Int & __fastcall operator += (T a) { return add (a); }
    template <typename T> Int & __fastcall operator -= (T a) { return sub (a); }
    template <typename T> Int & __fastcall operator *= (T a) { return mul (a); }
    template <typename T> Int & __fastcall operator /= (T a) { return div (a); }
    template <typename T> T __fastcall operator %= (T a) { return mod (a); }

    Int & __fastcall operator += (const Int &a) { return add (a); }
    Int & __fastcall operator -= (const Int &a) { return sub (a); }
    Int & __fastcall operator *= (const Int &a) { return mul (a); }
    Int & __fastcall operator /= (const Int &a) { return div (a); }
    Int & __fastcall operator %= (const Int &a) { return mod (a); }

    Int & __fastcall operator <<= (bitcnt_t shift) { return shift_left (shift); }
    Int & __fastcall operator >>= (bitcnt_t shift) { return shift_right (shift); }

    template <typename T> friend Int & __fastcall add (Int &, const Int &, T);
    template <typename T> friend Int & __fastcall sub (Int &, const Int &, T);
    template <typename T> friend Int & __fastcall mul (Int &, const Int &, T);
    template <typename T> friend inline T __fastcall div_qr (Int *q, const Int &a, T b, DivisionMode dm);
    template <typename T> friend Int & __fastcall div (Int &, const Int &, T, DivisionMode);
    template <typename T> friend T __fastcall mod (Int &, const Int &, T, DivisionMode);

    friend inline Int & __fastcall inc (Int &, const Int &, limb_t);
    friend inline Int & __fastcall dec (Int &, const Int &, limb_t);

    friend Int & __fastcall add (Int &, const Int &, const Int &);
    friend Int & __fastcall sub (Int &, const Int &, const Int &);
    friend Int & __fastcall mul (Int &, const Int &, const Int &);
    friend Int & __fastcall div_qr (Int *q, Int *r, const Int &a, const Int &b, DivisionMode dm);
    friend Int & __fastcall div (Int &, const Int &, const Int &, DivisionMode);
    friend Int & __fastcall mod (Int &, const Int &, const Int &, DivisionMode);

    friend Int & __fastcall shift_left (Int &, const Int &, bitcnt_t);
    friend Int & __fastcall shift_right (Int &, const Int &, bitcnt_t);
};



    // Suffix '_dec' for decimal numbers (begins with digit from 1 to 9).
inline const Int __fastcall operator"" _dec (const char *str) { return Int (str); }
    // Suffix '_hex' for hexadecimal numbers (begins with 0x).
inline const Int __fastcall operator"" _hex (const char *str) { assert (str[0]=='0' && (str[1]=='x'||str[1]=='X'));  return Int (str+2, 16); }
    // Suffix '_bin' for binary numbers (begins with 0).
inline const Int __fastcall operator"" _bin (const char *str) { assert (str[0]=='0');  return Int (str+1, 2); }



template <typename T> inline const bool __fastcall operator <  (const Int &a, T b) { return a.compare(b) < 0; }
template <typename T> inline const bool __fastcall operator >  (const Int &a, T b) { return a.compare(b) > 0; }
template <typename T> inline const bool __fastcall operator <= (const Int &a, T b) { return a.compare(b) <= 0; }
template <typename T> inline const bool __fastcall operator >= (const Int &a, T b) { return a.compare(b) >= 0; }
template <typename T> inline const bool __fastcall operator != (const Int &a, T b) { return a.compare(b) != 0; }
template <typename T> inline const bool __fastcall operator == (const Int &a, T b) { return a.compare(b) == 0; }

template <typename T> inline const bool __fastcall operator <  (T a, const Int &b) { return b.compare(a) > 0; }
template <typename T> inline const bool __fastcall operator >  (T a, const Int &b) { return b.compare(a) < 0; }
template <typename T> inline const bool __fastcall operator <= (T a, const Int &b) { return b.compare(a) >= 0; }
template <typename T> inline const bool __fastcall operator >= (T a, const Int &b) { return b.compare(a) <= 0; }
template <typename T> inline const bool __fastcall operator != (T a, const Int &b) { return b.compare(a) != 0; }
template <typename T> inline const bool __fastcall operator == (T a, const Int &b) { return b.compare(a) == 0; }

inline const bool __fastcall operator <  (const Int &a, const Int &b) { return a.compare(b) < 0; }
inline const bool __fastcall operator >  (const Int &a, const Int &b) { return a.compare(b) > 0; }
inline const bool __fastcall operator <= (const Int &a, const Int &b) { return a.compare(b) <= 0; }
inline const bool __fastcall operator >= (const Int &a, const Int &b) { return a.compare(b) >= 0; }
inline const bool __fastcall operator == (const Int &a, const Int &b) { return a.compare(b) == 0; }
inline const bool __fastcall operator != (const Int &a, const Int &b) { return a.compare(b) != 0; }



// !!! Optimize them (more accurately)

template <typename T> inline size_t size_after_add (const Int &a, T b) { return max (a.size(), size_in_limbs<T>(b)) + 1; }
template <typename T> inline size_t size_after_sub (const Int &a, T b) { return max (a.size(), size_in_limbs<T>(b)) + 1; }
template <typename T> inline size_t size_after_mul (const Int &a, T b) { return a.size() + size_in_limbs<T>(b); }
template <typename T> inline size_t size_after_div (const Int &a, T b) { return a.size()>=size_in_limbs<T>(b) ? a.size() - size_in_limbs<T>(b) + 1 : 1; }

inline size_t size_after_add (const Int &a, const Int &b) { return max (a.size(), b.size()) + 1; }
inline size_t size_after_sub (const Int &a, const Int &b) { return max (a.size(), b.size()) + 1; }
inline size_t size_after_mul (const Int &a, const Int &b) { return a.size() + b.size(); }
inline size_t size_after_div (const Int &a, const Int &b) { return a.size(); }
inline size_t size_after_mod (const Int &a, const Int &b) { return b.size() + 1; }

inline size_t size_after_shift_left (const Int &a, bitcnt_t s) { return a.size() + (s+LIMB_BITS-1)/LIMB_BITS; }
inline size_t size_after_shift_right (const Int &a, bitcnt_t s) { return a.size(); }



    // Auxiliary function for arithmetic operations.
    // In all cases 'c' should have enough memory space, 'a' and 'b' are normalized.
    // Return reference to 'c' in all cases except modulo by built-in data types.



inline Int & __fastcall inc (Int &c, const Int &a, limb_t b) {
  c.adapt (size_after_add (a, b));
  ::inc (c.data, a.data, b);
  return c;
}

inline Int & __fastcall dec (Int &c, const Int &a, limb_t b) {
  c.adapt (size_after_sub (a, b));
  ::dec (c.data, a.data, b);
  return c;
}



Int & __fastcall add (Int &c, const Int &a, const Int &b) { c.adapt (size_after_add (a, b));  ::add (c.data, a.data, b.data);  return c; }
Int & __fastcall sub (Int &c, const Int &a, const Int &b) { c.adapt (size_after_sub (a, b));  ::sub (c.data, a.data, b.data);  return c; }
Int & __fastcall mul (Int &c, const Int &a, const Int &b) { c.adapt (size_after_mul (a, b));  ::mul (c.data, a.data, b.data);  return c; }
Int & __fastcall div_qr (Int *q, Int *r, const Int &a, const Int &b, DivisionMode dm) {
  if (q)  q->adapt (size_after_div (a, b));
  if (r)  r->adapt (size_after_mod (a, b));
  ::div_qr (q?&q->data:(num_t*)nullptr, r?&r->data:(num_t*)nullptr, a.data, b.data, dm);
  return q ? *q : *r;
}
Int & __fastcall div (Int &c, const Int &a, const Int &b, DivisionMode dm) { c.adapt (size_after_div (a, b));  ::div (c.data, a.data, b.data, dm);  return c; }
Int & __fastcall mod (Int &c, const Int &a, const Int &b, DivisionMode dm) { c.adapt (size_after_mod (a, b));  ::mod (c.data, a.data, b.data, dm);  return c; }

template <typename T>
inline Int & __fastcall add (Int &c, const Int &a, T b) {
  c.adapt (size_after_add (a, b));
  ::add (c.data, a.data, b);
  return c;
}



template <typename T>
inline Int & __fastcall sub (Int &c, const Int &a, T b) {
  c.adapt (size_after_sub (a, b));
  ::sub (c.data, a.data, b);
  return c;
}


template <typename T>
inline Int & __fastcall mul (Int &c, const Int &a, T b) {
  c.adapt (size_after_mul (a, b));
  ::mul (c.data, a.data, b);
  return c;
}



template <typename T>
inline T __fastcall div_qr (Int *q, const Int &a, T b, DivisionMode dm) {
  if (q)  q->adapt (size_after_div (a, b));
  T r = ::div_qr (q?&q->data:(num_t*)nullptr, a.data, b, dm);
  return r;
}

template <typename T>
inline Int & __fastcall div (Int &q, const Int &a, T b, DivisionMode dm=DM_EUCLIDEAN) { div_qr (&q, a, b, dm);  return q; }

template <typename T>
inline T __fastcall mod (const Int &a, T b, DivisionMode dm=DM_EUCLIDEAN) { return div_qr (nullptr, a, b, dm); }

Int & __fastcall shift_left (Int &c, const Int &a, bitcnt_t shift) { c.adapt (size_after_shift_left (a, shift));  ::shift_left (c.data, a.data, shift);  return c; }
Int & __fastcall shift_right (Int &c, const Int &a, bitcnt_t shift)  { c.adapt (size_after_shift_right (a, shift));  ::shift_right (c.data, a.data, shift);  return c; }



    // Helpers



template <typename T> inline Int __fastcall operator + (const Int &a, T b) { Int c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
template <typename T> inline Int __fastcall operator - (const Int &a, T b) { Int c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
template <typename T> inline Int __fastcall operator * (const Int &a, T b) { Int c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
template <typename T> inline Int __fastcall operator / (const Int &a, T b) { Int c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
template <typename T> inline T __fastcall operator % (const Int &a, T b) { return mod (a, b); }

template <typename T> inline Int __fastcall operator + (T a, const Int &b) { Int c (size_after_add (b, a), CI_SIZE);  return add (c, b, a); }
template <typename T> inline Int __fastcall operator - (T a, const Int &b) { Int A(a), c (size_after_sub (A, b), CI_SIZE);  return sub (c, A, b); }
template <typename T> inline Int __fastcall operator * (T a, const Int &b) { Int c (size_after_mul (b, a), CI_SIZE);  return mul (c, b, a); }
template <typename T> inline Int __fastcall operator / (T a, const Int &b) { Int A(a), c (size_after_div (A, b), CI_SIZE);  return div (c, A, b); }
template <typename T> inline Int __fastcall operator % (T a, const Int &b) { Int A(a), c (size_after_mod (A, b), CI_SIZE); return mod (c, A, b); }

inline Int __fastcall operator + (const Int &a, const Int &b) { Int c (size_after_add (a, b), CI_SIZE);  return add (c, a, b); }
inline Int __fastcall operator - (const Int &a, const Int &b) { Int c (size_after_sub (a, b), CI_SIZE);  return sub (c, a, b); }
inline Int __fastcall operator * (const Int &a, const Int &b) { Int c (size_after_mul (a, b), CI_SIZE);  return mul (c, a, b); }
inline Int __fastcall operator / (const Int &a, const Int &b) { Int c (size_after_div (a, b), CI_SIZE);  return div (c, a, b); }
inline Int __fastcall operator % (const Int &a, const Int &b) { Int c (size_after_mod (a, b), CI_SIZE);  return mod (c, a, b); }

inline Int __fastcall operator << (const Int &a, bitcnt_t shift) { Int c (size_after_shift_left (a, shift), CI_SIZE);  return shift_left (c, a, shift); }
inline Int __fastcall operator >> (const Int &a, bitcnt_t shift) { Int c (size_after_shift_right (a, shift), CI_SIZE);  return shift_right (c, a, shift); }

#endif
