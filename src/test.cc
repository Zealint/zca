#include "test.h"
#include "check.h"
#include "vec_t.h"
#include "num_t.h"

/*
#if LIMB_BITS == 8
#elif LIMB_BITS == 16
#elif LIMB_BITS == 32
#elif LIMB_BITS == 64
#endif
*/

bool test_vec_t() {
  bool ok = true;

  { // Constructors, also operator [] and set_/is_zero()
    vec_t a;
    check (a.size==0 && a.limbs==nullptr);
    check (a.is_zero());
    vec_t b(3);
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
    vec_t d(c+b);
    check (d.size==2 && d[1]==4 && d[0]==2);
    check (c.size==2 && c[1]==2 && c[0]==1);
    check (c==b);
    check (d.limbs!=b.limbs && d.limbs!=c.limbs);
    d.set_zero();
    check (d.is_zero());
  }

  {	// Conversion from u64 and string
    vec_t a(3);
    check (a.is_zero());
    a = 0LLU;
    check (a.is_zero());
    a = 123LLU;
    check (a.size==1 && a[0]==123);
#if LIMB_BITS == 8
    a = 0xA1234LLU;
    check (a.size==3 && a[2]==0xAU && a[1]==0x12U && a[0]==0x34U);
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
	}

	{	// To / From string by back and forth conversion
    vec_t a(3);
    a.size = 3;
    a[0] = 1;
    a[1] = LIMB_MAX;
    a[2] = (limb_t)1 << (LIMB_BITS-1);	// 0x80..00
    char str[LIMB_BITS*3+1];
    vec_t b(3);
    for (u8 base=2; base<=36; ++base) {
      a.to_string (str, base);
      b.from_string (str, base);
      check (a == b);
      vec_t c (str, base);
      check (c == b);
    }
	}

	{ // Resize, Adapt
    vec_t a(3);
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
	}

	{ // Normalize
    vec_t a(10);
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
    vec_t a(3), b(3);
    a.size = 3;
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    b = a;
    check (a==b);
    check (&a != &b);
    check (&a.size != &b.size);
    check (a.limbs != b.limbs);
    b = a+a;
    check (b.size==3 && b[0]==0 && b[1]==2 && b[2]==4);
    a.set_zero();
    check (a.is_zero());
    b = a;
    check (b.is_zero());
  }

  { // Compare with limb_t
    vec_t a(10);
    limb_t b = 0;
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a != b);
    check (b != a);
    check (!(a==b));
    check (!(b==a));
    check (a>b);
    check (!(a<b));
    check (a>=b);
    check (!(a<=b));
    check (b<a);
    check (!(b>a));
    check (b<=a);
    check (!(b>=a));

    b = 128;
    a = (u64)b;
    check (a == b);
    check (b == a);
    check (!(a!=b));
    check (!(b!=a));
    check (a <= b);
    check (a >= b);
    check (!(a>b));
    check (!(a<b));

    b = 129;
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
    b = 0;
    check (a==b);
    check (b==a);
    check (!(a!=b));
    check (!(b!=a));
    check (!(a<b));
    check (!(b<a));
    check (!(a>b));
    check (!(b>a));
    check (a<=b);
    check (b<=a);
    check (a>=b);
    check (b>=a);

    b = 255;
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

	{ // Compare with vec_t
    vec_t a(10), b(10);
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
    vec_t a(3), b(3), c(3);
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

    b = c += LIMB_MAX;
    check (c.size==2 && c[0]==0 && c[1]==1);
    check (b == c);
    b = c -= 2;
    check (c.size==1 && c[0]==LIMB_MAX-1);
    check (b == c);
    a = c + 3;
    check (a.size==2 && a[0]==1 && a[1]==1);
    c = a - 2;
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
    check (c == a);
    a = a + 0;
    check (a == c);

    b = a -= 0;
    check (a == c);
    check (b == a);
    c = a - 0;
    check (c == a);
    a = a - 0;
    check (a == c);

    a.size = 1;
    a[0] = 10;
    c = a;
    b = a - 10;
    check (b.is_zero());
    a -= 10;
    check (a.is_zero());
    c = c - 10;
    check (c.is_zero());
  }

  { // Add, Sub.
    vec_t a(4), b(4), c(4);
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
    check (b.size==2 && b[1]==LIMB_MAX && b[0]==1);
    a.size = 1;
    a[0] = 1;
    a = a + a;
    check (a.size == 1 && a[0] == 2);
    a = a - a;
    check (a.is_zero());
    a.size = 1;
    a[0] = LIMB_MAX-1;
    a = a + a;
    check (a.size == 2 && a[1] == 1 && a[0] == LIMB_MAX-3);
    a = a - a;
    check (a.is_zero());
    a.size = 2;
    a[0] = a[1] = 1;
    b.size = 2;
    b[0] = b[1] = LIMB_MAX;
    b = b + a;
    check (b.size==3 && b[0]==0 && b[1]==1 && b[2]==1);
    b = b - a;
    check (b.size==2 && b[0]==LIMB_MAX && b[1]==LIMB_MAX);
    b = a + b;
    check (b.size==3 && b[0]==0 && b[1]==1 && b[2]==1);
    a = b - a;
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
  }

  { // Mul by limb
    vec_t a(3), b(3);
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
    check (b.size==2 && b[0]==9 && b[1]==18);
    a = a * 3;
    check (a.size==2 && a[0]==9 && a[1]==18);
    a.size = 1;
    a[0]=LIMB_MAX;
    b = a * LIMB_MAX;
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
  }

  { // addmul, submul.
    vec_t a(4), b(4), c(4), d(4);
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
    vec_t a(10), b(10), c(10), d(10);
    a.size = 3;
    a[0] = 3;
    a[1] = 2;
    a[2] = 1;
    b.size = 3;
    b[0] = 6;
    b[1] = 5;
    b[2] = 4;
    c = a * b;
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
    check (b.is_zero());
    d = a *= c;
    check (a.is_zero());
    check (d == a);
    vec_t z(100);
    z = 1031llu;
    for (size_t i=1; i<=5; ++i)  z *= z;
    check (z == "2656301020209296604918669081053362592769335327396991619655420824350472399920137716010779784559361");
  }

  { // Shift left and right
    vec_t a(10), b(10), c(10);
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
    check (c.is_zero());
    c = a << 1000;
    check (c.is_zero());
    c = a >> 0;
    check (c.is_zero());
    c = a >> 1000;
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
    check (b.size==3 && b[0]==20 && b[1] == LIMB_MAX-1 && b[2] == 29);
    c = a <<= 1;
    check (a.size==3 && a[0]==20 && a[1] == LIMB_MAX-1 && a[2] == 29);
    check (c == a);
    b = a >> 1;
    check (b.size==3 && b[0]==10 && b[1] == LIMB_MAX && b[2] == 14);
    c = a >>= 1;
    check (a.size==3 && a[0]==10 && a[1] == LIMB_MAX && a[2] == 14);
    check (c == a);

    const limb_t H = (limb_t)1<<(LIMB_BITS-1);
    a[2] = H;

    b = a << (LIMB_BITS-1);
    check (b.size==4 && b[0]==0 && b[1]==H+5 && b[2]==LIMB_MAX/2 && b[3]==H/2);
    a <<= (LIMB_BITS-1);
    check (a.size==4 && a[0]==0 && a[1]==H+5 && a[2]==LIMB_MAX/2 && a[3]==H/2);
    b = a >> (LIMB_BITS-1);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    a >>= (LIMB_BITS-1);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);

    b = a << (LIMB_BITS-1+LIMB_BITS);
    check (b.size==5 && b[0]==0 && b[1]==0 && b[2]==H+5 && b[3]==LIMB_MAX/2 && b[4]==H/2);
    a <<= (LIMB_BITS-1+LIMB_BITS);
    check (a.size==5 && a[0]==0 && a[1]==0 && a[2]==H+5 && a[3]==LIMB_MAX/2 && a[4]==H/2);
    b = a >> (LIMB_BITS-1+LIMB_BITS);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    a >>= (LIMB_BITS-1+LIMB_BITS);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);

    b = a << (LIMB_BITS-1+3*LIMB_BITS);
    check (b.size==7 && b[0]==0 && b[1]==0 && b[2]==0 && b[3]==0 && b[4]==H+5 && b[5]==LIMB_MAX/2 && b[6]==H/2);
    c = a <<= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==7 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==H+5 && a[5]==LIMB_MAX/2 && a[6]==H/2);
    check (c == a);
    b = a >> (LIMB_BITS-1+3*LIMB_BITS);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    c = a >>= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);
    check (c == a);

    a >>= LIMB_BITS;
    check (a.size==2 && a[0]==LIMB_MAX && a[1]==H);
    a <<= LIMB_BITS;
    check (a.size==3 && a[0]==0 && a[1]==LIMB_MAX && a[2]==H);
  }

  { // Div by limb
    vec_t a(5), b(5);
    a.set_zero();
    b = a /= 1;
    check (a.is_zero());
    check (b == a);
    b = a/1;
    check (b.is_zero());
    b = a /= 123;
    check (a.is_zero());
    check (b == a);
    b = a/123;
    check (b.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    b = a/3;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= 3;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a *= LIMB_MAX;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    b = a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    check (b == a);

    a.size = 3;
    a[0] = a[1] = a[2] = LIMB_MAX;
    a *= LIMB_MAX;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==LIMB_MAX && b[1]==LIMB_MAX && b[2]==LIMB_MAX);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==LIMB_MAX);

    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==LIMB_MAX && b[1]==LIMB_MAX && b[2]==LIMB_MAX);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==LIMB_MAX && a[1]==LIMB_MAX && a[2]==LIMB_MAX);

    a.size = 3;
    a[0] = 128;
    a[1] = (limb_t)(LIMB_MAX<<7);
    a[2] = 127;
    b = a/128;
    check (b.size==2 && b[1]==LIMB_MAX && b[0]==1);
    a /= 128;
    check (a.size==2 && a[1]==LIMB_MAX && a[0]==1);
  }

  { // Mod by limb
    vec_t a(5), b(5);
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
  }

  { // Div, Mod
    vec_t a(100), b(100), c(100);
    a.set_zero();
    b.size = 1;
    b[0] = 1;
    c = a / b;
    check (c.is_zero());
    a.size = 1;
    a[0] = 1;
    c = a / b;
    check (c.size==1 && c[0]==1);
    a = "333231302928272625242322212019181716151413121110987654321";
    b = "12345678910111213141516171819202122";
    c = a / b;
    check (c == "26991735760708423406858");
    a /= b;
    check (a == c);

    a = "333231302928272625242322212019181716151413121110987654321";
    b = "12345678910111213141516171819202122";
    c = a % b;
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
    check (c.size==3 && c[2]==1 && c[1]==0 && c[0]==0);
    c = a % b;
    check (c.size==2 && c[1]==LIMB_MAX && c[0]==LIMB_MAX-1);
  }

	return ok;
}



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



bool unit_tests() {
	bool ok = true;

	ok = test_vec_t() && ok;
	ok = test_num_t() && ok;

	return ok;
}
