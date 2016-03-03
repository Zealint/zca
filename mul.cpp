/*
**  Здесь реализованы функции умножения векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
*/

namespace MulAsm {

  /**
   * Умножение вектора u размером size на лимб v.
   * Результат в z. Память выделена заранее.
   * Вернуть перенос в старший лимб.
   */  
  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v );
  
  /**
   * То же, но "на месте".
   */
  limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v );

};


namespace Mul0 {

  /**
   * Перемножить два лимба u и v по технологии LoHi.
   * Результат в двух лимбах (h, l).
   */
  void mul_limbs ( limb_t & h, limb_t & l, limb_t u, limb_t v ) {
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
  
  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v ) {  
    limb_t h, l, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, u [ i ], v );
      z [ i ] = l + s;
      // Сложить старшую половину и перенос при сложении.
      s = h + ( ( ( l & s ) | ( ( l | s ) & ( ~ z [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }

  limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v ) {  
    limb_t h, l, s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      mul_limbs ( h, l, u [ i ], v );
      u [ i ] = l + s;
      s = h + ( ( ( l & s ) | ( ( l | s ) & ( ~ u [ i ] ) ) ) >> ( LIMB_BITS - 1 ) );
    }
    return s;
  }
};

namespace Mul1 {
  
  limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v ) {  
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) u [ i ] * v;
      z [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }
  
  limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v ) {  
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {
      s += ( dlimb_t ) u [ i ] * v;
      u [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }

};

using namespace Mul0;

/**
 * Умножение вектора на либм z = u * v.
 * Возвращает корректный размер результата при v > 0.
 * Память в z выделена заранее.
 */
static size_t mul1 ( limb_t * z, const limb_t * u, size_t size, limb_t v ) {
  limb_t carry = mul_N_by_1 ( z, u, size, v );
  if ( carry ) z [ size ++ ] = carry;
  return size;
}

/**
 * То же, но "на месте".
 * Память для возможного переноса в u выделена заранее.
 */
static size_t mul1 ( limb_t * u, size_t size, limb_t v ) {
  limb_t carry = mul_N_by_1 ( u, size, v );
  if ( carry ) u [ size ++ ] = carry;
  return size;
}
