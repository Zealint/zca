/*
**  ����� ����������� ������� ������� ��������.
**  ������ ���� �������� ��������������� ��� Int.cpp � ���������� � ����.
*/

namespace DivAsm {

  /**
   * ������� 2/1. ����� (u1, u0) �� d. ������ u1<d � d>0.
   * ��������� � z. 
   * ������� ������� �� �������.
   */  
  limb_t __fastcall div_2_by_1 ( limb_t & z, limb_t u1, limb_t u0, limb_t d );

  /**
   * ������� 2/1 � ���������������� �������� ���������. 
   * ����� (u1, u0) �� d. ������ u1<d � d>0.
   * ����� ����, d ��� ������������ � �������� ������� v �������� �������.
   * ��������� � z. 
   * ������� ������� �� �������.
   */  
  limb_t __fastcall div_2_by_1_pre ( limb_t & z, limb_t u1, limb_t u0, limb_t d, limb_t v );

  /**
   * ������ ��������� �������� v � ���������������� �������� d.
   * ������� v.
   */  
  limb_t __fastcall inv_2_by_1 ( limb_t d );
  
};

namespace Div0 {
  // ������ ������ �� ��������.
};


namespace Div1 {
  static dlimb_t __fastcall glue ( limb_t u1, limb_t u0 ) {
    return ( ( dlimb_t ( u1 ) << LIMB_BITS ) | u0 );
  }

  static limb_t __fastcall div_2_by_1 ( limb_t & q, limb_t u1, limb_t u0, limb_t d ) {
    dlimb_t U = glue ( u1, u0 );
    q = limb_t ( U / d );
    return limb_t ( U - dlimb_t ( q ) * d );
  }

  static limb_t __fastcall div_2_by_1_pre ( limb_t & q, limb_t u1, limb_t u0, limb_t d, limb_t v ) {
    q = ( dlimb_t ( u1 ) * v >> LIMB_BITS ) + u1;
    dlimb_t R = glue ( u1, u0 ) - dlimb_t ( q ) * d;
    while ( R >= d ) {
      ++ q;
      R -= d;
    }
    return limb_t ( R );
  }

  static limb_t __fastcall inv_2_by_1 ( limb_t d ) {    
    return limb_t ( dlimb_t ( -1 ) / d );
  }  
};

using namespace Div1;
