#include "test.h"
#include "typedef.h"
#include "check.h"
#include "add_low.h"
#include "add.h"
#include "sub_low.h"
#include "sub.h"
#include "mul_low.h"
#include "mul.h"
#include "div_low.h"
#include "div.h"
#include "vec_t.h"
#include "num_t.h"
#include "int.h"



bool test_misc() {
  bool ok = true;

  u8 a=200, b=100;
  swap (a, b);
  check (a==100 && b==200);

  check (max(a, b)==200 && max(b, a)==200);
  check (min(a, b)==100 && min(b, a)==100);

  i16 c=-30000, d=20000, e=0;
  check (abs(c)==30000 && abs(d)==20000 && abs(e)==0);

  check (sign(1000000000000000000llu) == 1);
  check (sign(-1000000000000000000ll) == -1);
  check (sign(0) == 0);

  limb_t z[5], *x;
  x = &z[0];
  check (can_copy_up (z, x, 5));
  --x;
  check (!can_copy_up (z, x, 5));
  --x;
  check (can_copy_up (z, x, 0));
  check (can_copy_up (z, x, 1));
  check (can_copy_up (z, x, 2));
  check (!can_copy_up (z, x, 3));
  x = &z[0];
  ++x;
  check (can_copy_up (z, x, 20));
  check (can_copy_up (z, x, 0));

  check (count_lz ((u8)0) == 8);
  check (count_lz ((u8)1) == 7);
  check (count_lz ((u8)2) == 6);
  check (count_lz ((u8)3) == 6);
  check (count_lz ((u8)0x08) == 4);
  check (count_lz ((u8)0x70) == 1);
  check (count_lz ((u8)0xC0) == 0);

  check (count_lz ((u16)0) == 16);
  check (count_lz ((u16)1) == 15);
  check (count_lz ((u16)2) == 14);
  check (count_lz ((u16)3) == 14);
  check (count_lz ((u16)0x0008) == 12);
  check (count_lz ((u16)0x0070) == 9);
  check (count_lz ((u16)0x00C0) == 8);
  check (count_lz ((u16)0x07C0) == 5);
  check (count_lz ((u16)0x80C0) == 0);

  check (count_lz ((u32)0) == 32);
  check (count_lz ((u32)1) == 31);
  check (count_lz ((u32)2) == 30);
  check (count_lz ((u32)3) == 30);
  check (count_lz ((u32)0x00000008u) == 28);
  check (count_lz ((u32)0x00000070u) == 25);
  check (count_lz ((u32)0x000000C0u) == 24);
  check (count_lz ((u32)0x000007C0u) == 21);
  check (count_lz ((u32)0x000080C0u) == 16);
  check (count_lz ((u32)0x010080C0u) == 7);
  check (count_lz ((u32)0xE10080C0u) == 0);

  check (count_lz ((u64)0) == 64);
  check (count_lz ((u64)1) == 63);
  check (count_lz ((u64)2) == 62);
  check (count_lz ((u64)3) == 62);
  check (count_lz ((u64)0x0000000000000008llu) == 60);
  check (count_lz ((u64)0x0000000000000070llu) == 57);
  check (count_lz ((u64)0x00000000000000C0llu) == 56);
  check (count_lz ((u64)0x00000000000007C0llu) == 53);
  check (count_lz ((u64)0x00000000000080C0llu) == 48);
  check (count_lz ((u64)0x00000000010080C0llu) == 39);
  check (count_lz ((u64)0x00000000E10080C0llu) == 32);
  check (count_lz ((u64)0x00101000E10080C0llu) == 11);
  check (count_lz ((u64)0x40000000E10080C0llu) == 1);
  check (count_lz ((u64)0xB0000000E10080C0llu) == 0);

  check (pos_of_high_one (0x0000000000000000llu) == (bitcnt_t)(-1));
  check (pos_of_high_one (0x0000000000000001llu) == 0);
  check (pos_of_high_one (0x0000400000B00000llu) == 46);
  check (pos_of_high_one (0x800000000A000000llu) == 63);

  check (!is_power_of_2 (0));
  check (is_power_of_2 (1));
  check (is_power_of_2 (2));
  check (is_power_of_2 (4));
  check (!is_power_of_2 (5));
  check (!is_power_of_2 (LIMB_MAX));
  check (is_power_of_2 (HIGH_BIT_MASK));
  check (!is_power_of_2 (HIGH_BIT_MASK|1));

  limb_t y[5];

  set_zero (y, 5);
  check (y[0]==0 && y[1]==0 && y[2]==0 && y[3]==0 && y[4]==0);
  set_value (y, 3, 0xFF);
  check (y[0]==0xFF && y[1]==0xFF && y[2]==0xFF && y[3]==0 && y[4]==0);
  set_value (z, 5, 0x01);
  check (z[0]==0x01 && z[1]==0x01 && z[2]==0x01 && z[3]==0x01 && z[4]==0x01);
  copy_up (z, y, 2);
  check (z[0]==0xFF && z[1]==0xFF && z[2]==0x01 && z[3]==0x01 && z[4]==0x01);
  copy_down (y, z, 5);
  check (y[0]==0xFF && y[1]==0xFF && y[2]==0x01 && y[3]==0x01 && y[4]==0x01);

  check ((glue<u32, u64> (0xABCDEF01u, 0x23456789u)) == 0xABCDEF0123456789llu);
  check ((glue<u8, u16> (0xFE, 0x31)) == 0xFE31);
  check ((glue<u16, u32> (0xFEAB, 0x3109)) == 0xFEAB3109u);

  u8 h8, l8;
  split<u16, u8> (h8, l8, 0x1234);
  check (h8==0x12 && l8==0x34);

  u16 h16, l16;
  split<u32, u16> (h16, l16, 0x456789ABu);
  check (h16==0x4567 && l16==0x89AB);

  u32 h32, l32;
  split<u64, u32> (h32, l32, 0x0123456789ABCDEFllu);
  check (h32==0x01234567u && l32==0x89ABCDEFu);

  l8 = get_lo<u16, u8> (0xABCD);
  h8 = get_hi<u16, u8> (0x0123);
  check (l8==0xCD && h8==0x01);

  check (is_less_or_equal<u8> (0x23, 0x45, 0x23, 0x45));
  check (is_less_or_equal<u8> (0x23, 0x44, 0x23, 0x45));
  check (is_less_or_equal<u8> (0x13, 0x45, 0x23, 0x45));
  check (!is_less_or_equal<u8> (0x23, 0x46, 0x23, 0x45));
  check (!is_less_or_equal<u8> (0x24, 0xFF, 0x23, 0x45));

  check (!is_less<u8> (0x23, 0x45, 0x23, 0x45));
  check (!is_less<u8> (0x25, 0x45, 0x23, 0x45));
  check (!is_less<u8> (0x23, 0x48, 0x23, 0x45));
  check (is_less<u8> (0x23, 0x44, 0x23, 0x45));
  check (is_less<u8> (0x03, 0x90, 0x23, 0x45));

  l8 = 0x10;
  h8 = 0x20;
  add_lo_hi<u8> (h8, l8, 0x00, 0x66);
  check (h8==0x20 && l8==0x76);
  add_lo_hi<u8> (h8, l8, 0x01, 0xFF);
  check (h8==0x22 && l8==0x75);
  add_lo_hi<u8> (h8, l8, 0xFF, 0x80);
  check (h8==0x21 && l8==0xF5);

  sub_lo_hi<u8> (h8, l8, 0x00, 0xA2);
  check (h8==0x21 && l8==0x53);
  sub_lo_hi<u8> (h8, l8, 0x00, 0x86);
  check (h8==0x20 && l8==0xCD);
  sub_lo_hi<u8> (h8, l8, 0xFB, 0x23);
  check (h8==0x25 && l8==0xAA);

  mul_lo_hi<u8> (h8, l8, 0x00, 0x00);
  check (h8==0x00 && l8==0x00);
  mul_lo_hi<u8> (h8, l8, 0x01, 0xFF);
  check (h8==0x00 && l8==0xFF);
  mul_lo_hi<u8> (h8, l8, 0xFF, 0x01);
  check (h8==0x00 && l8==0xFF);
  mul_lo_hi<u8> (h8, l8, 0x02, 0xFF);
  check (h8==0x01 && l8==0xFE);
  mul_lo_hi<u8> (h8, l8, 0xFF, 0xFF);
  check (h8==0xFE && l8==0x01);
  mul_lo_hi<u8> (h8, l8, 0xAB, 0xCD);
  check (h8==0x88 && l8==0xEF);

  set_zero (z, 5);
  check (is_normalized (z, 0));
  check (!is_normalized (z, 1));
  check (normalize_size (z, 0) == 0);
  check (normalize_size (z, 5) == 0);
  z[0] = 1;
  check (is_normalized (z, 1));
  check (!is_normalized (z, 2));
  check (normalize_size (z, 4) == 1);

  check (is_digit ('9'));
  check (is_digit ('0'));
  check (!is_digit ('a'));
  check (!is_digit ('A'));
  check (!is_digit ('@'));
  check (!is_digit ('2', 2));
  check (is_digit ('2', 3));
  check (is_digit ('F', 16));
  check (is_digit ('f', 16));
  check (!is_digit ('g', 16));
  check (is_digit ('z', 36));
  check (!is_digit ('z', 35));

  check (char_by_digit (0) == '0');
  check (char_by_digit (7) == '7');
  check (char_by_digit (9) == '9');
  check (char_by_digit (10) == 'A');
  check (char_by_digit (11) == 'B');
  check (char_by_digit (35) == 'Z');

  check (digit_by_char ('0') == 0);
  check (digit_by_char ('1') == 1);
  check (digit_by_char ('a') == 10);
  check (digit_by_char ('B') == 11);
  check (digit_by_char ('z') == 35);

  for (size_t i=0; i<36; ++i)  check (digit_by_char (char_by_digit (i)) == i);



  return ok;
}



