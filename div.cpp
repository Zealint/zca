/*
**  Здесь реализованы функции деления векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
*/

namespace DivAsm {

  /**
   * Деление 2/1. Делим (u1, u0) на d. Причём u1<d и d>0.
   * Результат в q.
   * Вернуть остаток от деления.
   */
  limb_t __fastcall div_2_by_1 ( limb_t & q, limb_t u1, limb_t u0, limb_t d );

  /**
   * Деление 3/2. Делим (u2, u1, u0) на d=(d1, d0). Причём (u2, u1) < (d1, d0) и d нормализован.
   * Результат в q и он же возвращается.
   * Остаток в (r1, r0).
   */
  limb_t __fastcall div_3_by_2 ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0 );

  /**
   * Деление 2/1 с предподсчитанным обратным элементом.
   * Делим (u1, u0) на d. Причём u1<d и d>0.
   * Более того, d уже нормализован и обратный элемент v посчитан заранее.
   * Результат в q.
   * Вернуть остаток от деления.
   */
  limb_t __fastcall div_2_by_1_pre ( limb_t & q, limb_t u1, limb_t u0, limb_t d, limb_t v );

  /**
   * Деление 3/2 с предподсчитанным обратным элементом.
   * Делим (u2, u1, u0) на d=(d1, d0). Причём (u2, u1) < (d1, d0) и d нормализован.
   * Более того, обратный элемент v посчитан заранее.
   * Результат в q и он же возвращается.
   * Остаток в (r1, r0).
   */
  limb_t __fastcall div_3_by_2_pre ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v );

  /**
   * Расчёт обратного элемента v к нормализованному делителю d.
   * Вернуть v.
   */
  limb_t __fastcall inv_2_by_1 ( limb_t d );

  /**
   * Расчёт обратного элемента v к нормализованному делителю d=(d1, d0).
   * Вернуть v.
   */
  limb_t __fastcall inv_3_by_2 ( limb_t d1, limb_t d0 );

};

namespace Div0 {
  static limb_t __fastcall div_2_by_1 ( limb_t & q, limb_t u1, limb_t u0, limb_t d ) {        
    const size_t HLIMB_BITS = LIMB_BITS / 2;
    hlimb_t u0h, u0l, dh, dl;
    u0h = ( hlimb_t ) ( u0 >> HLIMB_BITS );
    u0l = ( hlimb_t ) u0;
    dh = ( hlimb_t ) ( d >> HLIMB_BITS );
    dl = ( hlimb_t ) d;
    // Делим первые три полулимба (u1, u0h) на знаменатель
    limb_t qh = u1 / dh;
    limb_t dqh = qh * dl;
    limb_t R = ( ( u1 - qh * dh ) << HLIMB_BITS ) | u0h;
    if ( R < dqh ) {
      -- qh;
      R += d;
      if ( R >= d && R < dqh ) {
        -- qh;
        R += d;
      }
    }    
    R -= dqh;
    q = ( qh << HLIMB_BITS ); // Старшая часть частного
    // Делим последние три полулимба (R, u0l) на знаменатель
    limb_t ql = R / dh;    
    limb_t dql = ql * dl;
    R = ( ( R - ql * dh ) << HLIMB_BITS ) | u0l;
    if ( R < dql ) {
      -- ql;
      R += d;
      if ( R >= d && R < dql ) {
        -- ql;
        R += d;
      }
    }
    R -= dql;
    q |= ql;  // Младшая часть ответа.
    return R;
  }

  static limb_t __fastcall div_2_by_1_pre ( limb_t & q, limb_t u1, limb_t u0, limb_t d, limb_t v ) {
    limb_t q1, q0;
    mul_limbs ( q1, q0, u1, v );
    q0 += u0;
    q1 += u1 + 1 + ( q0 < u0 );
    limb_t r = u0 - q1 * d;
    if ( r > q0 ) {
      -- q1;
      r += d;
    }
    if ( r >= d ) {
      ++ q1;
      r -= d;
    }
    q = q1;
    return r;    
  }

  limb_t __fastcall inv_2_by_1 ( limb_t d ) {
    limb_t q;
    div_2_by_1 ( q, ~ d, LIMB_T_MAX, d );
    return q;
  }
    
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

  // !!! Где-то здесь есть ошибка, но я пока не скажу, где !!!
  static limb_t __fastcall div_3_by_2 ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0 ) {
    dlimb_t U = glue ( u2, u1 );
    dlimb_t D = glue ( d1, d0 );
    dlimb_t Q = U / d1;    
    if ( Q >= ( dlimb_t ) ( LIMB_T_MAX ) + 1 )  Q = LIMB_T_MAX;
    dlimb_t DQ = Q * d0;
    dlimb_t R = ( ( U - Q * d1 ) << LIMB_BITS ) | u0;
    if ( R < DQ ) {
      -- Q;
      R += D;
      if ( R >= D && R < DQ ) {
        -- Q;
        R += D;
      }
    }
    R -= DQ;
    r1 = R >> LIMB_BITS;
    r0 = limb_t ( R );
    q = limb_t ( Q );
    return q;
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

  static limb_t __fastcall div_3_by_2_pre ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v ) {
    q = u2 + limb_t ( dlimb_t ( u2 ) * v >> LIMB_BITS );
    dlimb_t qd0 = dlimb_t ( q ) * d0;
    dlimb_t qd1 = dlimb_t ( q ) * d1;
    dlimb_t R0 = qd0;
    limb_t R0_lo = limb_t ( R0 );
    limb_t R0_hi = limb_t ( R0 >> LIMB_BITS );
    dlimb_t R = glue ( u2, u1 ) - qd1 - R0_hi;
    R -= ( u0 < R0_lo );
    u0 -= R0_lo;
    while ( R > d1 || R == d1 && u0 > d0 ) {
      ++ q;
      R -= d1 + ( u0 < d0 );
      u0 -= d0;
    }
    r0 = u0;
    r1 = limb_t ( R );
    return q;
  }

  static limb_t __fastcall inv_2_by_1 ( limb_t d ) {
    return limb_t ( dlimb_t ( -1 ) / d );
  }

  static limb_t __fastcall inv_3_by_2 ( limb_t d1, limb_t d0 ) {
    limb_t q, r1, r0;
    return div_3_by_2 ( q, r1, r0, LIMB_T_MAX-d1, LIMB_T_MAX-d0, LIMB_T_MAX, d1, d0 );
  }
};

using namespace Div0;