/**
 * Беседы о программировании 022.
 * Деление 2/1, часть II.
 */
#include "stdio.h"
#include "mini-gmp.h"
#include "Int.cpp"

//#define DEBUG

const size_t N = 1024*4;
const size_t T = 1024*1024*1024;

num_t a ( N ), b ( N ), c ( 2 * N );
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

int main ( ) {

  mpz_init2 ( A, ( N ) * LIMB_BITS );
  mpz_init2 ( B, ( N ) * LIMB_BITS );
  mpz_init2 ( C, ( 2 * N ) * LIMB_BITS );
  mpz_init2 ( D, ( 2 * N ) * LIMB_BITS );
  
#ifdef DEBUG
  for ( size_t test = 0; test < T; test ++ ) {
    /*a . size = N;
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
    
    mpz_mul ( C, A, B );
    mul ( c, a, b );    
    if ( ! isEqual ( C, c ) ) {
      fprintf ( stderr, "ERROR in 'c=a*b'\n" );
      return 1;
    }    
    */

    limb_t u1 = rnd ( ), 
           u0 = rnd ( ), 
           d = rnd ( ) | 0x80000000,            
           r, q;
    while ( u1 >= d ) u1 >>= 1;
    A [ 0 ] . _mp_size = 2;
    A [ 0 ] . _mp_d [ 0 ] = u0;
    A [ 0 ] . _mp_d [ 1 ] = u1;
    B [ 0 ] . _mp_size = 1;
    B [ 0 ] . _mp_d [ 0 ] = d;
    //void mpz_fdiv_qr ( mpz_t, mpz_t, const mpz_t, const mpz_t );
    mpz_fdiv_qr ( C, D, A, B );
    r = div_2_by_1 ( q, u1, u0, d );
    if ( C [ 0 ] . _mp_size != 1 || D [ 0 ] . _mp_size != 1 ||
         C [ 0 ] . _mp_d [ 0 ] != q || D [ 0 ] . _mp_d [ 0 ] != r ) {
      fprintf ( stderr, "ERROR in 'div'\n" );
      return 1;
    }
        
    r = div_2_by_1_pre ( q, u1, u0, d, inv_2_by_1 ( d ) );
    if ( C [ 0 ] . _mp_size != 1 || D [ 0 ] . _mp_size != 1 ||
         C [ 0 ] . _mp_d [ 0 ] != q || D [ 0 ] . _mp_d [ 0 ] != r ) {
      fprintf ( stderr, "ERROR in 'div_pre'\n" );
      return 1;
    } 

    /*
    limb_t u2 = rnd ( ), 
           u1 = rnd ( ), 
           u0 = rnd ( ), 
           d1 = rnd ( ) | 0x80000000, 
           d0 = rnd ( ),            
           r1, r0, q;
    while ( u2 > d1 ) u2 >>= 1;
    while ( u2 == d1 && u1 >= d0 ) u1 >>= 1;
    A [ 0 ] . _mp_size = 3;
    A [ 0 ] . _mp_d [ 0 ] = u0;
    A [ 0 ] . _mp_d [ 1 ] = u1;
    A [ 0 ] . _mp_d [ 2 ] = u2;
    B [ 0 ] . _mp_size = 2;
    B [ 0 ] . _mp_d [ 0 ] = d0;
    B [ 0 ] . _mp_d [ 1 ] = d1;
    //void mpz_fdiv_qr ( mpz_t, mpz_t, const mpz_t, const mpz_t );
    mpz_fdiv_qr ( C, D, A, B );
    q = div_3_by_2 ( q, r1, r0, u2, u1, u0, d1, d0 );
    if ( C [ 0 ] . _mp_size != 1 || D [ 0 ] . _mp_size > 2 ||
         C [ 0 ] . _mp_d [ 0 ] != q || D [ 0 ] . _mp_size >= 1 && D [ 0 ] . _mp_d [ 0 ] != r0 || D [ 0 ] . _mp_size >= 2 && D [ 0 ] . _mp_d [ 1 ] != r1 ) {
      fprintf ( stderr, "ERROR in 'div'\n" );
      return 1;
    }
        
    q = div_3_by_2_pre ( q, r1, r0, u2, u1, u0, d1, d0, inv_3_by_2 ( d1, d0 ) );
    if ( C [ 0 ] . _mp_size != 1 || D [ 0 ] . _mp_size > 2 ||
         C [ 0 ] . _mp_d [ 0 ] != q || D [ 0 ] . _mp_size >= 1 && D [ 0 ] . _mp_d [ 0 ] != r0 || D [ 0 ] . _mp_size >= 2 && D [ 0 ] . _mp_d [ 1 ] != r1 ) {
      fprintf ( stderr, "ERROR in 'div_pre'\n" );
      return 1;
    }    

    */
    
  }
  fprintf ( stderr, "OK\n" );
  return 0;
#endif

  size_t k = 0;
  //limb_t d1 = rnd ( ) | 0x80000000, d0 = rnd ( ), v, q, r1, r0;
  //v = inv_2_by_1 ( d1, d0 );
  limb_t d = rnd ( ) | 0x80000000, v, q, r;
  v = inv_2_by_1 ( d );
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

    /*limb_t u2 = rnd ( ), u1 = rnd ( ), u0 = rnd ( );    
    while ( u2 >= d1 )  u2 >>= 1;
    //q = test;

    //q = U / d;
    //div_3_by_2 ( q, r1, r0, u2, u1, u0, d1, d0 );
    //div_2_by_1_pre ( q, u1, u0, d, v );
    */

    limb_t u1 = rnd ( ), u0 = rnd ( );    
    while ( u1 >= d )  u1 >>= 1;
    q = test;

    //q = U / d;
    //div_2_by_1 ( q, u1, u0, d );
    div_2_by_1_pre ( q, u1, u0, d, v );

    k += q;
                
  }

  fprintf ( stderr, "%u\n", k );

  return 0;
}
