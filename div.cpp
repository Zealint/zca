/*
**  Здесь реализованы функции деления векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
*/

namespace DivAsm {

  /**
   * Деление 2/1. Делим (u1, u0) на d. Причём u1<d и d>0.
   * Результат в z. 
   * Вернуть остаток от деления.
   */  
  limb_t __fastcall div_2_by_1 ( limb_t & z, limb_t u1, limb_t u0, limb_t d );

  /**
   * Деление 2/1 с предподсчитанным обратным элементом. 
   * Делим (u1, u0) на d. Причём u1<d и d>0.
   * Более того, d уже нормализован и обратный элемент v посчитан заранее.
   * Результат в z. 
   * Вернуть остаток от деления.
   */  
  limb_t __fastcall div_2_by_1_pre ( limb_t & z, limb_t u1, limb_t u0, limb_t d, limb_t v );

  /**
   * Расчёт обратного элемента v к нормализованному делителю d.
   * Вернуть v.
   */  
  limb_t __fastcall inv_2_by_1 ( limb_t d );
  
};

namespace Div0 {
  // Данный раздел не завершён.
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
