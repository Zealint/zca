/*
**  ����� ����������� ������� ��������� ��������.
**  ������ ���� �������� ��������������� ��� Int.cpp � ���������� � ����.
*/

namespace MulAsm {

  /**
   * ��������� ������� u �������� size �� ���� v � ������������ ����� c.
   * ��������� � z. ������ �������� �������.
   * ������� ������� � ������� ����.
   */  
  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v, limb_t c );
  
  /**
   * �� ��, �� "�� �����".
   */
  limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v, limb_t c );

  /**
   * ��������� ������� v �������� size �� ���� w � ������������ ����������
   * � ������� u � ��� �� �������� size. 
   * ��������� � z. ������ �������� �������.
   * ������� ������� � ������� ����.
   */  
  limb_t __fastcall addmul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w );

  /**
   * �� ��, �� "�� �����".
   */
  limb_t __fastcall addmul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w );

  /**
   * ��������� ������� v �������� size �� ���� w � ���������� ����������
   * �� ������� u � ��� �� �������� size. 
   * ��������� � z. ������ �������� �������.
   * ������� ��� �� �������� �����.
   */  
  limb_t __fastcall submul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w );

  /**
   * �� ��, �� "�� �����".
   */
  limb_t __fastcall submul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w );

};


namespace Mul0 {

  /**
   * ����������� ��� ����� u � v �� ���������� LoHi.
   * ��������� � ���� ������ (h, l).
   */

