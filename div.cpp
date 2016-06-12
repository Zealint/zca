/*
**  Здесь реализованы функции деления векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
*/

namespace DivAsm {

  /**
   * Деление 2/1. Делим (u1, u0) на d. Причём u1<d и d>0 и d нормализован.
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

  /**
   * Деление n/1. Делим (u_n, u_(n-1), ..., u_0) на d. Причём d>0 и d нормализован.
   * n > 0. u_n < d.
   * Результат в q, если q != nullptr
   * Вернуть остаток от деления.
   */
  limb_t __fastcall div_n_by_1 ( limb_t * q, const limb_t u_n, const limb_t * u, size_t n, limb_t d );

  /**
   * Деление n/2. Делим (u_n, u_(n-1), ..., u_0) на d=(d_1, d_0). Причём d>0 и d нормализован.
   * n > 1. (u_n, u_(n-1)) < d.
   * Результат в q, если q != nullptr
   * Остаток от деления в r, если r != nullptr.
   */
  void __fastcall div_n_by_2 ( limb_t * q, limb_t * r, const limb_t u_n, const limb_t * u, size_t n, limb_t d );

  /**
   * Деление m/n. Делим (u_m, u_(m-1), ..., u_0) на d=(d_(n-1), ..., d_0). Причём d>0 и d нормализован.
   * n > 2.
   * Результат в q, если q != nullptr
   * Остаток от деления в u.
   */
  //void __fastcall div_m_by_n ( limb_t * q, limb_t * u, const limb_t u_n, size_t size_u, const limb_t * d, size_t size_d, const DivInverse & inv );

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

