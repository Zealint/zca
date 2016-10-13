#include "test.h"
#include "check.h"
#include "vec_t.h"



bool test_vec_t() {
  bool ok = true;

  { // Constructors and destructor, also operator [] and IsZero()
    vec_t a;
    check (a.size==0 && a.limbs==nullptr);
    check (a.IsZero());
    vec_t b(3);
    check (b.size==0 && b.limbs!=nullptr);
    check (b.IsZero());
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
    a.Adapt(2);
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.Adapt(4);
    check (a.size==3 && a[0]==1 && a[1]==2 && a[2]==3);
    a.size = 4;
    a[3] = 4;
    check (a.size==4 && a[0]==1 && a[1]==2 && a[2]==3 && a[3]==4);
    a.Resize(2);
    check (a.IsZero());
	}

	{ // Normalize
    vec_t a(10);
    a.size = 3;
    a[0] = a[1] = a[2] = 0;
    check (a.Normalize()==0);
    check (a.size == 0);
    check (a.IsZero());
    a.size = 3;
    a[0] = 1;
    a[1] = a[2] = 0;
    check (a.Normalize()==1);
    check (a.size == 1);
    check (!a.IsZero());
    a.size = 3;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    check (a.Normalize()==3);
    check (a.size == 3);
    check (!a.IsZero());
    a.size = 10;
    a[9] = a[8] = a[7] = 0;
    a[6] = LIMB_MAX;
    check (a.Normalize()==7);
    check (a.size == 7);
	}

	{ // Operator =; also SetZero()
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
    a.SetZero();
    check (a.IsZero());
    b = a;
    check (b.IsZero());
  }

	{ // Compare
    vec_t a(10), b(10);
    check (b.IsZero());
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

    b.SetZero();
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

    a.SetZero();
    b.SetZero();
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

    a.SetZero();
    Inc (b, a, 10);
    check (b.size == 1 && b[0] == 10);
    Dec (c, b, 5);
    check (c.size == 1 && c[0] == 5);

    a.size = 1;
    a[0] = LIMB_MAX-10;
    a.Inc(11);
    check (a.size == 2 && a[0] == 0 && a[1] == 1);
    a.Dec(LIMB_MAX);
    check (a.size == 1 && a[0] == 1);

    Inc (c, a, 0);
    check (c.size == 1 && c[0] == 1);
    Inc (c, c, 0);
    check (c.size == 1 && c[0] == 1);
    c.Inc(0);
    check (c.size == 1 && c[0] == 1);

    Dec (a, c, 0);
    check (a.size == 1 && a[0] == 1);
    Dec (a, a, 0);
    check (a.size == 1 && a[0] == 1);
    c.Dec(0);
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
    check (a.IsZero());
    a.Resize(2);
    a.size = 1;
    a[0] = LIMB_MAX-1;
    a = a + a;
    check (a.size == 2 && a[1] == 1 && a[0] == LIMB_MAX-3);
    a = a - a;
    check (a.IsZero());

    a.Adapt(3);
    a.size = 2;
    a[0] = a[1] = 1;
    b.Adapt(3);
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
    check (a.IsZero());
    a *= 10;
    check (a.IsZero());
    a *= 1;
    check (a.IsZero());

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

  { // AddMul1, SubMul1.
    vec_t a(4), b(4), c(4), d(4);
    a.size = 2;
    a[0] = 1;
    a[1] = 2;
    b.size = 2;
    b[0] = LIMB_MAX;
    b[1] = LIMB_MAX-2;
    AddMul1 (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    SubMul1 (c, a, b, 0);
    check (c.size==2 && c[0]==1 && c[1]==2);
    AddMul1 (c, a, b, 1);
    check (c.size==3 && c[0]==0 && c[1]==0 && c[2]==1);
    SubMul1 (d, c, b, 1);
    check (d.size==2 && d[0]==1 && d[1]==2);
    AddMul1 (c, a, a, 3);
    check (c.size==2 && c[0]==4 && c[1]==8);
    SubMul1 (d, c, a, 3);
    check (d.size==2 && d[0]==1 && d[1]==2);
    AddMul1 (a, b, b, 3);
    check (a.size==3 && a[0]==LIMB_MAX-3 && a[1]==LIMB_MAX-8 && a[2]==3);
    SubMul1 (d, a, b, 3);
    check (d.size==2 && d[0]==LIMB_MAX && d[1]==LIMB_MAX-2);

    a.size = 1;
    a[0] = 2;
    AddMul1 (a, a, a, LIMB_MAX);
    check (a.size==2 && a[0]==0 && a[1]==2);
    SubMul1 (a, a, a, 1);
    check (a.IsZero());

    a.size = 1;
    a[0] = 2;
    b.size = 2;
    b[0] = 1;
    b[1] = 2;
    b.AddMul1 (a, 0);
    check (b.size==2 && b[0]==1 && b[1]==2);
    b.AddMul1 (a, 5);
    check (b.size==2 && b[0]==11 && b[1]==2);
    b.AddMul1 (b, 5);
    check (b.size==2 && b[0]==66 && b[1]==12);
    b.SubMul1 (a, 0);
    check (b.size==2 && b[0]==66 && b[1]==12);
    b.SubMul1 (a, 1);
    check (b.size==2 && b[0]==64 && b[1]==12);
    b.SubMul1 (a, LIMB_MAX);
    check (b.size==2 && b[0]==66 && b[1]==10);

    a.SetZero();
    a.AddMul1 (a, 10);
    check (a.IsZero());
    a.SubMul1 (a, 10);
    check (a.IsZero());
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
    c.SetZero();
    b = a * c;
    check (b.IsZero());
    a *= c;
    check (a.IsZero());
  }

  { // Shift left and right
    vec_t a(10), b(10), c(10);
    a.SetZero();
    a <<= 0;
    check (a.IsZero());
    a <<= 1000;
    check (a.IsZero());
    a >>= 0;
    check (a.IsZero());
    a >>= 1000;
    check (a.IsZero());
    c = a << 0;
    check (c.IsZero());
    c = a << 1000;
    check (c.IsZero());
    c = a >> 0;
    check (c.IsZero());
    c = a >> 1000;
    check (c.IsZero());

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
    a.SetZero();
    a /= 1;
    check (a.IsZero());
    b = a/1;
    check (b.IsZero());
    a /= 123;
    check (a.IsZero());
    b = a/456;
    check (b.IsZero());

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
    a.SetZero();
    a %= 1;
    check (a.IsZero());
    b = a%1;
    check (b.IsZero());
    a %= 123;
    check (a.IsZero());
    b = a%456;
    check (b.IsZero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    b = a%3;
    check (b.IsZero());
    a %= 3;
    check (a.IsZero());

    a.size = 3;
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a *= LIMB_MAX;
    b = a%LIMB_MAX;
    check (b.IsZero());
    a %= LIMB_MAX;
    check (a.IsZero());

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
    check (b.IsZero());
    a %= LIMB_MAX;
    check (a.IsZero());

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
    a.SetZero();
    b.size = 1;
    b[0] = 1;
    c = a/b;
    check (c.IsZero());
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
    check (c.IsZero());
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

	return ok;
}



bool unit_tests() {
	bool ok = true;

	ok = test_vec_t() && ok;

	return ok;
}