bool test_low_level() {
  bool ok = true;

  assert (LIMB_BITS >= 8);

  const size_t N = 10;
  limb_t *a = new limb_t[N],
         *b = new limb_t[N],
         *c = new limb_t[N];
  size_t size_a;
  limb_t carry, borrow;

  {	// set_zero, copy_up/down, set_value
    for (size_t i=0; i<N; ++i) { a[i]=i; }
    set_zero (b, N);
    set_zero (c, N);
    for (size_t i=0; i<N; ++i) check (a[i]==i && b[i]==0 && c[i]==0);
    copy_up (b, a, N);
    for (size_t i=0; i<N; ++i) check (b[i]==i);
    copy_down (c, b, N);
    for (size_t i=0; i<N; ++i) check (c[i]==i);
    set_value (b, N, LIMB_MAX);
    for (size_t i=0; i<N; ++i) check (b[i]==LIMB_MAX);
    set_value (c, N, 1);
    for (size_t i=0; i<N; ++i) check (c[i]==1);
    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);
    for (size_t i=0; i<N; ++i) check (a[i]==0 && b[i]==0 && c[i]==0);
  }

  {	// Add
    set_zero (a, N);
    set_zero (c, N);
    size_a = 1;

    carry = inc (c, a, size_a, 0);
    check (carry==0 && c[0]==0 && c[1]==0);
    copy_up (a, c, size_a);
    carry = inc (c, a, size_a, 1);
    check (carry==0 && c[0]==1 && c[1]==0);
    copy_up (a, c, size_a);
    carry = inc (c, a, size_a, 2);
    check (carry==0 && c[0]==3 && c[1]==0);
    copy_up (a, c, size_a);
    carry = inc (c, a, size_a, LIMB_MAX);
    check (carry==1 && c[0]==2 && c[1]==0);
    copy_up (a, c, size_a);
    carry = inc (c, a, size_a, LIMB_MAX-1);
    check (carry==1 && c[0]==0 && c[1]==0);
    copy_up (a, c, size_a);

    size_a = 2;

    carry = inc (c, a, size_a, LIMB_MAX);
    check (carry==0 && c[0]==LIMB_MAX && c[1]==0 && c[2]==0);
    copy_down (a, c, size_a);
    carry = inc (c, a, size_a, LIMB_MAX);
    check (carry==0 && c[0]==LIMB_MAX-1 && c[1]==1 && c[2]==0);
    copy_down (a, c, size_a);
    carry = inc (c, a, size_a, LIMB_MAX-1);
    check (carry==0 && c[0]==LIMB_MAX-3 && c[1]==2 && c[2]==0);

    a[0] = LIMB_MAX - 2;
    a[1] = LIMB_MAX;

    carry = inc (c, a, 2, 4);
    check (carry==1 && c[0]==1 && c[1]==0 && c[2]==0);

    carry = inc (a, a, 2, 4);
    check (carry==1 && a[0]==1 && a[1]==0 && a[2]==0);

    carry = inc (a, 1, 10);
    check (carry==0 && a[0]==11 && a[1]==0 && a[2]==0);
    carry = inc (a, 1, LIMB_MAX);
    check (carry==1 && a[0]==10 && a[1]==0 && a[2]==0);
    carry = inc (a, 2, LIMB_MAX);
    check (carry==0 && a[0]==9 && a[1]==1 && a[2]==0);
    carry = inc (a, 2, 1);
    check (carry==0 && a[0]==10 && a[1]==1 && a[2]==0);

    a[0] = LIMB_MAX - 10;
    a[1] = LIMB_MAX;
    size_a = 2;

    carry = inc (a, 2, 12);
    check (carry==1 && a[0]==1 && a[1]==0 && a[2]==0);

    carry = inc (a, 2, 0);
    check (carry==0 && a[0]==1 && a[1]==0 && a[2]==0);

    carry = inc (c, a, 2, 0);
    check (carry==0 && c[0]==1 && c[1]==0 && c[2]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    carry = add (c, a, b, 1);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0);
    a[0] = 1;
    b[0] = 2;
    carry = add (c, a, b, 1);
    check (carry==0 && c[0]==3 && c[1]==0 && c[2]==0);
    carry = add (c, a, b, 2);
    check (carry==0 && c[0]==3 && c[1]==0 && c[2]==0);

    a[0] = LIMB_MAX-2;
    b[0] = 4;
    carry = add (c, a, b, 1);
    check (carry==1 && c[0]==1 && c[1]==0 && c[2]==0);
    carry = add (c, a, b, 2);
    check (carry==0 && c[0]==1 && c[1]==1 && c[2]==0);
    set_value (a, 2, LIMB_MAX);
    set_value (b, 2, LIMB_MAX);
    carry = add (c, a, b, 1);
    check (carry==1 && c[0]==LIMB_MAX-1 && c[1]==1 && c[2]==0);
    carry = add (c, a, b, 2);
    check (carry==1 && c[0]==LIMB_MAX-1 && c[1]==LIMB_MAX && c[2]==0);
    carry = add (c, a, b, 3);
    check (carry==0 && c[0]==LIMB_MAX-1 && c[1]==LIMB_MAX && c[2]==1 && c[3]==0);

    set_value (a, 3, LIMB_MAX);
    set_value (b, 3, 1);
    carry = add (c, a, b, 3);
    check (carry==1 && c[0]==0 && c[1]==1 && c[2]==1 && c[3]==0);
    b[0] = 0;
    carry = add (c, a, b, 3);
    check (carry==1 && c[0]==LIMB_MAX && c[1]==0 && c[2]==1 && c[3]==0);

    carry = add (a, a, b, 3);
    check (carry==1 && a[0]==LIMB_MAX && a[1]==0 && a[2]==1 && a[3]==0);

    carry = add (b, a, b, 3);
    check (carry==0 && b[0]==LIMB_MAX && b[1]==1 && b[2]==2 && b[3]==0);

    carry = add (a, a, a, 3);
    check (carry==0 && a[0]==LIMB_MAX-1 && a[1]==1 && a[2]==2 && a[3]==0);

    set_value (a, 3, LIMB_MAX);

    carry = add (a, a, a, 3);
    check (carry==1 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX && a[2]==LIMB_MAX && a[3]==0);

    set_value (c, N, LIMB_MAX);

    carry = add (c, a, a, 2);
    check (carry==1 && c[0]==LIMB_MAX-3 && c[1]==LIMB_MAX && c[2]==LIMB_MAX && c[3]==LIMB_MAX);

    c[1] = 10;
    c[3] = 0;
    carry = add (c, c, c, 3);
    check (carry==1 && c[0]==LIMB_MAX-7 && c[1]==21 && c[2]==LIMB_MAX-1 && c[3]==0);
  }

  {	// Sub
    set_zero (a, N);
    set_zero (c, N);

    size_a = 1;

    borrow = dec (c, a, size_a, 0);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, 1);
    check (borrow==1 && c[0]==LIMB_MAX && c[1]==0 && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, 1);
    check (borrow==0 && c[0]==LIMB_MAX-1 && c[1]==0 && c[2]==0);
    copy_up (a, c, size_a);

    size_a = 2;

    borrow = dec (c, a, size_a, 0);
    check (borrow==0 && c[0]==LIMB_MAX-1 && c[1]==0 && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, LIMB_MAX);
    check (borrow==1 && c[0]==LIMB_MAX && c[1]==LIMB_MAX && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, LIMB_MAX);
    check (borrow==0 && c[0]==0 && c[1]==LIMB_MAX && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, LIMB_MAX);
    check (borrow==0 && c[0]==1 && c[1]==LIMB_MAX-1 && c[2]==0);
    copy_up (a, c, size_a);
    borrow = dec (c, a, size_a, 1);
    check (borrow==0 && c[0]==0 && c[1]==LIMB_MAX-1 && c[2]==0);
    copy_up (a, c, size_a);

    borrow = dec (a, a, size_a, 1);
    check (borrow==0 && a[0]==LIMB_MAX && a[1]==LIMB_MAX-2 && a[2]==0);

    borrow = dec (a, a, 1, LIMB_MAX-1);
    check (borrow==0 && a[0]==1 && a[1]==LIMB_MAX-2 && a[2]==0);

    borrow = dec (a, a, 1, 2);
    check (borrow==1 && a[0]==LIMB_MAX && a[1]==LIMB_MAX-2 && a[2]==0);

    borrow = dec (a, 1, 2);
    check (borrow==0 && a[0]==LIMB_MAX-2 && a[1]==LIMB_MAX-2 && a[2]==0);
    borrow = dec (a, 1, LIMB_MAX);
    check (borrow==1 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX-2 && a[2]==0);
    borrow = dec (a, 2, LIMB_MAX-2);
    check (borrow==0 && a[0]==1 && a[1]==LIMB_MAX-2 && a[2]==0);
    borrow = dec (a, 2, 2);
    check (borrow==0 && a[0]==LIMB_MAX && a[1]==LIMB_MAX-3 && a[2]==0);
    a[0]=1;
    a[1]=0;
    borrow = dec (a, 2, 2);
    check (borrow==1 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==0);

    a[0] = 1;
    a[1] = 2;
    set_zero (b, N);
    borrow = sub (c, a, b, 2);
    check (borrow==0 && c[0]==1 && c[1]==2 && c[2]==0);
    borrow = sub (c, b, a, 2);
    check (borrow==1 && c[0]==LIMB_MAX && c[1]==LIMB_MAX-2 && c[2]==0);
    copy_up (b, a, 2);
    borrow = sub (c, a, b, 2);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0);
    b[0] = 0;
    borrow = sub (c, a, b, 2);
    check (borrow==0 && c[0]==1 && c[1]==0 && c[2]==0);
    borrow = sub (c, b, a, 2);
    check (borrow==1 && c[0]==LIMB_MAX && c[1]==LIMB_MAX && c[2]==0);

    a[0] = 1;
    a[1] = 3;
    b[0] = 2;
    b[1] = 1;
    borrow = sub (c, a, b, 2);
    check (borrow==0 && c[0]==LIMB_MAX && c[1]==1 && c[2]==0);

    borrow = sub (c, a, a, 1);
    check (borrow==0 && c[0]==0 && c[1]==1 && c[2]==0);
    borrow = sub (c, a, a, 2);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0);

    borrow = sub (a, a, a, 1);
    check (borrow==0 && a[0]==0 && a[1]==3 && a[2]==0);
    borrow = sub (a, a, a, 2);
    check (borrow==0 && a[0]==0 && a[1]==0 && a[2]==0);

    a[0] = 1;
    a[1] = 3;
    b[0] = 2;
    b[1] = 1;

    borrow = sub (a, a, b, 2);
    check (borrow==0 && a[0]==LIMB_MAX && a[1]==1 && a[2]==0);
    borrow = sub (a, a, b, 2);
    check (borrow==0 && a[0]==LIMB_MAX-2 && a[1]==0 && a[2]==0);
    borrow = sub (a, a, b, 2);
    check (borrow==1 && a[0]==LIMB_MAX-4 && a[1]==LIMB_MAX && a[2]==0);

    a[0] = 1;
    a[1] = 3;
    b[0] = 2;
    b[1] = 1;
    borrow = sub (b, a, b, 2);
    check (borrow==0 && b[0]==LIMB_MAX && b[1]==1 && b[2]==0);
    borrow = sub (b, a, b, 2);
    check (borrow==0 && b[0]==2 && b[1]==1 && b[2]==0);
  }

  {	// Mul
    set_zero (a, N);
    set_zero (c, N);

    carry = mul_n_by_1 (c, a, 2, 0);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0);
    carry = mul_n_by_1 (c, a, 2, 0, 12);
    check (carry==0 && c[0]==12 && c[1]==0 && c[2]==0);
    a[0] = LIMB_MAX;
    carry = mul_n_by_1 (c, a, 1, LIMB_MAX);
    check (carry==LIMB_MAX-1 && c[0]==1 && c[1]==0 && c[2]==0);
    carry = mul_n_by_1 (c, a, 2, LIMB_MAX);
    check (carry==0 && c[0]==1 && c[1]==LIMB_MAX-1 && c[2]==0);
    c[1] = 0;
    carry = mul_n_by_1 (c, a, 1, LIMB_MAX, LIMB_MAX);
    check (carry==LIMB_MAX && c[0]==0 && c[1]==0 && c[2]==0);
    carry = mul_n_by_1 (c, a, 2, LIMB_MAX, LIMB_MAX);
    check (carry==0 && c[0]==0 && c[1]==LIMB_MAX && c[2]==0);

    a[0] = 1;
    a[1] = 2;

    carry = mul_n_by_1 (c, a, 2, 4);
    check (carry==0 && c[0]==4 && c[1]==8 && c[2]==0);

    carry = mul_n_by_1 (a, a, 2, 4, 16);
    check (carry==0 && a[0]==20 && a[1]==8 && a[2]==0);

    a[0] = LIMB_MAX;
    a[1] = 0;
    carry = mul_n_by_1 (a, a, 2, LIMB_MAX, LIMB_MAX);
    check (carry==0 && a[0]==0 && a[1]==LIMB_MAX && a[2]==0);
    carry = mul_n_by_1 (a, a, 2, LIMB_MAX, LIMB_MAX-3);
    check (carry==LIMB_MAX-1 && a[0]==LIMB_MAX-3 && a[1]==1 && a[2]==0);

    carry = mul_n_by_1 (c, a, 2, 0);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0);

    carry = mul_n_by_1 (c, a, 2, 0, 5);
    check (carry==0 && c[0]==5 && c[1]==0 && c[2]==0);

    a[1] = LIMB_MAX;
    carry = mul_n_by_1 (a, a, 2, 0, 0);
    check (carry==0 && a[0]==0 && a[1]==0 && a[2]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    carry = addmul_n_by_1 (c, a, b, 3, 0);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    carry = addmul_n_by_1 (c, a, b, 3, 1);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    carry = addmul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (carry==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    b[0] = 3;
    carry = addmul_n_by_1 (c, a, b, 1, LIMB_MAX);
    check (carry==2 && c[0]==LIMB_MAX-2 && c[1]==0 && c[2]==0 && c[3]==0);
    carry = addmul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (carry==0 && c[0]==LIMB_MAX-2 && c[1]==2 && c[2]==0 && c[3]==0);
    a[0] = 5;
    a[1] = 7;
    carry = addmul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (carry==0 && c[0]==2 && c[1]==10 && c[2]==0 && c[3]==0);
    b[2] = 4;
    carry = addmul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (carry==3 && c[0]==2 && c[1]==10 && c[2]==LIMB_MAX-3 && c[3]==0);

    carry = addmul_n_by_1 (c, a, a, 2, 7);
    check (carry==0 && c[0]==40 && c[1]==56 && c[2]==LIMB_MAX-3 && c[3]==0);

    carry = addmul_n_by_1 (a, a, a, 2, 8);
    check (carry==0 && a[0]==45 && a[1]==63 && a[2]==0 && c[3]==0);

    b[0] = 1;
    b[1] = 2;
    b[2] = LIMB_MAX-7;

    carry = addmul_n_by_1 (a, a, b, 3, 8);
    check (carry==7 && a[0]==53 && a[1]==79 && a[2]==LIMB_MAX-63 && a[3]==0);

    b[2] = 0;
    carry = addmul_n_by_1 (a, b, a, 4, 2);
    check (carry==0 && a[0]==107 && a[1]==160 && a[2]==(limb_t)(LIMB_MAX-127) && a[3]==1);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    borrow = submul_n_by_1 (c, a, b, 3, 0);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    borrow = submul_n_by_1 (c, a, b, 3, 1);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    borrow = submul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (borrow==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);

    a[1] = 10;
    borrow = submul_n_by_1 (c, a, b, 3, LIMB_MAX);
    check (borrow==0 && c[0]==0 && c[1]==10 && c[2]==0 && c[3]==0);
    b[0] = 1;
    b[1] = 4;

    borrow = submul_n_by_1 (c, a, b, 1, 2);
    check (borrow==1 && c[0]==LIMB_MAX-1 && c[1]==10 && c[2]==0 && c[3]==0);
    borrow = submul_n_by_1 (c, a, b, 3, 2);
    check (borrow==0 && c[0]==LIMB_MAX-1 && c[1]==1 && c[2]==0 && c[3]==0);

    a[1] = 2;

    borrow = submul_n_by_1 (c, a, b, 2, LIMB_MAX);
    check (borrow==4 && c[0]==1 && c[1]==5 && c[2]==0 && c[3]==0);

    borrow = submul_n_by_1 (a, a, b, 2, LIMB_MAX);
    check (borrow==4 && a[0]==1 && a[1]==5 && a[2]==0 && a[3]==0);

    borrow = submul_n_by_1 (b, a, b, 2, 10);
    check (borrow==1 && b[0]==LIMB_MAX-8 && b[1]==LIMB_MAX-35 && b[2]==0 && b[3]==0);

    borrow = submul_n_by_1 (b, b, b, 3, 2);
    check (borrow==1 && b[0]==9 && b[1]==35 && b[2]==LIMB_MAX && b[3]==0);

    borrow = submul_n_by_1 (b, b, b, 2, 1);
    check (borrow==0 && b[0]==0 && b[1]==0 && b[2]==LIMB_MAX && b[3]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);
    a[0] = 1;
    b[0] = 1;
    carry = mul_n_by_m (c, a, 1, b, 1);
    check (carry==0 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0 && c[4]==0 && c[5]==0);
    a[1] = 2;
    carry = mul_n_by_m (c, a, 2, b, 1);
    check (carry==0 && c[0]==1 && c[1]==2 && c[2]==0 && c[3]==0 && c[4]==0 && c[5]==0);
    b[1] = 3;
    carry = mul_n_by_m (c, a, 2, b, 2);
    check (carry==0 && c[0]==1 && c[1]==5 && c[2]==6 && c[3]==0 && c[4]==0 && c[5]==0);
    b[2] = 4;
    carry = mul_n_by_m (c, a, 2, b, 3);
    check (carry==0 && c[0]==1 && c[1]==5 && c[2]==10 && c[3]==8 && c[4]==0 && c[5]==0);

    set_zero (c, N);
    a[0] = LIMB_MAX;
    a[1] = LIMB_MAX;
    a[2] = LIMB_MAX;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX;
    carry = mul_n_by_m (c, a, 1, b, 1);
    check (carry==LIMB_MAX-1 && c[0]==1 && c[1]==LIMB_MAX-1 && c[2]==0 && c[3]==0 && c[4]==0 && c[5]==0);
    carry = mul_n_by_m (c, a, 1, b, 2);
    check (carry==LIMB_MAX-1 && c[0]==1 && c[1]==LIMB_MAX && c[2]==LIMB_MAX-1 && c[3]==0 && c[4]==0 && c[5]==0);
    carry = mul_n_by_m (c, a, 2, b, 2);
    check (carry==LIMB_MAX && c[0]==1 && c[1]==0 && c[2]==LIMB_MAX-1 && c[3]==LIMB_MAX && c[4]==0 && c[5]==0);
    carry = mul_n_by_m (c, a, 3, b, 2);
    check (carry==LIMB_MAX && c[0]==1 && c[1]==0 && c[2]==LIMB_MAX && c[3]==LIMB_MAX-1 && c[4]==LIMB_MAX && c[5]==0);
  }

  {	// Div
    limb_t q, r, r0, r1;

    r = div_2_by_1 (q, 0, 0, HIGH_BIT_MASK|1);
    check (q==0 && r==0);
    r = div_2_by_1 (q, 0, 0, LIMB_MAX);
    check (q==0 && r==0);
    r = div_2_by_1 (q, 0, 1, HIGH_BIT_MASK);
    check (q==0 && r==1);
    r = div_2_by_1 (q, 0, 127, HIGH_BIT_MASK);
    check (q==0 && r==127);
    r = div_2_by_1 (q, 1, 127, HIGH_BIT_MASK);
    check (q==2 && r==127);
    r = div_2_by_1 (q, 0, LIMB_MAX, HIGH_BIT_MASK);
    check (q==1 && r==LIMB_MAX/2);
    r = div_2_by_1 (q, 1, 0, LIMB_MAX);
    check (q==1 && r==1);
    r = div_2_by_1 (q, LIMB_MAX-1, 0, LIMB_MAX);
    check (q==LIMB_MAX-1 && r==LIMB_MAX-1);
    r = div_2_by_1 (q, LIMB_MAX-1, LIMB_MAX, LIMB_MAX);
    check (q==LIMB_MAX && r==LIMB_MAX-1);
    r = div_2_by_1 (q, LIMB_MAX/2, LIMB_MAX, HIGH_BIT_MASK);
    check (q==LIMB_MAX && r==LIMB_MAX/2);
    r = div_2_by_1 (q, LIMB_MAX/2, LIMB_MAX, HIGH_BIT_MASK|1);
    check (q==LIMB_MAX-1 && r==1);
    r = div_2_by_1 (q, LIMB_MAX/8, LIMB_MAX-1, HIGH_BIT_MASK|7);
    check (q==LIMB_MAX/4-3 && r==HIGH_BIT_MASK/2+26);

    div_3_by_2 (q, r1, r0, 0, 0, 0, HIGH_BIT_MASK, 0);
    check (q==0 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, 0, 0, LIMB_MAX, LIMB_MAX);
    check (q==0 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, 0, LIMB_MAX/10-9, LIMB_MAX, LIMB_MAX);
    check (q==0 && r1==0 && r0==LIMB_MAX/10-9);
    div_3_by_2 (q, r1, r0, 0, 115, LIMB_MAX/11, LIMB_MAX, LIMB_MAX/128);
    check (q==0 && r1==115 && r0==LIMB_MAX/11);
    div_3_by_2 (q, r1, r0, 0, HIGH_BIT_MASK, LIMB_MAX, HIGH_BIT_MASK, LIMB_MAX);
    check (q==1 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, HIGH_BIT_MASK|(LIMB_MAX>>7), 200, LIMB_MAX, 201);
    check (q==0 && r1==(HIGH_BIT_MASK|(LIMB_MAX>>7)) && r0==200);
    div_3_by_2 (q, r1, r0, HIGH_BIT_MASK-1, HIGH_BIT_MASK, HIGH_BIT_MASK, HIGH_BIT_MASK, HIGH_BIT_MASK);
    check (q==LIMB_MAX-1 && r1==1 && r0==HIGH_BIT_MASK);
    div_3_by_2 (q, r1, r0, LIMB_MAX-1, LIMB_MAX, LIMB_MAX, LIMB_MAX, LIMB_MAX);
    check (q==LIMB_MAX && r1==0 && r0==LIMB_MAX-1);

    limb_t *Q = new limb_t[N];

#ifdef DIV_PREINVERSION

    check (inv_2_by_1 (HIGH_BIT_MASK) == LIMB_MAX);
    check (inv_2_by_1 (LIMB_MAX) == 1);

    r = div_2_by_1 (q, 0, 0, HIGH_BIT_MASK|1, inv_2_by_1 (HIGH_BIT_MASK|1));
    check (q==0 && r==0);
    r = div_2_by_1 (q, 0, 0, LIMB_MAX, inv_2_by_1 (LIMB_MAX));
    check (q==0 && r==0);
    r = div_2_by_1 (q, 0, 1, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (q==0 && r==1);
    r = div_2_by_1 (q, 0, 127, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (q==0 && r==127);
    r = div_2_by_1 (q, 1, 127, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (q==2 && r==127);
    r = div_2_by_1 (q, 0, LIMB_MAX, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (q==1 && r==LIMB_MAX/2);
    r = div_2_by_1 (q, 1, 0, LIMB_MAX, inv_2_by_1 (LIMB_MAX));
    check (q==1 && r==1);
    r = div_2_by_1 (q, LIMB_MAX-1, 0, LIMB_MAX, inv_2_by_1 (LIMB_MAX));
    check (q==LIMB_MAX-1 && r==LIMB_MAX-1);
    r = div_2_by_1 (q, LIMB_MAX-1, LIMB_MAX, LIMB_MAX, inv_2_by_1 (LIMB_MAX));
    check (q==LIMB_MAX && r==LIMB_MAX-1);
    r = div_2_by_1 (q, LIMB_MAX/2, LIMB_MAX, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (q==LIMB_MAX && r==LIMB_MAX/2);
    r = div_2_by_1 (q, LIMB_MAX/2, LIMB_MAX, HIGH_BIT_MASK|1, inv_2_by_1 (HIGH_BIT_MASK|1));
    check (q==LIMB_MAX-1 && r==1);
    r = div_2_by_1 (q, LIMB_MAX/8, LIMB_MAX-1, HIGH_BIT_MASK|7, inv_2_by_1 (HIGH_BIT_MASK|7));
    check (q==LIMB_MAX/4-3 && r==HIGH_BIT_MASK/2+26);

    check (inv_3_by_2 (HIGH_BIT_MASK, 0) == LIMB_MAX);
    check (inv_3_by_2 (LIMB_MAX, LIMB_MAX) == 0);

    div_3_by_2 (q, r1, r0, 0, 0, 0, HIGH_BIT_MASK, 0, inv_3_by_2 (HIGH_BIT_MASK, 0));
    check (q==0 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, 0, 0, LIMB_MAX, LIMB_MAX, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (q==0 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, 0, LIMB_MAX/10-9, LIMB_MAX, LIMB_MAX, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (q==0 && r1==0 && r0==LIMB_MAX/10-9);
    div_3_by_2 (q, r1, r0, 0, 115, LIMB_MAX/11, LIMB_MAX, LIMB_MAX/128, inv_3_by_2 (LIMB_MAX, LIMB_MAX/128));
    check (q==0 && r1==115 && r0==LIMB_MAX/11);
    div_3_by_2 (q, r1, r0, 0, HIGH_BIT_MASK, LIMB_MAX, HIGH_BIT_MASK, LIMB_MAX, inv_3_by_2 (HIGH_BIT_MASK, LIMB_MAX));
    check (q==1 && r1==0 && r0==0);
    div_3_by_2 (q, r1, r0, 0, HIGH_BIT_MASK|(LIMB_MAX>>7), 200, LIMB_MAX, 201, inv_3_by_2 (LIMB_MAX, 201));
    check (q==0 && r1==(HIGH_BIT_MASK|(LIMB_MAX>>7)) && r0==200);
    div_3_by_2 (q, r1, r0, HIGH_BIT_MASK-1, HIGH_BIT_MASK, HIGH_BIT_MASK, HIGH_BIT_MASK, HIGH_BIT_MASK, inv_3_by_2 (HIGH_BIT_MASK, HIGH_BIT_MASK));
    check (q==LIMB_MAX-1 && r1==1 && r0==HIGH_BIT_MASK);
    div_3_by_2 (q, r1, r0, LIMB_MAX-1, LIMB_MAX, LIMB_MAX, LIMB_MAX, LIMB_MAX, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (q==LIMB_MAX && r1==0 && r0==LIMB_MAX-1);

    set_zero (a, N);
    set_zero (Q, N);

    a[0] = 1;
    r = div_n_by_1 (Q, 0, a, 1, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (r==1 && Q[0]==0 && Q[1]==0 && Q[2]==0 && Q[3]==0);
    check (r == mod_n_by_1 (0, a, 1, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK)));
    a[0] = HIGH_BIT_MASK;
    r = div_n_by_1 (Q, 0, a, 1, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (r==0 && Q[0]==1 && Q[1]==0 && Q[2]==0 && Q[3]==0);
    check (r == mod_n_by_1 (0, a, 1, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK)));
    a[0] |= 63;
    a[1] = LIMB_MAX-3;
    a[2] = LIMB_MAX;
    r = div_n_by_1 (Q, 0, a, 3, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (r==63 && Q[0]==LIMB_MAX-6 && Q[1]==LIMB_MAX && Q[2]==1 && Q[3]==0);
    check (r == mod_n_by_1 (0, a, 3, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK)));
    r = div_n_by_1 (Q, 30, a, 3, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (r==63 && Q[0]==LIMB_MAX-6 && Q[1]==LIMB_MAX && Q[2]==61 && Q[3]==0);
    check (r == mod_n_by_1 (30, a, 3, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK)));
    r = div_n_by_1 (a, 30, a, 3, HIGH_BIT_MASK, inv_2_by_1 (HIGH_BIT_MASK));
    check (r==63 && a[0]==LIMB_MAX-6 && a[1]==LIMB_MAX && a[2]==61 && a[3]==0);
    a[0] = a[1] = a[2] = LIMB_MAX;
    r = div_n_by_1 (a, 1, a, 3, LIMB_MAX, inv_2_by_1 (LIMB_MAX));
    check (r==1 && a[0]==2 && a[1]==2 && a[2]==2 && a[3]==0);
    a[0] = a[1] = a[2] = LIMB_MAX;
    r = div_n_by_1 (a, 1, a, 3, LIMB_MAX-1, inv_2_by_1 (LIMB_MAX-1));
    check (r==15 && a[0]==8 && a[1]==4 && a[2]==2 && a[3]==0);
    a[0] = a[1] = a[2] = LIMB_MAX;
    r = mod_n_by_1 (1, a, 3, LIMB_MAX-1, inv_2_by_1 (LIMB_MAX-1));
    check (r==15);

    set_zero (a, N);
    set_zero (Q, N);

    a[0] = a[1] = 1;
    limb_t R[2];
    div_n_by_2 (Q, R, 0, a, 2, HIGH_BIT_MASK, 0, inv_3_by_2 (HIGH_BIT_MASK, 0));
    check (R[0]==1 && R[1]==1 && Q[0]==0 && Q[1]==0 && Q[2]==0 && Q[3]==0);
    a[1] = HIGH_BIT_MASK;
    div_n_by_2 (Q, R, 0, a, 2, HIGH_BIT_MASK, 0, inv_3_by_2 (HIGH_BIT_MASK, 0));
    check (R[0]==1 && R[1]==0 && Q[0]==1 && Q[1]==0 && Q[2]==0 && Q[3]==0);
    a[0] = a[1] = a[2] = a[3] = HIGH_BIT_MASK;
    div_n_by_2 (Q, R, 0, a, 4, HIGH_BIT_MASK, HIGH_BIT_MASK, inv_3_by_2 (HIGH_BIT_MASK, HIGH_BIT_MASK));
    check (R[0]==0 && R[1]==0 && Q[0]==1 && Q[1]==0 && Q[2]==1 && Q[3]==0);
    mod_n_by_2 (r1, r0, 1, a, 4, HIGH_BIT_MASK, HIGH_BIT_MASK, inv_3_by_2 (HIGH_BIT_MASK, HIGH_BIT_MASK));
    check (r1==LIMB_MAX/2 && r0==HIGH_BIT_MASK);
    mod_n_by_2 (r1, r0, 1, a, 4, HIGH_BIT_MASK, 0, inv_3_by_2 (HIGH_BIT_MASK, 0));
    check (r1==0 && r0==HIGH_BIT_MASK);

    set_zero (a, N);
    set_zero (b, N);
    set_value(Q, N, LIMB_MAX);

    b[2] = HIGH_BIT_MASK;
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = 3;
    div_m_by_n (Q, a, size_a, b, 3, inv_3_by_2 (HIGH_BIT_MASK, 0));
    check (Q[0]==1 && Q[1]==LIMB_MAX && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==LIMB_MAX/2 && a[3]==0 && a[4]==0);
    a[0] = a[1] = a[2] = LIMB_MAX;
    b[0] = b[1] = b[2] = LIMB_MAX;
    size_a = 3;
    div_m_by_n (Q, a, size_a, b, 3, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (Q[0]==1 && Q[1]==LIMB_MAX && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==0 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX;
    b[0] = 1;
    b[1] = LIMB_MAX-1;
    b[2] = LIMB_MAX;
    size_a = 4;
    div_m_by_n (Q, a, size_a, b, 3, inv_3_by_2 (LIMB_MAX, LIMB_MAX-1));
    check (Q[0]==0 && Q[1]==1 && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX-1 && a[2]==1 && a[3]==0 && a[4]==0);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX;
    b[0] = b[1] = b[2] = b[3] = LIMB_MAX;
    size_a = 4;
    div_m_by_n (Q, a, size_a, b, 4, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (Q[0]==1 && Q[1]==1 && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==0 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX;
    b[0] = b[1] = b[2] = b[3] = LIMB_MAX;
    --a[0];
    size_a = 4;
    div_m_by_n (Q, a, size_a, b, 4, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (Q[0]==0 && Q[1]==1 && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==4 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX && a[2]==LIMB_MAX && a[3]==LIMB_MAX && a[4]==0);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX;
    b[0] = b[1] = b[2] = b[3] = LIMB_MAX;
    --b[0];
    size_a = 4;
    div_m_by_n (Q, a, size_a, b, 4, inv_3_by_2 (LIMB_MAX, LIMB_MAX));
    check (Q[0]==1 && Q[1]==1 && Q[2]==LIMB_MAX && Q[3]==LIMB_MAX);
    check (size_a==1 && a[0]==1 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==0);

#endif

    delete[] Q;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return ok;
}

bool test_high_level() {
  bool ok = true;

  const size_t N = 10;
  limb_t *a = new limb_t[N],
         *b = new limb_t[N],
         *c = new limb_t[N];
  size_t size_a, size_b, size_c;

  {	// Add
    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);
    size_a = size_b = size_c = 0;

    size_c = add (c, a, size_a, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = add (c, a, size_a, 1);
    check (size_c==1 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = add (c, a, size_a, LIMB_MAX);
    check (size_c==1 && c[0]==LIMB_MAX && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = LIMB_MAX-1;
    size_c = add (c, a, 1, LIMB_MAX);
    check (size_c==2 && c[0]==LIMB_MAX-2 && c[1]==1 && c[2]==0 && c[3]==0);
    a[0] = 3;
    a[1] = LIMB_MAX;
    size_a = add (a, a, 2, LIMB_MAX);
    check (size_a==3 && a[0]==2 && a[1]==0 && a[2]==1 && a[3]==0);
    size_a = add (a, a, 3, 10);
    check (size_a==3 && a[0]==12 && a[1]==0 && a[2]==1 && a[3]==0);
    size_a = add (a, a, 3, 0);
    check (size_a==3 && a[0]==12 && a[1]==0 && a[2]==1 && a[3]==0);

    size_a = add (a, 3, LIMB_MAX-11);
    check (size_a==3 && a[0]==0 && a[1]==1 && a[2]==1 && a[3]==0);
    a[0] = 2;
    a[1] = a[2] = LIMB_MAX;
    size_a = add (a, 3, LIMB_MAX);
    check (size_a==4 && a[0]==1 && a[1]==0 && a[2]==0 && a[3]==1);
    size_a = add (a, size_a, 0);
    check (size_a==4 && a[0]==1 && a[1]==0 && a[2]==0 && a[3]==1);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);
    a[0] = 1;
    a[1] = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX;
    size_a = size_b = 2;
    size_c = add (c, a, size_a, b, size_b);
    check (size_c==3 && c[0]==0 && c[1]==2 && c[2]==1);
    size_a = add (a, c, size_c, b, size_b);
    check (size_a==3 && a[0]==LIMB_MAX && a[1]==1 && a[2]==2);
    size_c = add (c, b, size_b, a, size_a);
    check (size_c==3 && c[0]==LIMB_MAX-1 && c[1]==1 && c[2]==3);

    size_c = add (c, size_c, b, size_b);
    check (size_c==3 && c[0]==LIMB_MAX-2 && c[1]==1 && c[2]==4);
    size_b = add (b, size_b, c, size_c);
    check (size_b==3 && b[0]==LIMB_MAX-3 && b[1]==1 && b[2]==5);
    size_b = add (b, size_b, b, size_b);
    check (size_b==3 && b[0]==LIMB_MAX-7 && b[1]==3 && b[2]==10);
    size_b = add (b, b, size_b, b, size_b);
    check (size_b==3 && b[0]==LIMB_MAX-15 && b[1]==7 && b[2]==20);
    a[0] = 10;
    size_a = add (b, a, 1, b, size_b);
    check (size_a==3 && b[0]==LIMB_MAX-5 && b[1]==7 && b[2]==20);
    size_a = add (b, b, size_b, a, 1);
    check (size_a==3 && b[0]==4 && b[1]==8 && b[2]==20);
  }

  {	// Sub
    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    size_c = sub (c, a, 0, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = 12;
    size_c = sub (c, a, 0, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = sub (c, a, 1, 0);
    check (size_c==1 && c[0]==12 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = 1;
    a[1] = 1;
    size_c = sub (c, a, 2, 2);
    check (size_c==1 && c[0]==LIMB_MAX && c[1]==0 && c[2]==0 && c[3]==0);
    size_a = sub (a, c, 1, LIMB_MAX);
    check (size_a==0 && a[0]==0 && a[1]==1 && a[2]==0 && a[3]==0);
    a[0] = LIMB_MAX;
    size_a = sub (a, 1, LIMB_MAX-1);
    check (size_a==1 && a[0]==1 && a[1]==1 && a[2]==0 && a[3]==0);
    size_a = sub (a, 1, 1);
    check (size_a==0 && a[0]==0 && a[1]==1 && a[2]==0 && a[3]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    a[0] = 4;
    a[1] = 1;
    b[0] = LIMB_MAX;
    size_c = sub (c, a, 2, b, 1);
    check (size_c==1 && c[0]==5 && c[1]==0 && c[2]==0);
    b[1] = 2;
    size_c = sub (c, b, 2, a, 2);
    check (size_c==2 && c[0]==LIMB_MAX-4 && c[1]==1 && c[2]==0);
    size_b = sub (b, b, 2, a, 2);
    check (size_b==2 && b[0]==LIMB_MAX-4 && b[1]==1 && b[2]==0);
    size_b = sub (b, b, 2, b, 2);
    check (size_b==0 && b[0]==0 && b[1]==0 && b[2]==0);
    b[3] = 1;
    size_a = sub (a, b, 4, a, 2);
    check (size_a==3 && a[0]==LIMB_MAX-3 && a[1]==LIMB_MAX-1 && a[2]==LIMB_MAX && a[3]==0);
    b[0] = 0;
    b[1] = LIMB_MAX;
    size_a = sub (a, size_a, b, 2);
    check (size_a==3 && a[0]==LIMB_MAX-3 && a[1]==LIMB_MAX && a[2]==LIMB_MAX-1 && a[3]==0);
    size_a = sub (a, size_a, a, size_a);
    check (size_a==0 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0);
  }

  {	// Mul
    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    size_c = mul (c, a, 0, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = mul (c, a, 0, LIMB_MAX, 1);
    check (size_c==1 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = 1;
    size_c = mul (c, a, 1, LIMB_MAX);
    check (size_c==1 && c[0]==LIMB_MAX && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = mul (c, a, 1, LIMB_MAX, 3);
    check (size_c==2 && c[0]==2 && c[1]==1 && c[2]==0 && c[3]==0);
    size_a = mul (a, a, 1, LIMB_MAX, LIMB_MAX);
    check (size_a==2 && a[0]==LIMB_MAX-1 && a[1]==1 && a[2]==0 && a[3]==0);
    size_a = mul (a, a, size_a, LIMB_MAX, 1);
    check (size_a==3 && a[0]==3 && a[1]==LIMB_MAX-3 && a[2]==1 && a[3]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    size_c = addmul (c, a, 0, b, 0, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = addmul (c, a, 0, b, 0, LIMB_MAX);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = 1;
    size_c = addmul (c, a, 1, b, 0, LIMB_MAX);
    check (size_c==1 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0);
    b[0] = 1;
    size_c = addmul (c, a, 1, b, 1, 0);
    check (size_c==1 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = addmul (c, a, 1, b, 1, 1);
    check (size_c==1 && c[0]==2 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = addmul (c, a, 1, b, 1, LIMB_MAX);
    check (size_c==2 && c[0]==0 && c[1]==1 && c[2]==0 && c[3]==0);
    a[1] = 2;
    size_c = addmul (c, a, 2, b, 1, LIMB_MAX);
    check (size_c==2 && c[0]==0 && c[1]==3 && c[2]==0 && c[3]==0);
    b[1] = 2;
    b[2] = 3;
    size_c = addmul (c, a, 2, b, 3, 5);
    check (size_c==3 && c[0]==6 && c[1]==12 && c[2]==15 && c[3]==0);
    size_a = addmul (a, a, 2, b, 3, 5);
    check (size_a==3 && a[0]==6 && a[1]==12 && a[2]==15 && a[3]==0);
    size_a = addmul (a, b, 3, a, 3, 2);
    check (size_a==3 && a[0]==13 && a[1]==26 && a[2]==33 && a[3]==0);
    size_a = addmul (a, a, 3, a, 3, 3);
    check (size_a==3 && a[0]==52 && a[1]==104 && a[2]==132 && a[3]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    size_c = submul (c, a, 0, b, 0, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    size_c = submul (c, a, 0, b, 0, LIMB_MAX);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = 1;
    size_c = submul (c, a, 1, b, 0, 0);
    check (size_c==1 && c[0]==1 && c[1]==0 && c[2]==0 && c[3]==0);
    a[1] = 2;
    size_c = submul (c, a, 2, b, 0, 0);
    check (size_c==2 && c[0]==1 && c[1]==2 && c[2]==0 && c[3]==0);
    b[0] = 1;
    size_c = submul (c, a, 2, b, 1, 0);
    check (size_c==2 && c[0]==1 && c[1]==2 && c[2]==0 && c[3]==0);
    size_c = submul (c, a, 2, b, 0, LIMB_MAX);
    check (size_c==2 && c[0]==1 && c[1]==2 && c[2]==0 && c[3]==0);
    size_c = submul (c, a, 2, b, 1, LIMB_MAX);
    check (size_c==2 && c[0]==2 && c[1]==1 && c[2]==0 && c[3]==0);
    b[1] = 2;
    size_c = submul (c, a, 2, b, 2, 1);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    a[0] = a[1] = 0;
    a[2] = 1;
    size_c = submul (c, a, 3, b, 1, LIMB_MAX);
    check (size_c==2 && c[0]==1 && c[1]==LIMB_MAX && c[2]==0 && c[3]==0);
    size_a = submul (a, a, 3, b, 1, LIMB_MAX);
    check (size_a==2 && a[0]==1 && a[1]==LIMB_MAX && a[2]==0 && a[3]==0);
    size_a = submul (a, b, 2, a, 2, 0);
    check (size_a==2 && a[0]==1 && a[1]==2 && a[2]==0 && a[3]==0);
    size_a = submul (a, a, 2, a, 2, 1);
    check (size_a==0 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0);

    set_zero (a, N);
    set_zero (b, N);
    set_zero (c, N);

    size_c = mul (c, a, 0, b, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    a[1] = 2;
    size_c = mul (c, a, 2, b, 0);
    check (size_c==0 && c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0);
    b[0] = 0;
    b[1] = 1;
    size_c = mul (c, a, 2, b, 2);
    check (size_c==3 && c[0]==0 && c[1]==0 && c[2]==2 && c[3]==0);
    a[0] = a[1] = LIMB_MAX;
    b[0] = b[1] = LIMB_MAX;
    size_c = mul (c, a, 2, b, 2);
    check (size_c==4 && c[0]==1 && c[1]==0 && c[2]==LIMB_MAX-1 && c[3]==LIMB_MAX);
  }

  {	// Div
    limb_t *q = new limb_t[N], r, R[2];

    set_zero (a, N);
    set_zero (q, N);

    size_a = 3;
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = div_qr (q, &r, a, size_a, HIGH_BIT_MASK);
    check (size_a==3 && r==LIMB_MAX/2 && q[0]==LIMB_MAX && q[1]==LIMB_MAX && q[2]==1);
    size_a = div_qr (q, &r, a, size_a, LIMB_MAX-1);
    check (size_a==3 && r==7 && q[0]==4 && q[1]==2 && q[2]==1);
    size_a = div_qr (q, nullptr, a, size_a, LIMB_MAX-1);
    check (size_a==3 && q[0]==4 && q[1]==2 && q[2]==1);
    size_a = div_qr (nullptr, &r, a, size_a, LIMB_MAX-1);
    check (size_a==3 && r==7);
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = div_qr (a, &r, a, size_a, HIGH_BIT_MASK);
    check (size_a==3 && r==LIMB_MAX/2 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==1);
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = div_qr (q, a, a, size_a, HIGH_BIT_MASK);
    check (size_a==3 && a[0]==LIMB_MAX/2 && q[0]==LIMB_MAX && q[1]==LIMB_MAX && q[2]==1);
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = div_qr (q, a, a, size_a, LIMB_MAX-1);
    check (size_a==3 && a[0]==7 && q[0]==4 && q[1]==2 && q[2]==1);
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = div_qr (a, &r, a, size_a, LIMB_MAX-1);
    check (size_a==3 && r==7 && a[0]==4 && a[1]==2 && a[2]==1);

    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    size_a = 3;
    size_a = div_qr (nullptr, &r, a, size_a, 2);
    check (size_a==3 && r==0);
    size_a = div_qr (nullptr, &r, a, size_a, 16);
    check (size_a==3 && r==10);
    size_a = div_qr (nullptr, &r, a, size_a, 32);
    check (size_a==2 && r==10);
    size_a = div_qr (q, &r, a, 3, 4);
    check (size_a==3 && r==2 && q[0]==2 && q[1]==(HIGH_BIT_MASK|5) && q[2]==7);
    a[0] += 4;
    size_a = div_qr (q, &r, a, 3, 5);
    check (size_a==3 && r==4 && q[0]==2 && q[1]==4 && q[2]==6);
    a[0] = LIMB_MAX;
    a[1] = HIGH_BIT_MASK;
    size_a = div_qr (q, &r, a, 2, LIMB_MAX);
    check (size_a==1 && r==HIGH_BIT_MASK && q[0]==HIGH_BIT_MASK+1);

    set_zero (a, N);
    set_zero (q, N);

    a[0] = a[1] = LIMB_MAX;
    size_a = div_qr (q, R, a, 2, 1, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==0 && q[0]==LIMB_MAX && q[1]==0 && q[2]==0);
    size_a = div_qr (nullptr, R, a, 2, 1, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==0);
    size_a = div_qr (q, R, a, 2, 4, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==3 && q[0]==LIMB_MAX/4 && q[1]==0 && q[2]==0);
    size_a = div_qr (q, nullptr, a, 2, 4, 0);
    check (size_a==1 && q[0]==LIMB_MAX/4 && q[1]==0 && q[2]==0);
    size_a = div_qr (q, R, a, 2, HIGH_BIT_MASK, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==LIMB_MAX/2 && q[0]==1 && q[1]==0 && q[2]==0);
    size_a = div_qr (nullptr, R, a, 2, HIGH_BIT_MASK, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==LIMB_MAX/2);
    size_a = div_qr (q, R, a, 2, LIMB_MAX, LIMB_MAX);
    check (size_a==1 && R[0]==0 && R[1]==0 && q[0]==1 && q[1]==0 && q[2]==0);

    size_a = div_qr (a, R, a, 2, HIGH_BIT_MASK, 0);
    check (size_a==1 && R[0]==LIMB_MAX && R[1]==LIMB_MAX/2 && a[0]==1 && q[1]==0 && q[2]==0);
    a[0] = a[1] = LIMB_MAX;
    size_a = div_qr (q, a, a, 2, HIGH_BIT_MASK, 0);
    check (size_a==1 && a[0]==LIMB_MAX && a[1]==LIMB_MAX/2 && q[0]==1 && q[1]==0 && q[2]==0);
    a[0] = a[1] = LIMB_MAX;
    a[2] = 3;
    size_a = div_qr (q, a, a, 3, LIMB_MAX, LIMB_MAX);
    check (size_a==1 && a[0]==3 && a[1]==0 && q[0]==4 && q[1]==0 && q[2]==0);
    a[0] = a[1] = LIMB_MAX;
    a[2] = 3;

    set_zero (a, N);
    set_zero (b, N);
    set_zero (q, N);

    b[2] = 1;
    a[0] = a[1] = a[2] = LIMB_MAX-1;
    size_a = 3;
    size_c = div_qr (q, a, size_a, b, 3);
    check (size_c==1 && size_a==2 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX-1 && a[2]==0 && q[0]==LIMB_MAX-1);

    b[2] = 1;
    a[0] = a[1] = a[2] = LIMB_MAX-1;
    size_a = 3;
    size_c = div_qr (nullptr, a, size_a, b, 3);
    check (size_c==0 && size_a==2 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX-1 && a[2]==0);

    b[2] = 4;
    a[0] = a[1] = a[2] = LIMB_MAX-1;
    size_a = 3;
    size_c = div_qr (q, a, size_a, b, 3);
    check (size_c==1 && size_a==3 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX-1 && a[2]==2 && q[0]==LIMB_MAX/4);

    b[2] = 4;
    a[0] = a[1] = a[2] = LIMB_MAX-1;
    size_a = 3;
    size_c = div_qr (nullptr, a, size_a, b, 3);
    check (size_c==0 && size_a==3 && a[0]==LIMB_MAX-1 && a[1]==LIMB_MAX-1 && a[2]==2);

    b[0] = b[1] = b[2] = LIMB_MAX;
    a[0] = a[1] = a[2] = a[3] = LIMB_MAX-3;
    size_a = 4;
    size_c = div_qr (q, a, size_a, b, 3);
    check (size_c==1 && size_a==3 && a[0]==LIMB_MAX-7 && a[1]==LIMB_MAX-2 && a[2]==LIMB_MAX-3 && q[0]==LIMB_MAX-3);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX-3;
    size_a = 4;
    size_c = div_qr (q, a, size_a, b, 2);
    check (size_c==2 && size_a==2 && a[0]==LIMB_MAX-6 && a[1]==LIMB_MAX-6 && q[0]==LIMB_MAX-2 && q[1]==LIMB_MAX-3);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX-3;
    size_a = 4;
    size_c = div_qr (q, a, size_a, b, 2);
    check (size_c==2 && size_a==2 && a[0]==LIMB_MAX-6 && a[1]==LIMB_MAX-6 && q[0]==LIMB_MAX-2 && q[1]==LIMB_MAX-3);

    a[0] = a[1] = a[2] = a[3] = LIMB_MAX-3;
    size_a = 4;
    size_c = div_qr (q, a, size_a, b, 1);
    check (size_c==3 && size_a==1 && a[0]==LIMB_MAX-12 && q[0]==LIMB_MAX-8 && q[1]==LIMB_MAX-5 && q[2]==LIMB_MAX-2);

    set_zero (b, 3);
    b[2] = 8;
    a[0] = a[1] = a[2] = LIMB_MAX;
    size_a = 3;
    size_c = div_qr (nullptr, a, size_a, b, 3);
    check (size_c==0 && size_a==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==7);

    delete[] q;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return ok;
}



bool test_vec_t() {
  bool ok = true;

  { // Constructors, also operator [] and set_/is_zero()
    vec_t a;
    check (a.size==0 && a.limbs==nullptr);
    check (a.is_zero());
    vec_t b (2, CI_SIZE);
    check (b.size==0 && b.limbs!=nullptr);
    check (b.is_zero());
    b.size = 2;
    b[0] = 1;
    b[1] = 2;
    vec_t c(b);
    check (c.size==2 && c[1]==2 && c[0]==1);
    check (c==b && b==c);
    check (&c != &b);
    check (c.limbs != b.limbs);
    vec_t d(vec_t(123));
    check (d.size==1 && d[0]==123);
    d.set_zero();
    check (d.is_zero());
    vec_t e(10);
    check (e.size==1 && e[0]==10);
    vec_t f("123");
    check (f.size==1 && f[0]==123);
    vec_t g("FE", 16);
    check (g.size==1 && g[0]==254);
    vec_t h("1011", 2);
    check (h.size==1 && h[0]==11);
    vec_t x(4294967295u);
    check (x.size==limbs_in_<u32>());
    for (size_t i=0; i<limbs_in_<u32>(); ++i) check (x[i]==LIMB_MAX);
    vec_t z(18446744073709551615llu);
    check (z.size==limbs_in_<u64>());
    for (size_t i=0; i<limbs_in_<u64>(); ++i) check (z[i]==LIMB_MAX);
  }

  {	// Conversion from u8..u64 and string
    vec_t a(3, CI_SIZE);
    check (a.is_zero());
    a = 0LLU;
    check (a.is_zero());
    a = 123;
    check (a.size==1 && a[0]==123);
#if LIMB_BITS == 8
    a = 0xA1234;
    check (a.size==3 && a[2]==0xA && a[1]==0x12 && a[0]==0x34);
    vec_t b ("660020");
    check (b == a);
    b.set_zero();
    b.from_string ("A1234", 16);
    check (b == a);
    a = 0x1234LLU;
    check (a.size==2 && a[1]==0x12U && a[0]==0x34U);
    vec_t c ("4660");
    check (c == a);
    c.set_zero();
    c.from_string ("1234", 16);
    check (c == a);
    vec_t d ("1234", 16);
    check (d == c);
#elif LIMB_BITS == 16
    a = 0xA12345678LLU;
    check (a.size==3 && a[2]==0xAU && a[1]==0x1234U && a[0]==0x5678U);
    vec_t b ("43255092856");
    check (b == a);
    b.set_zero();
    b.from_string ("A12345678", 16);
    check (b == a);
    a = 0x12345678LLU;
    check (a.size==2 && a[1]==0x1234U && a[0]==0x5678U);
    vec_t c ("305419896");
    check (c == a);
    c.set_zero();
    c.from_string ("12345678", 16);
    check (c == a);
    vec_t d ("12345678", 16);
    check (d == c);
#elif LIMB_BITS == 32
    a.from_string ("A123456789ABCDEF0", 16);
    check (a.size==3 && a[2]==0xAU && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    vec_t b ("185779209204559306480");
    check (b == a);
    b.set_zero();
    b.from_string ("A123456789abcdef0", 16);
    check (b == a);
    a = 0x123456789ABCDEF0LLU;
    check (a.size==2 && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    vec_t c ("1311768467463790320");
    check (c == a);
    c.set_zero();
    c.from_string ("123456789ABCDEF0", 16);
    check (c == a);
    vec_t d ("123456789ABCDEF0", 16);
    check (d == c);
#elif LIMB_BITS == 64
    a.from_string ("A123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==3 && a[2]==0xAU && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    vec_t c = a.from_string ("123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==2 && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    a = 0x123456789ABCDEF0LLU;
    check (a.size==1 && a[0]==0x123456789ABCDEF0U);
    vec_t b ("1311768467463790320");
    check (b == a);
    vec_t d ("123456789ABCDEF0FEDCBA9876543210", 16);
    check (d == c);
#endif
    a.from_string ("0000000000");
    check (a.is_zero());
    a.from_string ("0000000001");
    check (a.size==1 && a[0]==1);
    a.from_string ("0000000001", 16);
    check (a.size==1 && a[0]==1);
    // 2**1039-1
    vec_t big ("5890680864316836766447387249177476247119386964598150177535756899376584320794655559932591384900650140340063891615625817543763223144510803885845624607194288107610698331745992221533871131893632012106238622173921469033288521558997823700137184806201826907368669534112523820726591354912103343876844956209126576528293887");
    check (big.size == (1039+LIMB_BITS-1)/LIMB_BITS);
    for (size_t i=0; i<1039/LIMB_BITS; ++i)
      check (big[i] == LIMB_MAX);
    check (big[big.size-1] == ((limb_t)1<<(1039%LIMB_BITS))-1);

    a = (u8)123;
    vec_t a8((u8)123);
    check (a.size==1 && a[0]==123);
    check (a == a8);
    a = (u16)123;
    vec_t a16((u16)123);
    check (a.size==1 && a[0]==123);
    check (a == a16);
    a = (u32)123;
    vec_t a32((u32)123);
    check (a.size==1 && a[0]==123);
    check (a == a32);
    a = (u64)123;
    vec_t a64((u64)123);
    check (a.size==1 && a[0]==123);
    check (a == a64);
	}

	{	// To / From string by back and forth conversion
    vec_t a(3, CI_SIZE);
    a.size = 3;
    a[0] = 1;
    a[1] = LIMB_MAX;
    a[2] = HIGH_BIT_MASK;
    char str[LIMB_BITS*3+1];
    vec_t b(3, CI_SIZE);
    for (u8 base=2; base<=36; ++base) {
      a.to_string (str, base);
      b.from_string (str, base);
      check (a == b);
      vec_t c (str, base);
      check (c == b);
    }
	}

	{ // Resize, Adapt
    vec_t a(3, CI_SIZE);
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a.adapt(2);
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.adapt(4);
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.size = 4;
    a[3] = 4;
    check (a.size==4 && a[0]==1 && a[1]==2 && a[2]==3 && a[3]==4);
    a.resize(2);
    check (a.is_zero());
    a.set_zero();
    a.adapt(100);
    a[99] = 123;
    check (a.limbs[99] == 123);
	}

	{	// Clear, swap, move
    vec_t a (3, CI_SIZE), b (3, CI_SIZE);
    a = 60000;
    a.clear();
    check (a.is_zero());
    a.adapt(3);
    a = 50000;
    b.move (a);
    check (b==50000 && a.is_zero());
    a.resize(4);
    a = 1000000;
    b.swap(a);
    check (a==50000 && b==1000000);
    a.clear();
    b.clear();
	}

	{ // Normalize
    vec_t a(10, CI_SIZE);
    a.size = 3;
    a[0] = a[1] = a[2] = 0;
    check (!a.is_normalized());
    check (a.normalize()==0);
    check (a.size == 0);
    check (a.is_zero());
    check (a.is_normalized());
    a.size = 3;
    a[0] = 1;
    a[1] = a[2] = 0;
    check (!a.is_normalized());
    check (a.normalize()==1);
    check (a.size == 1);
    check (!a.is_zero());
    check (a.is_normalized());
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a.is_normalized());
    check (a.normalize()==3);
    check (a.size == 3);
    check (!a.is_zero());
    a.size = 10;
    a[9] = a[8] = a[7] = 0;
    a[6] = LIMB_MAX;
    check (!a.is_normalized());
    check (a.normalize()==7);
    check (a.size == 7);
    check (a.is_normalized());
	}

	{ // Operator =
    vec_t a(3, CI_SIZE), b(3, CI_SIZE);
    a.size = 3;
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    b = a;
    check (a == b);
    check (&a != &b);
    check (&a.size != &b.size);
    check (a.limbs != b.limbs);
    b = vec_t("254");
    check (b.size==1 && b[0]==254);
    a.set_zero();
    check (a.is_zero());
    b = a;
    check (b.is_zero());
    a = 123;
    a = a;
  }

  { // Compare with built-in types
    vec_t a(10, CI_SIZE);
    u8 b8 = 0;
    u16 b16 = 0;
    u32 b32 = 0;
    u64 b64 = 0;

    a.set_zero();

    check (!(a!=b8));
    check (!(b8!=a));
    check ((a==b8));
    check ((b8==a));
    check (!(a>b8));
    check (!(a<b8));
    check ((a>=b8));
    check ((a<=b8));
    check (!(b8<a));
    check (!(b8>a));
    check ((b8<=a));
    check ((b8>=a));

    check (!(a!=b16));
    check (!(b16!=a));
    check ((a==b16));
    check ((b16==a));
    check (!(a>b16));
    check (!(a<b16));
    check ((a>=b16));
    check ((a<=b16));
    check (!(b16<a));
    check (!(b16>a));
    check ((b16<=a));
    check ((b16>=a));

    check (!(a!=b32));
    check (!(b32!=a));
    check ((a==b32));
    check ((b32==a));
    check (!(a>b32));
    check (!(a<b32));
    check ((a>=b32));
    check ((a<=b32));
    check (!(b32<a));
    check (!(b32>a));
    check ((b32<=a));
    check ((b32>=a));

    check (!(a!=b64));
    check (!(b64!=a));
    check ((a==b64));
    check ((b64==a));
    check (!(a>b64));
    check (!(a<b64));
    check ((a>=b64));
    check ((a<=b64));
    check (!(b64<a));
    check (!(b64>a));
    check ((b64<=a));
    check ((b64>=a));

    for (size_t i=0; i<8; ++i) {
      a = (u64)66<<(i*8);
      check (a!=b8);
      check (b8!=a);
      check (!(a==b8));
      check (!(b8==a));
      check (a>b8);
      check (!(a<b8));
      check (a>=b8);
      check (!(a<=b8));
      check (b8<a);
      check (!(b8>a));
      check (b8<=a);
      check (!(b8>=a));

      check (a!=b16);
      check (b16!=a);
      check (!(a==b16));
      check (!(b16==a));
      check (a>b16);
      check (!(a<b16));
      check (a>=b16);
      check (!(a<=b16));
      check (b16<a);
      check (!(b16>a));
      check (b16<=a);
      check (!(b16>=a));

      check (a!=b32);
      check (b32!=a);
      check (!(a==b32));
      check (!(b32==a));
      check (a>b32);
      check (!(a<b32));
      check (a>=b32);
      check (!(a<=b32));
      check (b32<a);
      check (!(b32>a));
      check (b32<=a);
      check (!(b32>=a));

      check (a!=b64);
      check (b64!=a);
      check (!(a==b64));
      check (!(b64==a));
      check (a>b64);
      check (!(a<b64));
      check (a>=b64);
      check (!(a<=b64));
      check (b64<a);
      check (!(b64>a));
      check (b64<=a);
      check (!(b64>=a));
    }

    a = b8 = 70;
    check (a.compare(b8) == 0);
    check (a.compare(b8-1) == +1);
    check (a.compare(b8+2) == -1);
    a = b16 = 60000;
    check (a.compare(b16) == 0);
    check (a.compare(b16-3) == +1);
    check (a.compare(b16+100) == -1);
    a = b32 = 2000000000llu;
    check (a.compare(b32) == 0);
    check (a.compare(b32-70000) == +1);
    check (a.compare(b32+1000000) == -1);
    a = b64 = 0xABCDEF0123456789;
    check (a.compare(b64) == 0);
    check (a.compare(b64-100) == +1);
    check (a.compare(b64+200) == -1);

  }

	{ // Compare with vec_t
    vec_t a(10, CI_SIZE), b(10, CI_SIZE);
    check (b.is_zero());
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a != b);
    check (!(a==b));
    b = a;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    check (a == b);
    check (!(a!=b));
    check (a <= b);
    check (a >= b);
    check (!(a>b));
    check (!(a<b));

    b.set_zero();
    check (a>b);
    check (!(a<b));
    check (a>=b);
    check (!(a<=b));
    check (b<a);
    check (!(b>a));
    check (b<=a);
    check (!(b>=a));

    b.size = 3;
    b[0] = 2;
    b[1] = 2;
    b[2] = 3;

    check (a!=b);
    check (!(a==b));
    check (a<b);
    check (!(a>b));
    check (a<=b);
    check (!(a>=b));
    check (b>a);
    check (!(b<a));
    check (b>=a);
    check (!(b<=a));

    a.set_zero();
    b.set_zero();
    check (a==b);
    check (!(a!=b));
    check (!(a<b));
    check (!(a>b));
    check (a<=b);
    check (a>=b);

    a.size = 2;
    a[0] = 1;
    a[1] = 2;
    b.size = 3;
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;
    check (a!=b);
    check (b!=a);
    check (!(a==b));
    check (!(b==a));
    check (a<b);
    check (b>a);
    check (a<=b);
    check (b>=a);
    check (!(a>b));
    check (!(b<a));
    check (!(a>=b));
    check (!(b<=a));
  }

  { // Inc, Dec
    vec_t a(3, CI_SIZE), b(3, CI_SIZE), c(3, CI_SIZE);
    a.size = 1;
    a[0] = LIMB_MAX;
    c = ++a;
    check (a.size==2 && a[0]==0 && a[1]==1);
    check (c == a);
    ++a;
    check (a.size==2 && a[0]==1 && a[1]==1);
    c = --a;
    check (a.size==2 && a[0]==0 && a[1]==1);
    check (c == a);
    --a;
    check (a.size==1 && a[0]==LIMB_MAX);
    --a;
    check (a.size==1 && a[0]==LIMB_MAX-1);

    a.set_zero();
    c = inc (b, a, 10);
    check (b.size==1 && b[0]==10);
    check (c == b);
    a = dec (c, b, 5);
    check (c.size==1 && c[0]==5);
    check (a == c);

    a.size = 1;
    a[0] = LIMB_MAX-10;
    c = a.inc(11);
    check (a.size==2 && a[0]==0 && a[1]==1);
    check (c == a);
    c = a.dec(LIMB_MAX);
    check (a.size==1 && a[0]==1);
    check (c == a);

    b = inc (c, a, 0);
    check (c.size==1 && c[0]==1);
    check (b == c);
    b = inc (c, c, 0);
    check (c.size==1 && c[0]==1);
    check (b == c);
    b = c.inc(0);
    check (c.size==1 && c[0]==1);
    check (b == c);

    b = dec (a, c, 0);
    check (a.size==1 && a[0]==1);
    check (b == a);
    b = dec (a, a, 0);
    check (a.size==1 && a[0]==1);
    check (b == a);
    b = c.dec(0);
    check (c.size==1 && c[0]==1);
    check (b == c);
    b = c.inc();
    check (b.size==1 && b[0]==2 && b==c);
    b = c.dec();
    check (b.size==1 && b[0]==1 && b==c);
  }

  {	// Add, sub for built-in types
    vec_t a(3, CI_SIZE), b(3, CI_SIZE), c(3, CI_SIZE);
    b.size = 1;
    b[0] = 1;
    c = b;
    b = c += LIMB_MAX;
    check (c.size==2 && c[0]==0 && c[1]==1);
    check (b == c);
    b = c -= 2;
    check (c.size==1 && c[0]==LIMB_MAX-1);
    check (b == c);
    a = c + 3;
    a.adapt(3);
    check (a.size==2 && a[0]==1 && a[1]==1);
    c.adapt(3);
    c = a - 2;
    c.adapt(3);
    check (c.size==1 && c[0]==LIMB_MAX);
    dec (a, a, 2);
    check (a.size==1 && a[0]==LIMB_MAX);
    inc (a, a, 3);
    check (a.size==2 && a[0]==2 && a[1]==1);
    c = a;
    b = a += 0;
    check (a == c);
    check (b == a);
    c = a + 0;
    c.adapt(3);
    check (c == a);
    a = a + 0;
    a.adapt(3);
    check (a == c);
    b = a -= 0;
    check (a == c);
    check (b == a);
    c.adapt(3);
    c = a - 0;
    c.adapt(3);
    check (c == a);
    a = a - 0;
    a.adapt(3);
    check (a == c);

    a.size = 1;
    a[0] = 10;
    c = a;
    b = a - 10;
    b.adapt(3);
    check (b.is_zero());
    a -= 10;
    check (a.is_zero());
    c = c - 10;
    c.adapt(3);
    check (c.is_zero());

    c.adapt(10);
    a = 1;
    c = a + 0xFFFFFFFFFFFFFFFEllu;
    check (c == 0xFFFFFFFFFFFFFFFFllu);
    c = a + 0xFFFFFFFEu;
    check (c == 0xFFFFFFFFu);
    c = a + (u16)0xFFFE;
    check (c == 0xFFFFu);
    c = a + (u8)0xFE;
    check (c == 0xFFu);

    c.adapt(10);
    a.adapt(10);
    a = 1;
    c = a += 0xFFFFFFFFFFFFFFFEllu;
    check (c==a && c==0xFFFFFFFFFFFFFFFFllu);
    a = 1;
    c = a += 0xFFFFFFFEu;
    check (c==a && c==0xFFFFFFFFu);
    a = 1;
    c = a += (u16)0xFFFE;
    check (c==a && c==0xFFFFu);
    a = 1;
    c = a += (u8)0xFE;
    check (c==a && c==0xFFu);

  }

  { // Add, Sub.
    vec_t a(4, CI_SIZE), b(4, CI_SIZE), c(4, CI_SIZE);
    a.size = 3;
    a[0] = LIMB_MAX;
    a[1] = LIMB_MAX;
    a[2] = LIMB_MAX;
    b.size = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX;
    c = a += b;
    check (a.size==4 && a[3]==1 && a[2]==0 && a[1]==LIMB_MAX && a[0]==LIMB_MAX-1);
    check (c == a);

    c.size = 2;
    c[0] = 1;
    c[1] = LIMB_MAX;
    c += a;
    check (c.size==4 && c[3]==1 && c[2]==1 && c[1]==LIMB_MAX-1 && c[0]==LIMB_MAX);
    b = c -= a;
    check (c.size==2 && c[1]==LIMB_MAX && c[0]==1);
    check (b == c);
    c += a;
    check (c.size==4 && c[3]==1 && c[2]==1 && c[1]==LIMB_MAX-1 && c[0]==LIMB_MAX);
    b = a + c;
    check (b.size==4 && b[3]==2 && b[2]==2 && b[1]==LIMB_MAX-1 && b[0]==LIMB_MAX-2);
    check (c>=a);
    b = c - a;
    b.adapt(4);
    check (b.size==2 && b[1]==LIMB_MAX && b[0]==1);
    a.size = 1;
    a[0] = 1;
    a = a + a;
    a.adapt(4);
    check (a.size == 1 && a[0] == 2);
    a = a - a;
    a.adapt(4);
    check (a.is_zero());
    a.size = 1;
    a[0] = LIMB_MAX-1;
    a = a + a;
    a.adapt(4);
    check (a.size == 2 && a[1] == 1 && a[0] == LIMB_MAX-3);
    a = a - a;
    a.adapt(4);
    check (a.is_zero());
    a.size = 2;
    a[0] = a[1] = 1;
    b.size = 2;
    b[0] = b[1] = LIMB_MAX;
    b = b + a;
    b.adapt(4);
    check (b.size==3 && b[0]==0 && b[1]==1 && b[2]==1);
    b = b - a;
    b.adapt(4);
    check (b.size==2 && b[0]==LIMB_MAX && b[1]==LIMB_MAX);
    b = a + b;
    b.adapt(4);
    check (b.size==3 && b[0]==0 && b[1]==1 && b[2]==1);
    a = b - a;
    b.adapt(4);
    check (a.size==2 && a[0]==LIMB_MAX && a[1]==LIMB_MAX);

    c = a -= a;
    check (a.is_zero());
    check (c.is_zero());
    ++a;
    c = a += a;
    check (a.size==1 && a[0]==2);
    check (c == a);
    a += a;
    check (a.size==1 && a[0]==4);
    a += a;
    check (a.size==1 && a[0]==8);
    a += a;
    check (a.size==1 && a[0]==16);
    a[0] = LIMB_MAX;
    a += a;
    check (a.size==2 && a[1]==1 && a[0]==LIMB_MAX-1);
    b = a - a;
    check (b.is_zero());

    a.adapt(10);
    c.adapt(10);
    a = 18446744073709551615llu;
    c = a - 9223372036854775807llu;
    check (c == 9223372036854775808llu);
    a -= 9223372036854775807llu;
    check (a == c);
    c = a - 4294967295u;
    check (c == 9223372032559808513llu);
    a -= 4294967295u;
    check (a == c);
    c = a - (u16)65535;
    check (c == 9223372032559742978llu);
    a -= (u16)65535;
    check (a == c);
    c = a - (u8)255;
    check (c == 9223372032559742723llu);
    a -= (u8)255;
    check (a == c);
    c = 18446744073709551615llu - a;
    check (c == 9223372041149808892llu);
    a = 254;
    c = (u8)255 - a;
    check (c == 1);
  }

  { // Mul by limb
    vec_t a(3, CI_SIZE), b(3, CI_SIZE);
    a.size = 2;
    a[0] = a[1] = 1;
    b = a *= 1;
    check (a.size==2 && a[0]==1 && a[1]==1);
    check (b == a);
    b = a *= 0;
    check (a.is_zero());
    check (b == a);
    b = a *= 10;
    check (a.is_zero());
    check (b == a);
    b = a *= 1;
    check (a.is_zero());
    check (b == a);

    a.size = 2;
    a[0] = 1;
    a[1] = 2;
    a *= 3;
    check (a.size==2 && a[0]==3 && a[1]==6);
    b = a * 3;
    b.adapt(3);
    check (b.size==2 && b[0]==9 && b[1]==18);
    a = a * 3;
    a.adapt(3);
    check (a.size==2 && a[0]==9 && a[1]==18);
    a.size = 1;
    a[0]=LIMB_MAX;
    b = a * LIMB_MAX;
    b.adapt(3);
    check (b.size==2 && b[0]==1 && b[1]==LIMB_MAX-1);
    a *= LIMB_MAX;
    check (a == b);

    a.size = 2;
    a[0] = 1;
    a[1] = LIMB_MAX;
    b = a * 128;
    check (b.size==3 && b[2]==127 && b[1]==(limb_t)(LIMB_MAX<<7) && b[0]==128);
    a *= 128;
    check (a == b);

    vec_t c(100, CI_SIZE);
    a.adapt(100);
    a = "18447870523372208128";
    c = a * 18446744073709551615llu;
    check (c == "340303146249577404581276070867338526720");
    c = 18446744073709551615llu * a;
    check (c == "340303146249577404581276070867338526720");
    a *= 18446744073709551615llu;
    check (a == c);
    c = a * 4294967295u;
    check (c == "1461590883527536860247563893741321255955883622400");
    c = 4294967295u * a;
    check (c == "1461590883527536860247563893741321255955883622400");
    a *= 4294967295u;
    check (a == c);
    c = a * (u16)65535;
    check (c == "95785358551977128136324099776337488509068833193984000");
    c = (u16)65535 * a;
    check (c == "95785358551977128136324099776337488509068833193984000");
    a *= (u16)65535;
    check (a == c);
    c = a * (u8)255;
    check (c == "24425266430754167674762645442966059569812552464465920000");
    c = (u8)255 * a;
    check (c == "24425266430754167674762645442966059569812552464465920000");
    a *= (u8)255;
    check (a == c);
  }

  { // addmul, submul.
    vec_t a(4, CI_SIZE), b(4, CI_SIZE), c(4, CI_SIZE), d(4, CI_SIZE);
    a.size = 2;
    a[0] = 1;
    a[1] = 2;
    b.size = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX-2;
    d = addmul (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    check (d == c);
    d = submul (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    check (d == c);
    d = addmul (c, a, b, 1);
    check (c.size==3 && c[0]==0 && c[1]==0 && c[2]==1);
    check (d == c);
    submul (d, c, b, 1);
    check (d.size==2 && d[0]==1 && d[1]==2);
    d = addmul (c, a, a, 3);
    check (c.size==2 && c[0]==4 && c[1]==8);
    check (d == c);
    submul (d, c, a, 3);
    check (d.size==2 && d[0]==1 && d[1]==2);
    addmul (a, b, b, 3);
    check (a.size==3 && a[0]==LIMB_MAX-3 && a[1]==LIMB_MAX-8 && a[2]==3);
    submul (d, a, b, 3);
    check (d.size==2 && d[0]==LIMB_MAX && d[1]==LIMB_MAX-2);

    a.size = 1;
    a[0] = 2;
    b = addmul (a, a, a, LIMB_MAX);
    check (a.size==2 && a[0]==0 && a[1]==2);
    check (b == a);
    b = submul (a, a, a, 1);
    check (a.is_zero());
    check (b == a);

    a.size = 1;
    a[0] = 2;
    b.size = 2;
    b[0] = 1;
    b[1] = 2;
    c = b.addmul (a, 0);
    check (b.size==2 && b[0]==1 && b[1]==2);
    check (c == b);
    c = b.addmul (a, 5);
    check (b.size==2 && b[0]==11 && b[1]==2);
    check (c == b);
    c = b.addmul (b, 5);
    check (b.size==2 && b[0]==66 && b[1]==12);
    check (c == b);
    c = b.submul (a, 0);
    check (b.size==2 && b[0]==66 && b[1]==12);
    check (c == b);
    c = b.submul (a, 1);
    check (b.size==2 && b[0]==64 && b[1]==12);
    check (c == b);
    c = b.submul (a, LIMB_MAX);
    check (b.size==2 && b[0]==66 && b[1]==10);
    check (c == b);

    a.set_zero();
    c = a.addmul (a, 10);
    check (a.is_zero());
    check (c == a);
    c = a.submul (a, 10);
    check (a.is_zero());
    check (c == a);

    c = a.addmul (b, 5);
    check (a == b*5);
    check (c == a);
  }

  { // Mul
    vec_t a(10, CI_SIZE), b(10, CI_SIZE), c(10, CI_SIZE), d(10, CI_SIZE);
    a.size = 3;
    a[0] = 3;
    a[1] = 2;
    a[2] = 1;
    b.size = 3;
    b[0] = 6;
    b[1] = 5;
    b[2] = 4;
    c = a * b;
    c.adapt(10);
    check (c.size==5 && c[0]==18 && c[1]==27 && c[2]==28 && c[3]==13 && c[4]==4);
    c = a;
    d = c *= b;
    check (c.size==5 && c[0]==18 && c[1]==27 && c[2]==28 && c[3]==13 && c[4]==4);
    check (d == c);
    c.size = 1;
    c[0] = 3;
    d = a *= c;
    check (a.size==3 && a[0]==9 && a[1]==6 && a[2]==3);
    check (d == a);
    c.set_zero();
    b = a * c;
    b.adapt(10);
    check (b.is_zero());
    d = a *= c;
    check (a.is_zero());
    check (d == a);
    vec_t z(100, CI_SIZE);
    z = 1031;
    for (size_t i=1; i<=5; ++i)  z *= z;
    check (z == "2656301020209296604918669081053362592769335327396991619655420824350472399920137716010779784559361");
  }

  { // Shift left and right
    vec_t a(10, CI_SIZE), b(10, CI_SIZE), c(10, CI_SIZE);
    a.set_zero();
    b = a <<= 0;
    check (a.is_zero());
    check (b == a);
    b = a <<= 1000;
    check (a.is_zero());
    check (b == a);
    b = a >>= 0;
    check (a.is_zero());
    check (b == a);
    b = a >>= 1000;
    check (a.is_zero());
    check (b == a);
    c = a << 0;
    c.adapt(10);
    check (c.is_zero());
    c = a << 1000;
    c.adapt(10);
    check (c.is_zero());
    c = a >> 0;
    c.adapt(10);
    check (c.is_zero());
    c = a >> 1000;
    c.adapt(10);
    check (c.is_zero());

    a.size = 3;
    a[0] = 10;
    a[1] = LIMB_MAX;
    a[2] = 14;
    b = a;
    a <<= 0;
    check (a==b);
    a >>= 0;
    check (a==b);

    b = a << 1;
    b.adapt(10);
    check (b.size==3 && b[0]==20 && b[1] == LIMB_MAX-1 && b[2] == 29);
    c = a <<= 1;
    check (a.size==3 && a[0]==20 && a[1] == LIMB_MAX-1 && a[2] == 29);
    check (c == a);
    b = a >> 1;
    b.adapt(10);
    check (b.size==3 && b[0]==10 && b[1] == LIMB_MAX && b[2] == 14);
    c = a >>= 1;
    check (a.size==3 && a[0]==10 && a[1] == LIMB_MAX && a[2] == 14);
    check (c == a);

    const limb_t H = (limb_t)1<<(LIMB_BITS-1);
    a[2] = H;

    b = a << (LIMB_BITS-1);
    b.adapt(10);
    check (b.size==4 && b[0]==0 && b[1]==H+5 && b[2]==LIMB_MAX/2 && b[3]==H/2);
    a <<= (LIMB_BITS-1);
    check (a.size==4 && a[0]==0 && a[1]==H+5 && a[2]==LIMB_MAX/2 && a[3]==H/2);
    b = a >> (LIMB_BITS-1);
    b.adapt(10);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    a >>= (LIMB_BITS-1);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);

    b = a << (LIMB_BITS-1+LIMB_BITS);
    b.adapt(10);
    check (b.size==5 && b[0]==0 && b[1]==0 && b[2]==H+5 && b[3]==LIMB_MAX/2 && b[4]==H/2);
    a <<= (LIMB_BITS-1+LIMB_BITS);
    check (a.size==5 && a[0]==0 && a[1]==0 && a[2]==H+5 && a[3]==LIMB_MAX/2 && a[4]==H/2);
    b = a >> (LIMB_BITS-1+LIMB_BITS);
    b.adapt(10);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    a >>= (LIMB_BITS-1+LIMB_BITS);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);

    b = a << (LIMB_BITS-1+3*LIMB_BITS);
    b.adapt(10);
    check (b.size==7 && b[0]==0 && b[1]==0 && b[2]==0 && b[3]==0 && b[4]==H+5 && b[5]==LIMB_MAX/2 && b[6]==H/2);
    c = a <<= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==7 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==H+5 && a[5]==LIMB_MAX/2 && a[6]==H/2);
    check (c == a);
    b = a >> (LIMB_BITS-1+3*LIMB_BITS);
    b.adapt(10);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    c = a >>= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);
    check (c == a);

    a >>= LIMB_BITS;
    check (a.size==2 && a[0]==LIMB_MAX && a[1]==H);
    a <<= LIMB_BITS;
    check (a.size==3 && a[0]==0 && a[1]==LIMB_MAX && a[2]==H);
  }

  { // Div by built-in type
    vec_t a(5, CI_SIZE), b(5, CI_SIZE);
    a.set_zero();
    b = a /= 1;
    check (a.is_zero());
    check (b == a);
    b = a/1;
    b.adapt(5);
    check (b.is_zero());
    b = a /= 123;
    check (a.is_zero());
    check (b == a);
    b = a/123;
    b.adapt(5);
    check (b.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    b = a/3;
    b.adapt(5);
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= 3;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a *= LIMB_MAX;
    b = a/LIMB_MAX;
    b.adapt(5);
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a/LIMB_MAX;
    b.adapt(5);
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a.size = 3;
    a[0] = a[1] = a[2] = LIMB_MAX;
    a *= LIMB_MAX;
    b = a/LIMB_MAX;
    b.adapt(5);
    check (b.size==3 && b[0]==LIMB_MAX && b[1]==LIMB_MAX && b[2]==LIMB_MAX);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==LIMB_MAX);

    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a/LIMB_MAX;
    b.adapt(5);
    check (b.size==3 && b[0]==LIMB_MAX && b[1]==LIMB_MAX && b[2]==LIMB_MAX);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==LIMB_MAX);

    a.size = 3;
    a[0] = 128;
    a[1] = (limb_t)(LIMB_MAX<<7);
    a[2] = 127;
    b = a/128;
    b.adapt(5);
    check (b.size==2 && b[1]==LIMB_MAX && b[0]==1);
    a /= 128;
    check (a.size==2 && a[1]==LIMB_MAX && a[0]==1);

    b.adapt(100);
    a.adapt(100);
    a = "6277101735386673878902659511859823688140582075920999192721";
    b = a / 18446744073709551615llu;
    check (b == "340282366920938090248848508140124695342");
    b = 18446744073709551615llu / a;
    check (b.is_zero());
    b.adapt(100);
    b = a / 1000000000u;
    check (b == "6277101735386673878902659511859823688140582075920");
    b.adapt(100);
    b = a / (u16)10000;
    check (b == "627710173538667387890265951185982368814058207592099919");
    b.adapt(100);
    b = a / (u8)100;
    check (b == "62771017353866738789026595118598236881405820759209991927");
    a /= 1000000000000000000llu;
    check (a == "6277101735386673878902659511859823688140");
    a /= 1000000000u;
    check (a == "6277101735386673878902659511859");
    a /= (u16)10000;
    check (a == "627710173538667387890265951");
    a /= (u8)100;
    check (a == "6277101735386673878902659");

  }

  { // Mod by limb
    vec_t a(5, CI_SIZE), b(5, CI_SIZE);
    a.set_zero();
    a %= 1;
    check (a.is_zero());
    check (a%1 == 0);
    a %= 123;
    check (a.is_zero());
    check (a%123 == 0);

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    check (a%3 == 0);
    a %= 3;
    check (a.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a *= LIMB_MAX;
    check (a%LIMB_MAX == 0);
    a %= LIMB_MAX;
    check (a.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    check (a%LIMB_MAX == LIMB_MAX-1);
    a %= LIMB_MAX;
    check (a.size==1 && a[0]==LIMB_MAX-1);

    a.size = 3;
    a[0] = a[1] = a[2] = LIMB_MAX;
    a *= LIMB_MAX;
    b = a%LIMB_MAX;
    check (b.is_zero());
    a %= LIMB_MAX;
    check (a.is_zero());

    a.size = 3;
    a[0] = a[1] = a[2] = LIMB_MAX;
    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a%LIMB_MAX;
    check (b.size==1 && b[0]==LIMB_MAX-1);
    a %= LIMB_MAX;
    check (a.size==1 && a[0]==LIMB_MAX-1);

    b.adapt(100);
    a.adapt(100);
    a = "6277101735386673878902659511859823688140582075920999192721";
    b = a % 18446744073709551615llu;
    check (18446744073700115391llu == b);
    u64 b64 = 18446744073709551615llu % a;
    check (b64 == 18446744073709551615llu);
    u32 b32 = a % 1000000000u;
    check (b32 == 999192721);
    u16 b16 = a % (u16)10000;
    check (b16 == 2721);
    u8 b8 = a % (u8)100;
    check (b8 == 21);
    a = "6277101735386673878902659511859823688140582075920999192721";
    a %= 1000000000000000000llu;
    check (a == 582075920999192721llu);
    a %= 1000000000u;
    check (a == 999192721u);
    a %= (u16)10000;
    check (a == 2721);
    a %= (u8)100;
    check (a == 21);
  }

  { // Div, Mod
    vec_t a(100, CI_SIZE), b(100, CI_SIZE), c(100, CI_SIZE);
    a.set_zero();
    b.size = 1;
    b[0] = 1;
    c = a / b;
    c.adapt(100);
    check (c.is_zero());
    a.size = 1;
    a[0] = 1;
    c = a / b;
    c.adapt(100);
    check (c.size==1 && c[0]==1);
    a = "333231302928272625242322212019181716151413121110987654321";
    b = "12345678910111213141516171819202122";
    c = a / b;
    c.adapt(100);
    check (c == "26991735760708423406858");
    a /= b;
    check (a == c);

    a = "333231302928272625242322212019181716151413121110987654321";
    b = "12345678910111213141516171819202122";
    c = a % b;
    c.adapt(100);
    check (c == "2270551139277776469142220444701645");
    a %= b;
    check (a == c);

    a.size = 4;
    a[0] = a[1] = a[2] = a[3] = LIMB_MAX;
    --a;
    b.size = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX;
    c = a / b;
    c.adapt(100);
    check (c.size==3 && c[2]==1 && c[1]==0 && c[0]==0);
    c = a % b;
    c.adapt(100);
    check (c.size==2 && c[1]==LIMB_MAX && c[0]==LIMB_MAX-1);
  }

	return ok;
}



/*
bool test_num_t() {
  bool ok = true;

  { // Constructors, also operator [] and set_/is_zero()
    num_t a;
    check (a.size==0 && a.limbs==nullptr);
    check (a.is_zero());
    num_t b(3);
    check (b.size==0 && b.limbs!=nullptr);
    check (b.is_zero());
    b.size = -2;
    b[0] = 1;
    b[1] = 2;
    num_t c(b);
    check (c.size==-2 && c[1]==2 && c[0]==1);
    check (c==b && b==c);
    check (&c != &b);
    check (c.limbs != b.limbs);
    num_t d(c+b);
    check (d.size==-2 && d[1]==4 && d[0]==2);
    check (c.size==-2 && c[1]==2 && c[0]==1);
    check (c==b);
    check (d.limbs!=b.limbs && d.limbs!=c.limbs);
    d.set_zero();
    check (d.is_zero());
  }

  {	// Compare
    num_t a(2);
    a.size = 2;
    a[0] = 1;
    a[1] = 1;

    check (a > 0);
    check (a > LIMB_MAX);
    check (a > -128);
    check (a >= 0);
    check (a >= LIMB_MAX);
    check (a >= -128);
    check (a != 0);
    check (a != LIMB_MAX);
    check (a != -128);
    check (!(a < 0));
    check (!(a < LIMB_MAX));
    check (!(a < -128));
    check (!(a <= 0));
    check (!(a <= LIMB_MAX));
    check (!(a <= -128));
    check (!(a == 0));
    check (!(a == LIMB_MAX));
    check (!(a == -128));

    a.neg();

    check (!(a > 0));
    check (!(a > LIMB_MAX));
    check (!(a > -128));
    check (!(a >= 0));
    check (!(a >= LIMB_MAX));
    check (!(a >= -128));
    check ((a != 0));
    check ((a != LIMB_MAX));
    check ((a != -128));
    check ((a < 0));
    check ((a < LIMB_MAX));
    check ((a < -128));
    check ((a <= 0));
    check ((a <= LIMB_MAX));
    check ((a <= -128));
    check (!(a == 0));
    check (!(a == LIMB_MAX));
    check (!(a == -128));

    a.size = 1;
    a[0] = 128;

    check ((a > 0));
    check (!(a > LIMB_MAX));
    check ((a > -128));
    check ((a >= 0));
    check (!(a >= LIMB_MAX));
    check ((a >= -128));
    check ((a != 0));
    check ((a != LIMB_MAX));
    check ((a != -128));
    check (!(a < 0));
    check ((a < LIMB_MAX));
    check (!(a < -128));
    check (!(a <= 0));
    check ((a <= LIMB_MAX));
    check (!(a <= -128));
    check (!(a == 0));
    check (!(a == LIMB_MAX));
    check (!(a == -128));

    a.neg();

    check (!(a > 0));
    check (!(a > LIMB_MAX));
    check (!(a > -128));
    check (!(a >= 0));
    check (!(a >= LIMB_MAX));
    check ((a >= -128));
    check ((a != 0));
    check ((a != LIMB_MAX));
    check (!(a != -128));
    check ((a < 0));
    check ((a < LIMB_MAX));
    check (!(a < -128));
    check ((a <= 0));
    check ((a <= LIMB_MAX));
    check ((a <= -128));
    check (!(a == 0));
    check (!(a == LIMB_MAX));
    check ((a == -128));

    a.set_zero();

    check (!(a > 0));
    check (!(a > LIMB_MAX));
    check ((a > -128));
    check ((a >= 0));
    check (!(a >= LIMB_MAX));
    check ((a >= -128));
    check (!(a != 0));
    check ((a != LIMB_MAX));
    check ((a != -128));
    check (!(a < 0));
    check ((a < LIMB_MAX));
    check (!(a < -128));
    check ((a <= 0));
    check ((a <= LIMB_MAX));
    check (!(a <= -128));
    check ((a == 0));
    check (!(a == LIMB_MAX));
    check (!(a == -128));

    a = (u64)LIMB_MAX;
    check (a == LIMB_MAX);

    a.size = 2;
    a[0] = a[1] = 1;
    num_t b(a);
    check (a == b);
    a.neg();
    check (a < b);
    check (b > a);

    b = (u64)1;
    a.neg();
    check (a > b);
    check (b < a);
    a.neg();
    check (a < b);
    check (b > a);
  }

  {	// Conversion from i64/u64 and string
    num_t a(3);
    check (a.is_zero());
    a = 0LLU;
    check (a.is_zero());
    a = 123LLU;
    check (a.size==1 && a[0]==123);
    a = -123LL;
    check (a.size==-1 && a[0]==123);
    a = 54LL;
    check (a.size==1 && a[0]==54);
#if LIMB_BITS == 8
    a = 0xA1234LLU;
    check (a.size==3 && a[2]==0xAU && a[1]==0x12U && a[0]==0x34U);
    num_t b ("660020");
    check (b == a);
    b.set_zero();
    b.from_string ("+A1234", 16);
    check (b == a);
    a = 0x1234LLU;
    check (a.size==2 && a[1]==0x12U && a[0]==0x34U);
    num_t c ("+4660");
    check (c == a);
    c.set_zero();
    c.from_string ("1234", 16);
    check (c == a);
    num_t d ("+1234", 16);
    check (d == c);

    a = -(0xA1234LL);
    check (a.size==-3 && a[2]==0xAU && a[1]==0x12U && a[0]==0x34U);
    b = "-660020";
    check (b == a);
    b.set_zero();
    b.from_string ("-A1234", 16);
    check (b == a);
    a = -(0x1234LL);
    check (a.size==-2 && a[1]==0x12U && a[0]==0x34U);
    c = "-4660";
    check (c == a);
    c.set_zero();
    c.from_string ("-1234", 16);
    check (c == a);
    num_t e ("-1234", 16);
    check (e == c);

#elif LIMB_BITS == 16
    a = 0xA12345678LLU;
    check (a.size==3 && a[2]==0xAU && a[1]==0x1234U && a[0]==0x5678U);
    num_t b ("+43255092856");
    check (b == a);
    b.set_zero();
    b.from_string ("+A12345678", 16);
    check (b == a);
    a = 0x12345678LLU;
    check (a.size==2 && a[1]==0x1234U && a[0]==0x5678U);
    num_t c ("305419896");
    check (c == a);
    c.set_zero();
    c.from_string ("+12345678", 16);
    check (c == a);
    num_t d ("12345678", 16);
    check (d == c);

    a = -(0xA12345678LL);
    check (a.size==-3 && a[2]==0xAU && a[1]==0x1234U && a[0]==0x5678U);
    b = "-43255092856";
    check (b == a);
    b.set_zero();
    b.from_string ("-A12345678", 16);
    check (b == a);
    a = -(0x12345678LL);
    check (a.size==-2 && a[1]==0x1234U && a[0]==0x5678U);
    c = "-305419896";
    check (c == a);
    c.set_zero();
    c.from_string ("-12345678", 16);
    check (c == a);
    num_t e ("-12345678", 16);
    check (e == c);
#elif LIMB_BITS == 32
    a.from_string ("+A123456789ABCDEF0", 16);
    check (a.size==3 && a[2]==0xAU && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    num_t b ("+185779209204559306480");
    check (b == a);
    b.set_zero();
    b.from_string ("A123456789abcdef0", 16);
    check (b == a);
    a = 0x123456789ABCDEF0LLU;
    check (a.size==2 && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    num_t c ("1311768467463790320");
    check (c == a);
    c.set_zero();
    c.from_string ("123456789ABCDEF0", 16);
    check (c == a);
    num_t d ("123456789ABCDEF0", 16);
    check (d == c);

    a.from_string ("-A123456789ABCDEF0", 16);
    check (a.size==-3 && a[2]==0xAU && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    b = "-185779209204559306480";
    check (b == a);
    b.set_zero();
    b.from_string ("-A123456789abcdef0", 16);
    check (b == a);
    a = -(0x123456789ABCDEF0LL);
    check (a.size==-2 && a[1]==0x12345678U && a[0]==0x9ABCDEF0U);
    c = "-1311768467463790320";
    check (c == a);
    c.set_zero();
    c.from_string ("-123456789ABCDEF0", 16);
    check (c == a);
    num_t e ("-123456789ABCDEF0", 16);
    check (e == c);
#elif LIMB_BITS == 64
    a.from_string ("+A123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==3 && a[2]==0xAU && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    num_t c = a.from_string ("+123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==2 && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    a = 0x123456789ABCDEF0LLU;
    check (a.size==1 && a[0]==0x123456789ABCDEF0U);
    num_t b ("1311768467463790320");
    check (b == a);
    num_t d ("123456789ABCDEF0FEDCBA9876543210", 16);
    check (d == c);

    a.from_string ("-A123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==-3 && a[2]==0xAU && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    c = a.from_string ("-123456789ABCDEF0FEDCBA9876543210", 16);
    check (a.size==-2 && a[1]==0x123456789ABCDEF0LLU && a[0]==0xFEDCBA9876543210LLU);
    a = -(0x123456789ABCDEF0LL);
    check (a.size==-1 && a[0]==0x123456789ABCDEF0U);
    b = "-1311768467463790320";
    check (b == a);
    num_t e ("-123456789ABCDEF0FEDCBA9876543210", 16);
    check (e == c);
#endif
    a.from_string ("+0000000000");
    check (a.is_zero());
    a.from_string ("-0000000000");
    check (a.is_zero());
    a.from_string ("0000000001");
    check (a.size==1 && a[0]==1);
    a.from_string ("-0000000001");
    check (a.size==-1 && a[0]==1);
    a.from_string ("0000000001", 16);
    check (a.size==1 && a[0]==1);
    a.from_string ("-0000000001", 16);
    check (a.size==-1 && a[0]==1);
    // -(2**1039-1)
    num_t big ("-5890680864316836766447387249177476247119386964598150177535756899376584320794655559932591384900650140340063891615625817543763223144510803885845624607194288107610698331745992221533871131893632012106238622173921469033288521558997823700137184806201826907368669534112523820726591354912103343876844956209126576528293887");
    check (big.size == -(1039+LIMB_BITS-1)/LIMB_BITS);
    for (size_t i=0; i<1039/LIMB_BITS; ++i)
      check (big[i] == LIMB_MAX);
    check (big[-big.size-1] == ((limb_t)1<<(1039%LIMB_BITS))-1);
	}

	{	// To / From string by back and forth conversion
    num_t a(3);
    a.size = -3;
    a[0] = 1;
    a[1] = LIMB_MAX;
    a[2] = (limb_t)1 << (LIMB_BITS-1);	// 0x80..00
    char str[LIMB_BITS*3+2];
    num_t b(3);
    for (u8 base=2; base<=36; ++base) {
      a.to_string (str, base);
      b.from_string (str, base);
      check (a == b);
      num_t c (str, base);
      check (c == b);
    }
	}

	{ // Resize, Adapt
    num_t a(3);
    a.size = -3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a.adapt(2);
    check (a.size==-3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.adapt(4);
    check (a.size==-3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.size = -4;
    a[3] = 4;
    check (a.size==-4 && a[0]==1 && a[1]==2 && a[2]==3 && a[3]==4);
    a.resize(2);
    check (a.is_zero());
	}

	{ // Normalize
    num_t a(10);
    a.size = -3;
    a[0] = a[1] = a[2] = 0;
    check (!a.is_normalized());
    check (a.normalize()==0);
    check (a.size == 0);
    check (a.is_zero());
    check (a.is_normalized());
    a.size = -3;
    a[0] = 1;
    a[1] = a[2] = 0;
    check (!a.is_normalized());
    check (a.normalize()==-1);
    check (a.size == -1);
    check (!a.is_zero());
    check (a.is_normalized());
    a.size = -3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a.is_normalized());
    check (a.normalize()==-3);
    check (a.size == -3);
    check (!a.is_zero());
    a.size = -10;
    a[9] = a[8] = a[7] = 0;
    a[6] = LIMB_MAX;
    check (!a.is_normalized());
    check (a.normalize()==-7);
    check (a.size == -7);
    check (a.is_normalized());
	}

	{ // Operator =
    num_t a(3), b(3);
    a.size = -3;
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    b = a;
    check (a==b);
    check (&a != &b);
    check (&a.size != &b.size);
    check (a.limbs != b.limbs);
    b = a+a;
    check (b.size==-3 && b[0]==0 && b[1]==2 && b[2]==4);
    a.set_zero();
    check (a.is_zero());
    b = a;
    check (b.is_zero());
  }

  {	// Unary 'minus' and neg()
    num_t a ("123");
    check (a.size==1 && a[0]==123);
    num_t b (-a);
    check (a.size == -b.size && a[0] == b[0]);
    a = -a;
    check (a.size==-1 && a[0]==123);
    num_t c(1);
    c = -a;
    check (c.size==1 && c[0]==123);
    check (a != -a);
    a.set_zero();
    check (a == -a);
    a = "200";
    b = a.neg();
    check (a == b);
    check (a == "-200");
    a.set_zero();
    a.neg();
    check (a.is_zero());
  }

  {	// inc, dec, ++, --
    num_t a(2);
    check (a.is_zero());
    --a;
    check (a == "-1");
    ++a;
    check (a.is_zero());
    ++a;
    check (a == "+1");
    a = (u64)LIMB_MAX;
    ++a;
    check (a.size==2 && a[0]==0 && a[1]==1);
    --a;
    check (a.size==1 && a[0]==LIMB_MAX);

    a.neg();
    --a;
    check (a.size==-2 && a[0]==0 && a[1]==1);
    ++a;
    check (a.size==-1 && a[0]==LIMB_MAX);

    a = (u64)LIMB_MAX-10;
    a.neg();
    num_t b(2);
    b = a-=13;
    check (a.size==-2 && a[0]==2 && a[1]==1);
    check (b == a);
    b = a+=13;
    check (-a == LIMB_MAX-10);
    check (b == a);
    a += LIMB_MAX;
    check (a == 10);
    b = a+=LIMB_MAX;
    check (a.size==2 && a[0]==9 && a[1]==1);
    check (b == a);
  }

  {	// add, sub
    num_t a(16), b(16), c(16);
    a = "12345678910111213141516171819202122";
    b = "22212019181716151413121110987654321";
    c = a + b;
    check (c == "34557698091827364554637282806856443");
    c = a - b;
    check (c == "-9866340271604938271604939168452199");
    check (a+b == b+a);
    check (a-b == -(b-a));

    a = num_t ("1") << 2*LIMB_BITS;
    --a;	// Two limbs as "0xFF..FF"
    check (a.size==2 && a[0]==LIMB_MAX && a[1]==LIMB_MAX);
    b = num_t ("1") << 3*LIMB_BITS;
    --b;	// Three limbs as "0xFF..FF"
    check (b.size==3 && b[0]==LIMB_MAX && b[1]==LIMB_MAX && b[2]==LIMB_MAX);

    c = a;
    c += b;
    check (c.size==4 && c[0]==LIMB_MAX-1 && c[1]==LIMB_MAX && c[2]==0 && c[3]==1);
    check (c == a+b);

    c = b;
    c += a;
    check (c.size==4 && c[0]==LIMB_MAX-1 && c[1]==LIMB_MAX && c[2]==0 && c[3]==1);
    check (c == b+a);

    c = a;
    c -= b;
    check (c.size==-3 && c[0]==0 && c[1]==0 && c[2]==LIMB_MAX);
    check (c == a-b);

    c = b;
    c -= a;
    check (c.size==3 && c[0]==0 && c[1]==0 && c[2]==LIMB_MAX);
    check (c == b-a);

    a = c-=c;
    check (c.is_zero());
    check (a.is_zero());

    a = c+=c;
    check (c.is_zero());
    check (a.is_zero());

    a = b<<LIMB_BITS;

    check (a == (a-b)+b);
    check (a == (a+b)-b);
    check (b == (b-a)+a);
    check (b == (b+a)-a);
    check (-a == (-a+b)-b);
    check (-a == (-a-b)+b);
    check (-b == (-b+a)-a);
    check (-b == (-b-a)+a);
  }

  {	// mul by limb and slimb
    num_t a(16), b(16), c(16);
    check (a.is_zero());

    b = a.mul (0);
    check (a.is_zero() && b.is_zero());

    b = a.mul (10u);
    check (a.is_zero() && b.is_zero());

    b = a.mul (-10);
    check (a.is_zero() && b.is_zero());

    a = 1llu;
    a <<= 128;
    --a;	// At least two limbs like 0xFF..FF

    b = a;

    c = b*=125u;
    check (b == "42535295865117307932921825928971026431875");
    check (c == b);

    c = b*=-125;
    check (b == "-5316911983139663491615228241121378303984375");
    check (c == b);

    b *= 1u;
    check (c == b);
    b *= -1;
    check (c.neg() == b);

    c = b*=0;
    check (b.is_zero() && c.is_zero());

    b = a;

    c = b*=64u;
    check (b == a<<6);
    check (c == b);

    c = b*=-64;
    check (b == -a<<12);
    check (c == b);
  }

  {	// Mul
    num_t a(100), b(100), c(100);
    c = a*b;
    check (c.is_zero());
    a = 1llu;
    a <<= 256;
    --a;
    c = a*b;
    check (c.is_zero());
    b = a>>128;
    c = a*b;
    check (c == "39402006196394479212279040100143613804963947181228130472524722419237033863643600344381704752381994682191283092455425");
    check (-a*b == -c);
    check (a*(-b) == -c);
    check (-a*(-b) == c);

    b.size = 1;
    b[0] = 10;
    c = a*b;
    check (c == a*10);
    c = a*b.neg();
    check (c == a*(-10));

    a = 19993llu;
    for (size_t i=1; i<=5; ++i)  c = a*=a;
    check (a == "424712371344309495688001249487498185561027566896946736223051911729174087951070184048392022854482933317538743363994093094913995620549779201");
    check (c == a);
  }
  // !!! add div_qr functions !!!
  {	// Div by limb and slimb
    num_t a(30), b(30);

    a /= 10u;
    check (a.is_zero());
    a /= -10;
    check (a.is_zero());

    a = "1075488420943298174695497";

    b = a/=10u;
    check (a == "107548842094329817469549");
    check (b == a);

    b = a/=(-10u);
    check (a == "-10754884209432981746954");
    check (b == a);

    b = a/=1;
    check (a == "-10754884209432981746954");
    check (b == a);

    b = a/=-1;
    check (a == "10754884209432981746954");
    check (b == a);

    b = a/=64;
    check (a == "168045065772390339796");
    check (b == a);

    b = a/=-64;
    check (a == "-2625704152693599059");
    check (b == a);
  }

  { // Mod by limb and slimb
    num_t a(30), b(30);
    a%=10u;
    check (a.is_zero());
    a%=-10;
    check (a.is_zero());

    a = "14966675814359580587845230627";
    b = a%255;
    check (b == 177u);

    b = a%=255;
    check (a == 177u);
    check (b == a);

    a = "14966675814359580587845230627";
    b = a%(-64);
    check (b == 35u);

    a %= -64;
    check (a == 35u);

    a = "-14966675814359580587845230627";
    b = a%11;
    check (b == 2u);

    a %= 11;
    check (a == 2u);

    a = "-14966675814359580587845230627";
    b = a%(-11);
    check (b == 2u);

    a %= -11;
    check (a == 2u);

    b = "14966675814359580587845230627";

    a = b;
    a.mod (11, DM_FLOORED);
    check (a == 9u);
    a = b;
    a.mod (-11, DM_FLOORED);
    check (a == -2);
    a = -b;
    a.mod (11, DM_FLOORED);
    check (a == -9);
    a = -b;
    a.mod (-11, DM_FLOORED);
    check (a == -9);

    a = b;
    a.mod (11, DM_TRUNCATED);
    check (a == 9u);
    a = b;
    a.mod (-11, DM_TRUNCATED);
    check (a == 9u);
    a = -b;
    a.mod (11, DM_TRUNCATED);
    check (a == -9);
    a = -b;
    a.mod (-11, DM_TRUNCATED);
    check (a == -9);
  }

  {	// div, mod
    num_t a(50), b(50), c(50);

    a = "45349668008961920685673151087083693018221";
    b = "388328016259855395064461231";

    c = b/a;
    check (c.is_zero());
    c = a/b;
    check (c == "116781859948563");
    c = -a/b;
    check (c == "-116781859948564");
    c = a/-b;
    check (c == "-116781859948564");
    c = -a/-b;
    check (c == "116781859948563");

    c = a;
    c.mod (b, DM_EUCLIDEAN);
    check (c == "192491368730588944725357168");
    c = a;
    c.mod (-b, DM_EUCLIDEAN);
    check (c == "192491368730588944725357168");
    c = -a;
    c.mod (b, DM_EUCLIDEAN);
    check (c == "195836647529266450339104063");
    c = -a;
    c.mod (-b, DM_EUCLIDEAN);
    check (c == "195836647529266450339104063");

    c = a;
    c.mod (b, DM_TRUNCATED);
    check (c == "192491368730588944725357168");
    c = a;
    c.mod (-b, DM_TRUNCATED);
    check (c == "192491368730588944725357168");
    c = -a;
    c.mod (b, DM_TRUNCATED);
    check (c == "-192491368730588944725357168");
    c = -a;
    c.mod (-b, DM_TRUNCATED);
    check (c == "-192491368730588944725357168");

    c = a;
    c.mod (b, DM_FLOORED);
    check (c == "192491368730588944725357168");
    c = a;
    c.mod (-b, DM_FLOORED);
    check (c == "-195836647529266450339104063");
    c = -a;
    c.mod (b, DM_FLOORED);
    check (c == "195836647529266450339104063");
    c = -a;
    c.mod (-b, DM_FLOORED);
    check (c == "-192491368730588944725357168");

    c = a;
    a = (c -= a%b);
    c.mod (b, DM_EUCLIDEAN);
    check (c.is_zero());
    c = a;
    c.mod (b, DM_TRUNCATED);
    check (c.is_zero());
    c = a;
    c.mod (b, DM_FLOORED);
    check (c.is_zero());

    //char str[200];
    //printf ("%s\n", c.to_string(str));
  }

  {	// Shift
    num_t a(100), b(100), c(100);
    a <<= 0;
    check (a.is_zero());
    a >>= 0;
    check (a.is_zero());
    a <<= LIMB_BITS;
    check (a.is_zero());
    a >>= LIMB_BITS;
    check (a.is_zero());
    a <<= LIMB_BITS-1;
    check (a.is_zero());
    a >>= LIMB_BITS-1;
    check (a.is_zero());
    c = a = "97489136981438262577827";
    b = a>>=0;
    check (a==b && a==c);
    b = a<<=0;
    check (a==b && a==c);
    b = a>>=1;
    check (a=="48744568490719131288913");
    check (a==b);
    b = a<<=1;
    check (a=="97489136981438262577826");
    check (a==b);

    a = c;
    b = a>>=8;
    check (a=="380816941333743213194");
    check (a==b);
    b = a<<=8;
    check (a=="97489136981438262577664");
    check (a==b);

    a = c;
    b = a>>=16;
    check (a=="1487566177084934426");
    check (a==b);
    b = a<<=16;
    check (a=="97489136981438262542336");
    check (a==b);

    a = c;
    b = a>>=32;
    check (a=="22698458512648");
    check (a==b);
    b = a<<=32;
    check (a=="97489136981435962359808");
    check (a==b);

    a = c;
    b = a>>=64;
    check (a==5284u);
    check (a==b);
    b = a<<=64;
    check (a=="97472595685481270738944");
    check (a==b);

    a = c;
    b = a>>=100;
    check (a.is_zero());
    check (a==b);
    a = c;
    b = a<<=100;
    check (a=="123582163010252289805443712860838498128686019564797952");
    check (a==b);

    a = c << 100;
    check (a == b);
    check ((c>>65) == 2642u);
  }

  return ok;
}



bool test_int() {
  bool ok = true;

  Int a, b, c;
  Int d(a);

  check (d == 0);
  check (d.is_zero());

  a = 123_dec;
  check (a == 123);

  ++a;
  check (a[0] == 124);
  --a;
  check (a[0] == 123);

  a.set_zero();
  check (a.is_zero());

  a = 123456789_dec;
  check (!a.is_zero());
  a.clear();
  check (a.is_zero());

  a = 1llu;
  b = a <<= 1024;
  check (a.allocated_limbs() == a.size());
  c = a >>= 512;
  check (a.size() < a.allocated_limbs());
  a.shrink();
  check (a.allocated_limbs() == a.size());
  check (a == c);
  a.clear();
  check (a.is_zero() && a.allocated_limbs() == 0);
  a.resize (20);
  check (a.allocated_limbs()==20 && a.is_zero());



  a.clear();
  b.clear();
  ++a;  ++b;
  for (size_t i=0; i<100; ++i)  { a += a + a;  b += b; }
  check (a == 515377520732011331036461129765621272702107522001_dec);
  check (a == 0x5a4653ca673768565b41f775d6947d55cf3813d1_hex);
  check (a == 0101101001000110010100111100101001100111001101110110100001010110010110110100000111110111011101011101011010010100011111010101010111001111001110000001001111010001_bin);
  check (a != 1267650600228229401496703205376_dec);
  check (b == 1267650600228229401496703205376_dec);
  check (b == 0x10000000000000000000000000_hex);
  check (b == 010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000_bin);
  check (b < 1267650600228229401496703205378_dec);
  check (b > 1267650600228229401496703205375_dec);
  --a;  --b;
  check (a == 515377520732011331036461129765621272702107522000_dec);
  check (b == 1267650600228229401496703205375_dec);

  c = a - b;
  check (c == 515377520732011329768810529537391871205404316625_dec);
  c = b - a;
  check (c == -515377520732011329768810529537391871205404316625_dec);
  b /= 53;

  check (a == a+b-b);
  check (a == a-b+b);
  check (b == -a+b+a);
  check (-b == -a-b+a);
  check (a<<1 == a+a);
  check (a-a*2 == -a);

  c = a;
  c = c - c;
  check (c.is_zero());
  b = a - a;
  check (b.is_zero());
  a -= a;
  check (a.is_zero());

  a = 0xFF_hex;
  for (size_t i=0; i<5; ++i)  a *= a;
  check (a == 0xE1DD29730112F6EF1D8EDABFD4C3C60C823D865CD592ABCDF0BDEC64A1EFE001_hex);

  check (0xFF_hex << 16 == 0xFF0000_hex);
  check (0xFF00_hex + 0xFF_hex == 0xFFFF_hex);
  check (0xFF00_hex - 0xFF_hex == 0xFE01_hex);

  a.clear();
  a = 1LLU;
  b.clear();
  c.clear();
  b = 1LLU;
  c = 1LLU;
  for (limb_t s=1; s<=100; ++s)  a *= s;
  check (a == 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000_dec);
  check (a % 100000000000000000000_dec == 0);
  for (limb_t s=1; s<=78; ++s)  b *= s;
  for (limb_t s=79; s<=100; ++s)  c *= s;
  for (limb_t s=1; s<=100; ++s)  check ((a%s).is_zero());
  check (a / b == c);
  check (b / a == 0);
  check ((a%b).is_zero());
  check (b % a == b);
  check (a % 0x80U == 0);
  check (a % -128 == 0);

  return ok;
}

*/

bool unit_tests() {
	bool ok = true;

	// !!! test misc and other files

	ok = test_misc() && ok;
	ok = test_low_level() && ok;
	ok = test_high_level() && ok;
	ok = test_vec_t() && ok;
	//ok = test_num_t() && ok;
	//ok = test_int() && ok;

	return ok;
}
