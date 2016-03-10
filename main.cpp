/**
 * Беседы о программировании 014.
 * Умножение вектора на лимб со сложением или вычитанием.
 */
#include "stdio.h"
#include "mini-gmp.h"
#include "Int.cpp"

#define DEBUG

const size_t N = 1024*1024;
const size_t T = 1024*1;

num_t a ( N + 1 ), b ( N ), c ( N + 1 ), d ( N + 1 );
mpz_t A, B, C, D;

void Copy ( mpz_t & A, num_t & n ) {
  assert ( A [ 0 ] . _mp_alloc >= abs ( n . size ) );
  for ( size_t i = 0; i < abs ( n . size ); i ++ )
    A [ 0 ] . _mp_d [ i ] = n [ i ];
  A [ 0 ] . _mp_size = n . size;
}

bool isEqual ( mpz_t & A, num_t & n ) {
  if ( A [ 0 ] . _mp_size != n . size )  return false;
  for ( size_t i = 0; i < abs ( n . size ); i ++ )
    if ( A [ 0 ] . _mp_d [ i ] != n [ i ] )
      return false;
  return true;
}

// Адаптировано из mini-gmp
void gmp_umul_ppmm ( limb_t & w1, limb_t & w0, limb_t u, limb_t v ) {
    limb_t __x0, __x1, __x2, __x3;         
    unsigned __ul, __vl, __uh, __vh;          
    limb_t __u = (u), __v = (v);         
    __ul = __u & 0xFFFF;          
    __uh = __u >> 16;
    __vl = __v & 0xFFFF;
    __vh = __v >> 16;
    __x0 = (limb_t) __ul * __vl;
    __x1 = (limb_t) __ul * __vh;
    __x2 = (limb_t) __uh * __vl;
    __x3 = (limb_t) __uh * __vh;
    __x1 += __x0 >> 16;/* this can't give carry */ 
    __x1 += __x2;   /* but this indeed can */   
    if (__x1 < __x2)    /* did we get it? */      
      __x3 += 0x10000;  /* yes, add it in the proper pos. */
    w1 = __x3 + (__x1 >> 16); 
    w0 = (__x1 << 16) + (__x0 & 0xFFFF);
}

// Адаптировано из mini-gmp
limb_t mpn_addmul_1 (limb_t * rp, const limb_t * up, size_t n, limb_t vl) {
  limb_t ul, cl, hpl, lpl, rl;

  cl = 0;
  do
    {
      ul = *up++;
      gmp_umul_ppmm (hpl, lpl, ul, vl);

      lpl += cl;
      cl = (lpl < cl) + hpl;

      rl = *rp;
      lpl = rl + lpl;
      cl += lpl < rl;
      *rp++ = lpl;
    }
  while (--n != 0);

  return cl;
}

// Адаптировано из mini-gmp
limb_t mpn_submul_1 (limb_t * rp, const limb_t * up, size_t n, limb_t vl)
{
  limb_t ul, cl, hpl, lpl, rl;
  cl = 0;
  do
    {
      ul = *up++;
      gmp_umul_ppmm (hpl, lpl, ul, vl);

      lpl += cl;
      cl = (lpl < cl) + hpl;

      rl = *rp;
      lpl = rl - lpl;
      cl += lpl > rl;
      *rp++ = lpl;
    }
  while (--n != 0);

  return cl;
}

int main ( ) {

  mpz_init2 ( A, ( N + 1 ) * LIMB_BITS );
  mpz_init2 ( B, ( N + 1 ) * LIMB_BITS );
  mpz_init2 ( C, ( N + 1 ) * LIMB_BITS );
  mpz_init2 ( D, ( N + 1 ) * LIMB_BITS );

#ifdef DEBUG
  for ( size_t test = 0; test < T; test ++ ) {
    a . size = N;
    b . size = N;
    rnd ( a );
    rnd ( b );
    a . size = size_t ( rnd ( ) % N );
    b . size = size_t ( rnd ( ) % N );
    if ( rnd ( ) % 10 == 0 )  a = b;
    if ( rnd ( ) % 10 == 0 )  a . size = 0;
    if ( rnd ( ) % 10 == 0 )  b . size = 0;    
    if ( rnd ( ) % 5 == 0 ) {
      for ( size_t i = 0; i < abs ( a . size ); i ++ )
        a [ i ] = LIMB_T_MAX;
      if ( rnd ( ) % 5 == 0 )
        for ( size_t i = 0; i < abs ( b . size ); i ++ )
          b [ i ] = LIMB_T_MAX;
    }
    //if ( rnd ( ) % 2 )  a . size = - a . size;
    //if ( rnd ( ) % 2 )  b . size = - b . size;
    
    Copy ( A, a );
    Copy ( B, b );

    limb_t r = rnd ( );
    if ( rnd ( ) % 5 )
      r = 0;
    if ( rnd ( ) % 5 )
      r = LIMB_T_MAX;
    
    mpz_addmul_ui ( A, B, r );

    d = a;
    vec_t cc ( c ), aa ( a ), bb ( b ), dd ( d );
    addmul ( cc, aa, bb, r );
    c . size = cc . size;
    if ( ! isEqual ( A, c ) ) {
      fprintf ( stderr, "ERROR in 'c=a+b*r'\n" );
      return 1;
    }
    
    dd . addmul ( bb, r );
    d . size = dd . size;
    if ( ! isEqual ( A, d ) ) {
      fprintf ( stderr, "ERROR in 'c+=b*r'\n" );
      return 1;
    }

    mpz_submul_ui ( A, B, r );
    submul ( aa, cc, bb, r );
    a . size = aa . size;
    if ( ! isEqual ( A, a ) ) {
      fprintf ( stderr, "ERROR in 'c=a-b*r'\n" );
      return 1;
    }

    dd . submul ( bb, r );
    d . size = dd . size;
    if ( ! isEqual ( A, d ) ) {
      fprintf ( stderr, "ERROR in 'c-=b*r'\n" );
      return 1;
    }

    cc . FreeAlias ( );
    aa . FreeAlias ( );
    bb . FreeAlias ( );
    dd . FreeAlias ( );

  }
  fprintf ( stderr, "OK\n" );
  return 0;
#endif

  size_t k = 0;
  for ( size_t test = 0; test < T; test ++ ) {
    a . size = N;
    b . size = N;
    rnd ( a );
    rnd ( b );
    a . size = size_t ( rnd ( ) % ( N / 1 ) );
    b . size = size_t ( rnd ( ) % ( N / 1 ) );
    //if ( rnd ( ) % 2 )  a . size = - a . size;
    //if ( rnd ( ) % 2 )  b . size = - b . size;
    Copy ( A, a );
    Copy ( B, b );

    limb_t r = rnd ( );
        
    //mpn_addmul_1 ( a . limbs, b . limbs, b . size, r );    
    //addmul ( c . limbs, a . limbs, a . size, b . limbs, b . size, r );
    //addmul ( a . limbs, a . size, b . limbs, b . size, r );

    //mpn_submul_1 ( a . limbs, b . limbs, b . size, r );    
    //submul ( c . limbs, a . limbs, a . size, b . limbs, b . size, r );
    //submul ( a . limbs, a . size, b . limbs, b . size, r );           

  }

  fprintf ( stderr, "%u\n", k );

  return 0;
}
