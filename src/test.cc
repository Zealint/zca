#include "test.h"
#include "check.h"
#include "vec_t.h"



bool test_vec_t() {
  bool ok = true;

#if LIMB_BITS == 32
  { // Constructors and destructor, also operator [] and is_zero()
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
    check (c.size==2 && c[0]==1 && c[1]==2);
    check (c==b && b==c);
    check (&c != &b);
    vec_t d(c+b);
    check (d.size==2 && d[0]==2 && d[1]==4);
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
    check (a.normalize()==0);
    check (a.size == 0);
    check (a.is_zero());
    a.size = 3;
    a[0] = 1;
    a[1] = a[2] = 0;
    check (a.normalize()==1);
    check (a.size == 1);
    check (!a.is_zero());
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a.normalize()==3);
    check (a.size == 3);
    check (!a.is_zero());
    a.size = 10;
    a[9] = a[8] = a[7] = 0;
    a[6] = LIMB_MAX;
    check (a.normalize()==7);
    check (a.size == 7);
	}

	{ // Operator =; also set_zero()
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

	{ // Compare
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
    ++a;
    check (a.size == 2 && a[0] == 0 && a[1] == 1);
    ++a;
    check (a.size == 2 && a[0] == 1 && a[1] == 1);
    --a;
    check (a.size == 2 && a[0] == 0 && a[1] == 1);
    --a;
    check (a.size == 1 && a[0] == LIMB_MAX);
    --a;
    check (a.size == 1 && a[0] == LIMB_MAX-1);

    a.set_zero();
    inc (b, a, 10);
    check (b.size == 1 && b[0] == 10);
    dec (c, b, 5);
    check (c.size == 1 && c[0] == 5);

    a.size = 1;
    a[0] = LIMB_MAX-10;
    a.inc(11);
    check (a.size == 2 && a[0] == 0 && a[1] == 1);
    a.dec(LIMB_MAX);
    check (a.size == 1 && a[0] == 1);

    inc (c, a, 0);
    check (c.size == 1 && c[0] == 1);
    inc (c, c, 0);
    check (c.size == 1 && c[0] == 1);
    c.inc(0);
    check (c.size == 1 && c[0] == 1);

    dec (a, c, 0);
    check (a.size == 1 && a[0] == 1);
    dec (a, a, 0);
    check (a.size == 1 && a[0] == 1);
    c.dec(0);
    check (c.size == 1 && c[0] == 1);

    c += LIMB_MAX;
    check (c.size == 2 && c[0] == 0 && c[1] == 1);
    c -= 2;
    check (c.size == 1 && c[0] == LIMB_MAX-1);

    a = c + 3;
    check (a.size == 2 && a[0] == 1 && a[1] == 1);
    c = a - 2;
    check (c.size == 1 && c[0] == LIMB_MAX);
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
    a += b;
    check (a.size==4 && a[3]==1 && a[2]==0 && a[1]==LIMB_MAX && a[0]==LIMB_MAX-1);

    c.size = 2;
    c[0] = 1;
    c[1] = LIMB_MAX;
    c += a;
    check (c.size==4 && c[3]==1 && c[2]==1 && c[1]==LIMB_MAX-1 && c[0]==LIMB_MAX);
    c -= a;
    check (c.size==2 && c[1]==LIMB_MAX && c[0]==1);
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
    a.resize(2);
    a.size = 1;
    a[0] = LIMB_MAX-1;
    a = a + a;
    check (a.size == 2 && a[1] == 1 && a[0] == LIMB_MAX-3);
    a = a - a;
    check (a.is_zero());

    a.adapt(3);
    a.size = 2;
    a[0] = a[1] = 1;
    b.adapt(3);
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
  }

  { // Mul1
    vec_t a(3), b(3);
    a.size = 2;
    a[0] = a[1] = 1;
    a *= 1;
    check (a.size==2 && a[0]==1 && a[1]==1);
    a *= 0;
    check (a.is_zero());
    a *= 10;
    check (a.is_zero());
    a *= 1;
    check (a.is_zero());

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
    check (a==b);
  }

  { // addmul, submul.
    vec_t a(4), b(4), c(4), d(4);
    a.size = 2;
    a[0] = 1;
    a[1] = 2;
    b.size = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX-2;
    addmul (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    submul (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    addmul (c, a, b, 1);
    check (c.size==3 && c[0]==0 && c[1]==0 && c[2]==1);
    submul (d, c, b, 1);
    check (d.size==2 && d[0]==1 && d[1]==2);
    addmul (c, a, a, 3);
    check (c.size==2 && c[0]==4 && c[1]==8);
    submul (d, c, a, 3);
    check (d.size==2 && d[0]==1 && d[1]==2);
    addmul (a, b, b, 3);
    check (a.size==3 && a[0]==LIMB_MAX-3 && a[1]==LIMB_MAX-8 && a[2]==3);
    submul (d, a, b, 3);
    check (d.size==2 && d[0]==LIMB_MAX && d[1]==LIMB_MAX-2);

    a.size = 1;
    a[0] = 2;
    addmul (a, a, a, LIMB_MAX);
    check (a.size==2 && a[0]==0 && a[1]==2);
    submul (a, a, a, 1);//???
    check (a.is_zero());

    a.size = 1;
    a[0] = 2;
    b.size = 2;
    b[0] = 1;
    b[1] = 2;
    b.addmul (a, 0);
    check (b.size==2 && b[0]==1 && b[1]==2);
    b.addmul (a, 5);
    check (b.size==2 && b[0]==11 && b[1]==2);
    b.addmul (b, 5);
    check (b.size==2 && b[0]==66 && b[1]==12);
    b.submul (a, 0);
    check (b.size==2 && b[0]==66 && b[1]==12);
    b.submul (a, 1);
    check (b.size==2 && b[0]==64 && b[1]==12);
    b.submul (a, LIMB_MAX);
    check (b.size==2 && b[0]==66 && b[1]==10);

    a.set_zero();
    a.addmul (a, 10);
    check (a.is_zero());
    a.submul (a, 10);
    check (a.is_zero());
  }

  { // Mul
    vec_t a(10), b(10), c(10);
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
    c *= b;
    check (c.size==5 && c[0]==18 && c[1]==27 && c[2]==28 && c[3]==13 && c[4]==4);
    c.size = 1;
    c[0] = 3;
    a *= c;
    check (a.size==3 && a[0]==9 && a[1]==6 && a[2]==3);
    c.set_zero();
    b = a * c;
    check (b.is_zero());
    a *= c;
    check (a.is_zero());
  }

  { // Shift left and right
    vec_t a(10), b(10), c(10);
    a.set_zero();
    a <<= 0;
    check (a.is_zero());
    a <<= 1000;
    check (a.is_zero());
    a >>= 0;
    check (a.is_zero());
    a >>= 1000;
    check (a.is_zero());
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
    a <<= 1;
    check (a.size==3 && a[0]==20 && a[1] == LIMB_MAX-1 && a[2] == 29);
    b = a >> 1;
    check (b.size==3 && b[0]==10 && b[1] == LIMB_MAX && b[2] == 14);
    a >>= 1;
    check (a.size==3 && a[0]==10 && a[1] == LIMB_MAX && a[2] == 14);

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
    a <<= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==7 && a[0]==0 && a[1]==0 && a[2]==0 && a[3]==0 && a[4]==H+5 && a[5]==LIMB_MAX/2 && a[6]==H/2);
    b = a >> (LIMB_BITS-1+3*LIMB_BITS);
    check (b.size==3 && b[0]==10 && b[1]==LIMB_MAX && b[2]==H);
    a >>= (LIMB_BITS-1+3*LIMB_BITS);
    check (a.size==3 && a[0]==10 && a[1]==LIMB_MAX && a[2]==H);
  }

  { // Div1
    vec_t a(5), b(5);
    a.set_zero();
    a /= 1;
    check (a.is_zero());
    b = a/1;
    check (b.is_zero());
    a /= 123;
    check (a.is_zero());
    b = a/456;
    check (b.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    b = a/3;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    a /= 3;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);

    a *= LIMB_MAX;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);

    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a/LIMB_MAX;
    check (b.size==3 && b[0]==1 && b[1]==2 && b[2]==3);
    a /= LIMB_MAX;
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);

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
  }

  { // Mod1
    vec_t a(5), b(5);
    a.set_zero();
    a %= 1;
    check (a.is_zero());
    b = a%1;
    check (b.is_zero());
    a %= 123;
    check (a.is_zero());
    b = a%456;
    check (b.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    b = a%3;
    check (b.is_zero());
    a %= 3;
    check (a.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a *= LIMB_MAX;
    b = a%LIMB_MAX;
    check (b.is_zero());
    a %= LIMB_MAX;
    check (a.is_zero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a *= LIMB_MAX;
    a += LIMB_MAX-1;
    b = a%LIMB_MAX;
    check (b.size==1 && b[0]==LIMB_MAX-1);
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
    vec_t a(10), b(10), c(10);
    a.set_zero();
    b.size = 1;
    b[0] = 1;
    c = a/b;
    check (c.is_zero());
    a.size = 1;
    a[0] = 1;
    c = a/b;
    check (c.size==1 && c[0]==1);
    a.size = 2;
    a[0] = 123;
    a[1] = 456;
    c = a/b;
    check (c.size==2 && c[0]==123 && c[1]==456);
    b = a;
    c = a/b;
    check (c.size==1 && c[0]==1);
    a.size = 3;
    a[0] = 123456789;
    a[1] = 987654321;
    a[2] = 147;
    b.size = 4;
    b[0] = b[1] = b[2] = 0;
    b[3] = 1;
    c = a/b;
    check (c.is_zero());
    b.size = 2;
    b[0] = 258;
    b[1] = 369;
    c = a/b;
    check (c.size==1 && c[0]==1713679801);
    c = a%b;
    check (c.size==2 && c[0]==375699619 && c[1]==161);
    b.size = 3;
    b[0] = 258;
    b[1] = 369;
    b[2] = 111;
    c = a/b;
    check (c.size==1 && c[0]==1);
    c = a%b;
    check (c.size==3 && c[0]==123456531 && c[1]==987653952 && c[2]==36);
  }

#endif

	return ok;
}



bool unit_tests() {
	bool ok = true;

	ok = test_vec_t() && ok;

	return ok;
}
