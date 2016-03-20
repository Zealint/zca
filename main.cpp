/**
 * Беседы о программировании 015.
 * Умножение вектора на вектор.
 */
#include "stdio.h"
#include "mini-gmp.h"
#include "Int.cpp"

#define DEBUG

const size_t N = 1024*4;
const size_t T = 1024*1;

num_t a ( N ), b ( N ), c ( 2 * N );
mpz_t A, B, C;

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

int main ( ) {

  mpz_init2 ( A, ( N ) * LIMB_BITS );
  mpz_init2 ( B, ( N ) * LIMB_BITS );
  mpz_init2 ( C, ( 2 * N ) * LIMB_BITS );
  
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
    if ( rnd ( ) % 2 )  a . size = - a . size;
    if ( rnd ( ) % 2 )  b . size = - b . size;
    
    Copy ( A, a );
    Copy ( B, b );

    /*
    limb_t r = rnd ( );
    if ( rnd ( ) % 5 )
      r = 0;
    if ( rnd ( ) % 5 )
      r = LIMB_T_MAX;
    */

    mpz_mul ( C, A, B );
    mul ( c, a, b );    
    if ( ! isEqual ( C, c ) ) {
      fprintf ( stderr, "ERROR in 'c=a*b'\n" );
      return 1;
    }    

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
            
    //mpz_mul ( C, A, B );

    mul ( c, a, b );

  }

  fprintf ( stderr, "%u\n", k );

  return 0;
}
