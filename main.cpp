/**
 * ������ � ���������������� 025.
 * ������� m/n.
 */
#include "stdio.h"
#include "mini-gmp.h"
#include "Int.cpp"

#define DEBUG

const size_t N = 1024*3;
const size_t T = 1024*102;

//num_t a ( N ), b ( N ), c ( N );
vec_t a ( N ), b ( N ), c ( N ), d ( N );
mpz_t A, B, C, D;

void Copy ( mpz_t & A, num_t & n ) {
  assert ( A [ 0 ] . _mp_alloc >= abs ( n . size ) );
  for ( size_t i = 0; i < abs ( n . size ); i ++ )
    A [ 0 ] . _mp_d [ i ] = n [ i ];
  A [ 0 ] . _mp_size = n . size;
}

void Copy ( mpz_t & A, vec_t & n ) {
  assert ( A [ 0 ] . _mp_alloc >= n . size );
  for ( size_t i = 0; i < n . size; i ++ )
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

bool isEqual ( mpz_t & A, slimb_t b ) {
  if ( b == 0 )  return A [ 0 ] . _mp_size == 0;
  if ( A [ 0 ] . _mp_size != ( b > 0 ? 1 : -1 ) )  return false;
  if ( A [ 0 ] . _mp_d [ 0 ] != abs ( b ) )  return false;
  return true;
}

bool isEqual ( mpz_t & A, vec_t & n ) {
  if ( A [ 0 ] . _mp_size < 0 || A [ 0 ] . _mp_size != n . size )  return false;
  for ( size_t i = 0; i < n . size; i ++ )
    if ( A [ 0 ] . _mp_d [ i ] != n [ i ] )
      return false;
  return true;
}

int main ( ) {

  mpz_init2 ( A, ( N ) * LIMB_BITS );
  mpz_init2 ( B, ( N ) * LIMB_BITS );
  mpz_init2 ( C, ( N ) * LIMB_BITS );
  mpz_init2 ( D, ( N ) * LIMB_BITS );

#ifdef DEBUG
  for ( size_t test = 0; test < T; test ++ ) {
    fprintf ( stderr, "%u\n", test );
    a . size = N;
    b . size = N;
    rnd ( a );
    rnd ( b );
    a . size = size_t ( rnd ( ) % N );
    b . size = size_t ( rnd ( ) % ( N - 1 ) ) + 1;
    if ( rnd ( ) % 20 == 0 )  a . size = 0;
    if ( rnd ( ) % 10 == 0 )
      for ( size_t i = 0; i < abs ( a . size ); i ++ )
        a [ i ] = LIMB_T_MAX;
    if ( rnd ( ) % 10 == 0 )
      for ( size_t i = 0; i < abs ( b . size ); i ++ )
        b [ i ] = LIMB_T_MAX;
    if ( rnd ( ) % 10 == 0 )  b . size = 1;
    if ( rnd ( ) % 10 == 0 )  b . size = 2;
    if ( rnd ( ) % 5 == 0 )  a = b;
    if ( rnd ( ) % 2 ) {
      b [ 0 ] ++;
      if ( rnd ( ) % 4 == 0 )
        a [ 0 ] ++;
    }
    Copy ( A, a );
    Copy ( B, b );
    if ( test == 96 )
      test = test;
    mpz_tdiv_qr ( C, D, A, B );
    a . div_qr ( & d, b );
    if ( ! isEqual ( C, a ) || ! isEqual ( D, d ) ) {
      fprintf ( stderr, "ERROR in div_qr in line %u\n", test );
      return 1;
    }

    //fprintf ( stderr, "%u\n", test );

  }
  fprintf ( stderr, "OK\n" );
  return 0;
#endif

  size_t k = 0;
  limb_t d1 = rnd ( ) | 0x80000000, d0 = rnd ( ), v, q, r1, r0;
  v = inv_3_by_2 ( d1, d0 );
  //limb_t d = rnd ( ) | 0x80000000, v, q, r;
  //v = inv_2_by_1 ( d );
  for ( size_t test = 0; test < T; test ++ ) {
    /*
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
    */

    limb_t u2 = rnd ( ), u1 = rnd ( ), u0 = rnd ( );
    while ( u2 >= d1 )  u2 >>= 1;

    //q = test;
    //div_3_by_2 ( q, r1, r0, u2, u1, u0, d1, d0 );
    //div_3_by_2_pre ( q, r1, r0, u2, u1, u0, d1, d0, v );
    k += q;

    /*
    limb_t u1 = rnd ( ), u0 = rnd ( );
    while ( u1 >= d )  u1 >>= 1;
    q = test;
    */
    //q = U / d;
    //div_2_by_1 ( q, u1, u0, d );
    //div_2_by_1_pre ( q, u1, u0, d, v );

  }

  fprintf ( stderr, "%u\n", k );

  return 0;
}