  static limb_t __fastcall div_3_by_2 ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0 ) {
    if ( u2 == d1 ) {
      limb_t s1 = d0 - u1 - ( u0 != 0 );
      limb_t s0 = -u0;
      if ( s1 < d1 || s1 == d1 && s0 <= d0 ) {
        q = LIMB_T_MAX;
        r1 = d1 - s1 - ( d0 < s0 );
        r0 = d0 - s0;
      } else {
        q = LIMB_T_MAX - 1;
        r1 = d1 - s1 - ( d0 < s0 );
        r0 = d0 - s0;
        r0 += d0;
        r1 += d1 + ( r0 < d0 );
      }
      return q;
    }
    r1 = div_2_by_1 ( q, u2, u1, d1 );
    r0 = u0;
    limb_t DQ0, DQ1;
    mul_limbs ( DQ1, DQ0, q, d0 );
    if ( r1 < DQ1 || r1 == DQ1 && r0 < DQ0 ) {
      -- q;
      r0 += d0;
      r1 += d1 + ( r0 < d0 );
      if ( r1 > d1 && ( r1 < DQ1 || r1 == DQ1 && r0 < DQ0 ) ) {
        -- q;
        r0 += d0;
        r1 += d1 + ( r0 < d0 );
      }
    }
    r1 -= DQ1 + ( r0 < DQ0 );
    r0 -= DQ0;
    return q;
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

  static limb_t __fastcall div_3_by_2_pre ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0, limb_t v ) {
    limb_t q0, q1, t0, t1;
    mul_limbs ( q1, q0, v, u2 );
    q0 += u1;
    q1 += u2 + ( q0 < u1 );
    r1 = ( u1 - q1 * d1 );
    mul_limbs ( t1, t0, d0, q1 );
    t0 += d0;
    t1 += d1 + ( t0 < d0 );
    r0 = u0 - t0;
    r1 = r1 - t1 - ( u0 < t0 );
    q1 ++;
    if ( r1 >= q0 ) {
      q1 --;
      r0 += d0;
      r1 += d1 + ( r0 < d0 );
    }
    if ( r1 > d1 || r1 == d1 && r0 >= d0 ) {
      q1 ++;
      r1 = r1 - d1 - ( r0 < d0 );
      r0 -= d0;
    }
    q = q1;
    return q1;
  }

  limb_t __fastcall inv_2_by_1 ( limb_t d ) {
    limb_t q;
    div_2_by_1 ( q, ~ d, LIMB_T_MAX, d );
    return q;
  }

  static limb_t __fastcall inv_3_by_2 ( limb_t d1, limb_t d0 ) {
    limb_t r1, r0, q;
    return div_3_by_2 ( q, r1, r0, ~d1, ~d0, LIMB_T_MAX, d1, d0 );
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

  static limb_t __fastcall div_3_by_2 ( limb_t & q, limb_t & r1, limb_t & r0, limb_t u2, limb_t u1, limb_t u0, limb_t d1, limb_t d0 ) {
    dlimb_t U = glue ( u2, u1 );
    dlimb_t D = glue ( d1, d0 );
    if ( u2 == d1 ) {
      limb_t s1 = d0 - u1;
      dlimb_t S = ( ( dlimb_t ) ( s1 ) << LIMB_BITS ) - u0;
      if ( S <= D ) {
        q = LIMB_T_MAX;
        dlimb_t R = D - S;
        r1 = ( limb_t ) ( R >> LIMB_BITS );
        r0 = ( limb_t ) ( R );
      } else {
        q = LIMB_T_MAX - 1;
        dlimb_t R = 2 * D - S;
        r1 = ( limb_t ) ( R >> LIMB_BITS );
        r0 = ( limb_t ) ( R );
      }
      return q;
    }
    dlimb_t Q = U / d1;
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

/**
 * Класс для обратного элемента.
 */
class DivInverse {
  DivInverse ( );  // Запрещаем пустой конструктор

  void __fastcall Invert1 ( ) { v = inv_2_by_1 ( d1 ); }       // Инвертировать нормализованный либм [d1]
  void __fastcall Invert2 ( ) { v = inv_3_by_2 ( d1, d0 ); };  // Инвертировать нормализованный делитель [d1:d0]

  // Инвертировать один лимб
  void __fastcall Init ( limb_t d ) {
    shift = count_lz ( d );
    d1 = d << shift;
    Invert1 ( );
  }
  // Инвертировать два лимба
  void __fastcall Init ( limb_t dh, limb_t dl ) {
    shift = count_lz ( dh );
    if ( shift > 0 ) {
      dh = dh << shift | dl >> ( LIMB_BITS - shift );
      dl <<= shift;
    }
    d1 = dh;
    d0 = dl;
    Invert2 ( );
  }
  // Инвертировать два старших лимба, когда их больше двух
  void __fastcall Init ( limb_t dh, limb_t dl, limb_t tail ) {
    shift = count_lz ( dh );
    if ( shift > 0 ) {
      dh = dh << shift | dl >> ( LIMB_BITS - shift );
      dl = dl << shift | tail >> ( LIMB_BITS - shift );
    }
    d1 = dh;
    d0 = dl;
    Invert2 ( );
  }

public:
  limb_t v; // Обратный элемент
  limb_t d1, d0;  // Нормализованный делитель (d0 не используется для 2/1 деления).
  u8 shift; // Сдвиг для нормализации.

  DivInverse ( limb_t d ) { Init ( d ); }
  DivInverse ( limb_t d1, limb_t d0 ) { Init ( d1, d0 ); }
  DivInverse ( limb_t d1, limb_t d0, limb_t tail ) { Init ( d1, d0, tail ); }
  DivInverse ( const limb_t *d, size_t n ) { Init ( d [ n - 1 ], d [ n - 2 ], d [ n - 3 ] ); }
};

namespace Div0 {

  limb_t __fastcall div_n_by_1 ( limb_t * q, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
    limb_t R = u_n, Q;
    size_t i = n;
    if ( q != nullptr )
      while ( i -- > 0 ) R = div_2_by_1_pre ( q [ i ], R, u [ i ], inv . d1, inv . v );
    else
      while ( i -- > 0 ) R = div_2_by_1_pre ( Q, R, u [ i ], inv . d1, inv . v );
    return R;
  }

  void __fastcall div_n_by_2 ( limb_t * q, limb_t * r, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
    limb_t R1 = u_n, R0 = u [ n - 1 ], Q;
    size_t i = n - 1;
    if ( q != nullptr )
      while ( i -- > 0 ) div_3_by_2_pre ( q [ i ], R1, R0, R1, R0, u [ i ], inv . d1, inv . d0, inv . v );
    else
      while ( i -- > 0 ) div_3_by_2_pre ( Q, R1, R0, R1, R0, u [ i ], inv . d1, inv . d0, inv . v );
    if ( r != nullptr ) {
      r [ 0 ] = R0;
      r [ 1 ] = R1;
    }
  }

  void __fastcall div_m_by_n ( limb_t * q, limb_t * u, size_t & size_u, const limb_t * d, size_t size_d, const DivInverse & inv ) {
    size_t k = size_u - size_d;
    limb_t Q, R1, R0;
    // Сравниваем u и d*beta^k
    size_t i = size_d;
    while ( -- i > 0 )
      if ( u [ i + k ] != d [ i ] )
        break;
    // u >= d*beta^k
    if ( u [ i + k ] >= d [ i ] ) {
      if ( q != nullptr ) q [ k ] = 1;
      sub ( u + k, u + k, d, size_d );  // u -= d*beta^k
    }
    else if ( q != nullptr )  q [ k ] = 0;
    while ( k -- > 0 ) {
      size_u --;
      limb_t u2 = u [ size_d + k ], u1 = u [ size_d + k - 1 ], u0 = u [ size_d + k - 2 ];
      if ( u2 == inv . d1 && u1 == inv . d0 )  Q = LIMB_T_MAX;
      else  div_3_by_2_pre ( Q, R1, R0, u2, u1, u0, inv . d1, inv . d0, inv . v );
      limb_t borrow = submul_N_by_1 ( u + k, d, size_d, Q );  // u -= Q*d*beta^k
      if ( borrow > u2 ) {  // Если вычли слишком много (заём больше, чем u2, который должен уничтожиться).
        -- Q;
        add ( u + k, u + k, d, size_d ); // u += d*beta^k
      }
      u [ size_d + k ] = 0; // Уничтожаем старший лимб.
      if ( q != nullptr )  q [ k ] = Q;
    }
  }

};

namespace Div1 {

  limb_t __fastcall div_n_by_1 ( limb_t * q, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
    limb_t R = u_n, Q;
    size_t i = n;
    if ( q != nullptr )
      while ( i -- > 0 )  R = div_2_by_1 ( q [ i ], R, u [ i ], inv . d1 );
    else
      while ( i -- > 0 ) R = div_2_by_1 ( Q, R, u [ i ], inv . d1 );
    return R;
  }

  void __fastcall div_n_by_2 ( limb_t * q, limb_t * r, const limb_t u_n, const limb_t * u, size_t n, const DivInverse & inv ) {
    limb_t R1 = u_n, R0 = u [ n - 1 ], Q;
    size_t i = n - 1;
    if ( q != nullptr )
      while ( i -- > 0 ) div_3_by_2 ( q [ i ], R1, R0, R1, R0, u [ i ], inv . d1, inv . d0 );
    else
      while ( i -- > 0 ) div_3_by_2 ( Q, R1, R0, R1, R0, u [ i ], inv . d1, inv . d0 );
    if ( r != nullptr ) {
      r [ 0 ] = R0;
      r [ 1 ] = R1;
    }
  }

};

/**
 * Сдвиг влево вектора u на величену shift, не превышающую размер одного лимба.
 * Результат в r[0..n-1], возвращается старший лимб, выходящий за пределы n лимбов.
 * n > 0, shift > 0.
 */
static limb_t __fastcall shift_left_short ( limb_t * r, const limb_t * u, size_t n, u8 shift ) {
  //assert ( n >= 1 && 1 <= shift && shift < LIMB_BITS );

  r += n;
  u += n;
  u8 tail = LIMB_BITS - shift;
  limb_t l = * -- u;
  limb_t h = l << shift;
  limb_t res = l >> tail;

  while ( -- n != 0 ) {
    l = * -- u;
    * -- r = h | ( l >> tail );
    h = l << shift;
  }
  * -- r = h;

  return res;
}

/**
 * Сдвиг вправо вектора u на величену shift, не превышающую размер одного лимба.
 * Результат в r[0..n-1], возвращается лимб, который должен был быть на позиции r[-1].
 * n > 0, shift > 0.
 */
static limb_t __fastcall shift_right_short ( limb_t * r, const limb_t * u, size_t n, u8 shift ) {
  //assert ( n >= 1 && 1 <= shift && shift < LIMB_BITS );

  u8 tail = LIMB_BITS - shift;
  limb_t h = * u ++;
  limb_t l = h >> shift;
  limb_t res = h << tail;

  while ( -- n != 0) {
    h = * u ++;
    * r ++ = l | ( h << tail );
    l = h >> shift;
  }
  * r  = l;

  return res;
}


/**
 * Деление n/1. Делим (u_(n-1), ..., u_0) на d. Причём d>0.
 * n > 0.
 * Результат в q, если q != nullptr, остаток в r, если r != nullptr.
 * Вернуть нормализованный размер частного.
 */
static size_t __fastcall div_qr ( limb_t * q, limb_t * r, const limb_t * u, size_t n, limb_t d ) {
  DivInverse inv ( d );
  limb_t * u_shifted, u_n = 0, c = limb_t ( u [ n - 1 ] < d );
  if ( inv . shift > 0 ) {
    u_shifted = new limb_t [ n ];
    u_n = shift_left_short ( u_shifted, u, n, inv . shift );
    u = u_shifted;
  }
  limb_t R = div_n_by_1 ( q, u_n, u, n, inv );
  if ( inv . shift > 0 ) {
    R >>= inv . shift;
    delete [ ] u_shifted;
  }
  if ( r != nullptr ) * r = R;
  return n - c;
}

/**
 * Деление n/2. Делим (u_(n-1), ..., u_0) на d=(d1, d0). Причём d>0.
 * n > 1.
 * Результат в q, если q != nullptr, остаток в r, если r != nullptr.
 * Вернуть нормализованный размер частного.
 */
static size_t __fastcall div_qr ( limb_t * q, limb_t * r, const limb_t * u, size_t n, limb_t d1, limb_t d0 ) {
  DivInverse inv ( d1, d0 );
  limb_t * u_shifted, u_n = 0, c = limb_t ( u [ n - 1 ] < d1 || u [ n - 1 ] == d1 && u [ n - 2 ] < d0 ) + 1;
  if ( inv . shift > 0 ) {
    u_shifted = new limb_t [ n ];
    u_n = shift_left_short ( u_shifted, u, n, inv . shift );
    u = u_shifted;
  }
  limb_t R1, R0;
  div_n_by_2 ( q, r, u_n, u, n, inv );
  if ( inv . shift > 0 ) {
    if ( r != nullptr )
      shift_right_short ( r, r, 2, inv . shift );
    delete [ ] u_shifted;
  }
  return n - c;
}

/**
 * Деление m/n. Делим (u_(m-1), ..., u_0) на (d_(n-1), ..., d_0). Причём d>0.
 * ?? m >= n >= 3.
 * Результат в q, если q != nullptr, остаток в u.
 * Вернуть нормализованный размер частного.
 */
static size_t __fastcall div_qr ( limb_t * q, limb_t * u, size_t & size_u, const limb_t * d, size_t size_d ) {
  if ( size_u < size_d ) {
    return 0;
  }
  if ( size_d == 1 ) {
    size_t res = div_qr ( q, u, u, size_u, d [ 0 ] );
    size_u = 1;
    if ( u [ 0 ] == 0 ) size_u --;
    return res;
  }
  if ( size_d == 2 ) {
    size_t res = div_qr ( q, u, u, size_u, d [ 1 ], d [ 0 ] );
    size_u = 2;
    if ( u [ 1 ] == 0 ) size_u --;
    if ( u [ 0 ] == 0 ) size_u --;
    return res;
  }

  DivInverse inv ( d, size_d );
  size_t k = size_u - size_d;
  limb_t * u_shifted, u_m = 0;
  u_shifted = new limb_t [ size_u + 1 ];
  if ( inv . shift > 0 ) {
    u_m = shift_left_short ( u_shifted, u, size_u, inv . shift );
    if ( u_m )  u_shifted [ size_u ++ ] = u_m;
    limb_t * d_shifted = new limb_t [ size_d ];
    shift_left_short ( d_shifted, d, size_d, inv . shift );
    d = d_shifted;
  }
  else for ( size_t i = 0; i < size_u; i ++ ) u_shifted [ i ] = u [ i ];
  size_t size_q = k + 1;
  div_m_by_n ( q, u_shifted, size_u, d, size_d, inv );
  if ( inv . shift > 0 ) {
    shift_right_short ( u, u_shifted, size_u, inv . shift );
    delete [ ] d;
  } else {
    for ( size_t i = 0; i < size_u; i ++ ) u [ i ] = u_shifted [ i ];
  }
  delete [ ] u_shifted;
  while ( size_u > 0 && u [ size_u - 1 ] == 0 ) size_u --;
  if ( q [ size_q - 1 ] == 0 ) size_q --;
  return size_q;
}
