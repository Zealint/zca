/*
**  ����� ����������� ������� ��������� ��������.
**  ������ ���� �������� ��������������� ��� Int.cpp � ���������� � ����.
**  � ������ ����� ������� ��� ��������� ��� ���������� �� ����������, 
**  � ����� ������ ���������� ������� �� ��
**  ��������� � ���������� ������������� ��� ����.
*/

/**
 * ������� ���������, � ������� ��� ������������ ����������
 */
namespace SubAsm {

  /**
   * ������� ���� ���� 's' �� ������� 'u', ��������� ��������� � 'z'.
   * ������� ��� �� �������� �������.
   * �����������: ���� size_u=0, �� ������������ ��� ����� ����� 's',
   * ����� ��� � ��������� ������� �� ����� ����� ���� ��� �������.
   */
  limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s );
  
  /**
   * �� �� �������, ������ "�� �����" (��������� ������� � 'u').
   */
  limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s );

  /**
   * ������� �� ������� 'u' �� ������ 'v', ����� ��� ����� ���������� ������.
   * ��������� ��������� � 'z'.
   * ������� ����� ������� ��� - ���� ��� �������.
   */
  limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size );

}

/**
 * ������� ���������, � ������� ��� ������������ ����������
 */
namespace Sub0 {

  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; i < size_u; i ++ ) {
      limb_t t = u [ i ];      
      z [ i ] = t - s;
      s = ( t < s );
    }
    return s;
  }

  static limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; s > 0 && i < size_u; i ++ ) {
      limb_t t = u [ i ];
      u [ i ] = t - s;
      s = ( t < s );
    }
    return s;
  }

  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    limb_t borrow = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      limb_t s = u [ i ];
      limb_t t = v [ i ] + borrow;
      borrow = ( s < t ) || ( t == 0 );
      z [ i ] = s - t;
    }
    return borrow;
  }

};

/**
 * ������� ���������, � ������� ��� ������������ ������� ������.
 */
namespace Sub1 {

  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; i < size_u; i ++ ) {
      limb_t t = u [ i ];
      limb_t r = t - s;
      s = ( ( ~ t & s ) | ( ( ~ t | s ) & r ) ) >> ( LIMB_BITS - 1 );
      z [ i ] = r;
    }
    return s;
  }

  static limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; s > 0 && i < size_u; i ++ ) {
      limb_t t = u [ i ];
      limb_t r = t - s;
      s = ( ( ~ t & s ) | ( ( ~ t | s ) & r ) ) >> ( LIMB_BITS - 1 );
      u [ i ] = r;
    }
    return s;
  }
  
  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    limb_t borrow = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      limb_t s = u [ i ];
      limb_t t = v [ i ];
      limb_t r = s - t - borrow;
      borrow = ( ( ~ s & t ) | ( ( ~ s | t ) & r ) ) >> ( LIMB_BITS - 1 );
      z [ i ] = r;
    }
    return borrow;
  }

};

/**
 * ������� ��������� � ������� ������.
 */
namespace Sub2 {

  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    dlimb_t t = ( dlimb_t ) s;
    for ( size_t i = 0; i < size_u; i ++ ) {
      t = ( dlimb_t ) u [ i ] - t;
      z [ i ] = ( limb_t ) ( t );
      t = ( t >> ( 2 * LIMB_BITS - 1 ) );
    }
    return ( limb_t ) t;
  }

  static limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s ) {
    dlimb_t t = ( dlimb_t ) s;
    for ( size_t i = 0; t > 0 && i < size_u; i ++ ) {
      t = ( dlimb_t ) u [ i ] - t;
      u [ i ] = ( limb_t ) ( t );
      t = ( t >> ( 2 * LIMB_BITS - 1 ) );
    }
    return ( limb_t ) t;
  }  

  static limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      s = ( dlimb_t ) u [ i ] - s;
      s -= v [ i ];
      z [ i ] = ( limb_t ) s;
      s >>= ( 2 * LIMB_BITS - 1 );
    }
    return ( limb_t ) s;
  }

};

// ����� ������� ������ ������� ���������

using namespace Sub1;

/**
 * ������� �� ������� 'u' ������ 'v', ����� ��� ����� ����� ������ ������.
 * ������ 'u' ������ ���� �� ������ ������� 'v', � ��������� ������ ��������� �������������.
 * ������ 'z' ������ ����� ����������� ������, ����� �������� ��� ��������.
 * ������� true, ���� u >= v, � false � ��������� ������.
 */
static bool __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {  
  limb_t borrow = sub ( z, u, v, size_v );  
  if ( size_u > size_v )    
    borrow = sub ( z + size_v, u + size_v, size_u - size_v, borrow );  
  return borrow == 0;
}

/**
 * �� �� �������, ������ "�� �����"
 */
static bool __fastcall sub ( limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  limb_t borrow = sub ( u, u, v, size_v );
  if ( size_u > size_v )
    borrow = sub ( u + size_v, size_u - size_v, borrow );
  return borrow == 0;
}