  static void mul_limbs ( limb_t & h, limb_t & l, limb_t u, limb_t v ) {
    limb_t u0, v0, u1, v1, w0, w1, w2, w3;
    const limb_t HALF_LIMB_BITS = LIMB_BITS / 2;
    const limb_t L_HALF_MASK = LIMB_T_MAX >> HALF_LIMB_BITS;    
    u0 = u & L_HALF_MASK;
    u1 = u >> HALF_LIMB_BITS;
    v0 = v & L_HALF_MASK;
    v1 = v >> HALF_LIMB_BITS;
    w0 = u0 * v0;
    w1 = u1 * v0;
    w2 = u0 * v1;
    w3 = u1 * v1;
    l = w0 + ( ( w1 + w2 ) << HALF_LIMB_BITS );
    w1 += ( w0 >> HALF_LIMB_BITS );
    w2 += ( w1 & L_HALF_MASK );
    w1 >>= HALF_LIMB_BITS;
    w2 >>= HALF_LIMB_BITS;
    h = w1 + w2 + w3;
  }

  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v, limb_t c ) {
    limb_t h, l, s = c;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, u [ i ], v );
      z [ i ] = l + s;
      // ������� ������� �������� � ������� ��� ��������.
      s = h + ( ( ( l & s ) | ( ( l | s ) & ( ~ z [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }
  

  static limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v, limb_t c ) {  
    limb_t h, l, s = c;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, u [ i ], v );
      u [ i ] = l + s;
      s = h + ( ( ( l & s ) | ( ( l | s ) & ( ~ u [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }

  static limb_t __fastcall addmul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    limb_t h, l, p, q, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, v [ i ], w );
      p = u [ i ];
      q = l + p;
      h += ( ( ( l & p ) | ( ( l | p ) & ( ~ q ) ) ) >> ( LIMB_BITS - 1 ) );      
      z [ i ] = q + s;
      s = h + ( ( ( q & s ) | ( ( q | s ) & ( ~ z [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }
  
  static limb_t __fastcall addmul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    limb_t h, l, p, q, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, v [ i ], w );
      p = u [ i ];
      q = l + p;
      h += ( ( ( l & p ) | ( ( l | p ) & ( ~ q ) ) ) >> ( LIMB_BITS - 1 ) );
      u [ i ] = q + s;
      s = h + ( ( ( q & s ) | ( ( q | s ) & ( ~ u [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }

  limb_t __fastcall submul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    limb_t h, l, p, q, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, v [ i ], w );
      p = u [ i ];
      q = l + s;
      h += ( ( ( l & s ) | ( ( l | s ) & ( ~ q ) ) ) >> ( LIMB_BITS - 1 ) );
      z [ i ] = p - q;
      s = h + ( ( ( ~ p & q ) | ( ( ~ p | q ) & z [ i ] ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }

  limb_t __fastcall submul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    limb_t h, l, p, q, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, v [ i ], w );
      p = u [ i ];
      q = l + s;
      h += ( ( ( l & s ) | ( ( l | s ) & ( ~ q ) ) ) >> ( LIMB_BITS - 1 ) );
      u [ i ] = p - q;
      s = h + ( ( ( ~ p & q ) | ( ( ~ p | q ) & u [ i ] ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }

};

namespace Mul1 {
  
  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v, limb_t c ) {  
    dlimb_t s = c;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) u [ i ] * v;
      z [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }
  
  limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v, limb_t c ) {  
    dlimb_t s = c;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) u [ i ] * v;
      u [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }

  limb_t __fastcall addmul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) v [ i ] * w + u [ i ];
      z [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }

  limb_t __fastcall addmul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) v [ i ] * w + u [ i ];
      u [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }

  limb_t __fastcall submul_N_by_1 ( limb_t * z, const limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    dlimb_t s = 0, t;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) v [ i ] * w;
      t = ( dlimb_t ) u [ i ] - ( limb_t ) s;
      z [ i ] = ( limb_t ) t;
      s >>= LIMB_BITS;
      s += ( t >> ( 2 * LIMB_BITS - 1 ) );
    }
    return ( limb_t ) s;
  }

  limb_t __fastcall submul_N_by_1 ( limb_t * u, const limb_t * v, size_t size, limb_t w ) {
    dlimb_t s = 0, t;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) v [ i ] * w;
      t = ( dlimb_t ) u [ i ] - ( limb_t ) s;
      u [ i ] = ( limb_t ) t;
      s >>= LIMB_BITS;
      s += ( t >> ( 2 * LIMB_BITS - 1 ) );
    }
    return ( limb_t ) s;
  }

};

using namespace Mul0;

/**
 * ��������� ������� �� ���� z = u * v.
 * ���������� ���������� ������ ���������� ��� v > 0.
 * ������ � z �������� �������.
 */
static size_t mul1 ( limb_t * z, const limb_t * u, size_t size, limb_t v ) {
  limb_t carry = mul_N_by_1 ( z, u, size, v, 0 );
  if ( carry ) z [ size ++ ] = carry;
  return size;
}

/**
 * �� ��, �� "�� �����".
 * ������ ��� ���������� �������� � u �������� �������.
 */
static size_t mul1 ( limb_t * u, size_t size, limb_t v ) {
  limb_t carry = mul_N_by_1 ( u, size, v, 0 );
  if ( carry ) u [ size ++ ] = carry;
  return size;
}

/**
 * ��������� ������� �� ���� �� ��������� z = u + v*w.
 * ���������� ������ ����������.
 * ������ � z �������� �������.
 */
static size_t addmul ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v, limb_t w ) {
  limb_t carry;
  size_t size;
  if ( size_u >= size_v ) {
    carry = addmul_N_by_1 ( z, u, v, size_v, w );
    if ( size_u > size_v )
      carry = add ( z + size_v, u + size_v, size_u - size_v, carry );
    size = size_u;
  } else {
    carry = addmul_N_by_1 ( z, u, v, size_u, w );
    carry = mul_N_by_1 ( z + size_u, v + size_u, size_v - size_u, w, carry );
    size = size_v;
  }  
  if ( carry ) z [ size ++ ] = carry;
  return size;
}

/**
 * �� �� �� �����.
 */
static size_t addmul ( limb_t * u, size_t size_u, const limb_t * v, size_t size_v, limb_t w ) {
  limb_t carry;
  size_t size;
  if ( size_u >= size_v ) {
    carry = addmul_N_by_1 ( u, v, size_v, w );
    if ( size_u > size_v && carry )
      carry = add ( u + size_v, size_u - size_v, carry );
    size = size_u;
  } else {
    carry = addmul_N_by_1 ( u, v, size_u, w );
    carry = mul_N_by_1 ( u + size_u, v + size_u, size_v - size_u, w, carry );
    size = size_v;
  }  
  if ( carry ) u [ size ++ ] = carry;
  return size;
}

/**
 * ��������� ������� �� ���� � ���������� z = u - v*w.
 * �����������: u >= v*w.
 * ���������� true ��� ������� ���������.
 * ������ � z �������� �������.
 */
static bool submul ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v, limb_t w ) {
  limb_t borrow = submul_N_by_1 ( z, u, v, size_v, w );
  if ( size_u > size_v )
    borrow = sub ( z + size_v, u + size_v, size_u - size_v, borrow );
  return borrow == 0;
}

/**
 * �� �����.
 */
static bool submul ( limb_t * u, size_t size_u, const limb_t * v, size_t size_v, limb_t w ) {
  limb_t borrow = submul_N_by_1 ( u, v, size_v, w );
  if ( size_u > size_v && borrow )
    borrow = sub ( u + size_v, size_u - size_v, borrow );
  return borrow == 0;
}

/**
 * ��������� ������� �� ������ z = u*v.
 * ���������� ����� ������� ����.
 * ������ � z �������� �������.
 */

limb_t __fastcall mul_N_by_M ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  limb_t s = mul_N_by_1 ( z, u, size_u, * v, 0 );
  z [ size_u ] = s;
  while ( -- size_v > 0 ) {
    v ++;
    z ++;
    s = addmul_N_by_1 ( z, u, size_u, * v );
    z [ size_u ] = s;
  }
  return s;
}

/**
 * ��������� ������� �� ������ z = u*v.
 * ���������� ������ ����������.
 * ������ � z �������� �������.
 */
static size_t mul ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  limb_t carry = mul_N_by_M ( z, u, size_u, v, size_v );
  return size_u + size_v - size_t ( carry == 0 );
}
