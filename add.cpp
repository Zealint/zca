/*
**  Здесь реализованы функции сложения векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
**  В начале файла описаны три прототипа для реализации на ассемблере, 
**  а затем разные реализации функций на Си
**  выполнены в нескольких пространствах имён ниже.
*/

/**
 * Функции сложения, в которых перенос определяется сравнением
 */
namespace AddAsm {

  /**
   * Прибавить один лимб 's' к вектору 'u', результат поместить в 'z'.
   * Вернуть перенос в старший разряд (значение z[size_u]).
   * Особенность: если size_u=0, то возвращаемый перенос будет равен 's',
   * тогда как в остальных случаях он будет равен нулю или единице.
   */
  limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s );
  
  /**
   * Та же функция, только "на месте" (результат остаётся в 'u').
   */
  limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s );

  /**
   * Прибавить вектор 'u' к вектору 'v', когда оба имеют одинаковый размер.
   * Результат поместить в 'z'.
   * Вернуть самый старший перенос - ноль или единицу (значение z[size]).
   */
  limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size );

}

/**
 * Функции сложения, в которых перенос определяется сравнением
 */
namespace Add0 {

  static limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; i < size_u; i ++ ) {
      limb_t t = u [ i ] + s;
      s = ( t < s ); // Это краткая запись условия if(t<s) s=1; else s=0;
      z [ i ] = t;
    }
    return s;
  }

  static limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; s > 0 && i < size_u; i ++ ) {
      limb_t t = u [ i ] + s;
      s = ( t < s );
      u [ i ] = t;
    }
    return s;
  }

  static limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    limb_t carry = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      limb_t s = u [ i ] + carry;
      carry = ( s < carry );
      limb_t t = s + v [ i ];
      carry += ( t < s );
      z [ i ] = t;
    }
    return carry;
  }

};

/**
 * Функции сложения, в которых перенос определяется битовым трюком.
 */
namespace Add1 {

  static limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; i < size_u; i ++ ) {
      limb_t t = u [ i ];
      limb_t r = s + t;
      s = ( ( s & t ) | ( ( s | t ) & ( ~ r ) ) ) >> ( LIMB_BITS - 1 );
      z [ i ] = r;
    }
    return s;
  }

  static limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s ) {
    for ( size_t i = 0; s > 0 && i < size_u; i ++ ) {
      limb_t t = u [ i ];
      limb_t r = s + t;
      s = ( ( s & t ) | ( ( s | t ) & ( ~ r ) ) ) >> ( LIMB_BITS - 1 );
      u [ i ] = r;
    }
    return s;
  }
  
  static limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    limb_t carry = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      limb_t s = u [ i ], t = v [ i ];
      limb_t r = s + t + carry;
      carry = ( ( s & t ) | ( ( s | t ) & ( ~ r ) ) ) >> ( LIMB_BITS - 1 );
      z [ i ] = r;      
    }
    return carry;
  }

};

/**
 * Функции сложения с двойным лимбом.
 */
namespace Add2 {

  static limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s ) {
    dlimb_t t = ( dlimb_t ) s;
    for ( size_t i = 0; i < size_u; i ++ ) {
      t += u [ i ];      
      z [ i ] = ( limb_t ) t;
      t >>= LIMB_BITS;
    }
    return ( limb_t ) t;
  }

  static limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s ) {
    dlimb_t t = ( dlimb_t ) s;
    for ( size_t i = 0; t > 0 && i < size_u; i ++ ) {
      t += u [ i ];      
      u [ i ] = ( limb_t ) t;
      t >>= LIMB_BITS;
    }
    return ( limb_t ) t;
  }  

  static limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size ) {
    dlimb_t s = 0;
    for ( size_t i = 0; i < size; i ++ ) {      
      s += u [ i ];
      s += v [ i ];      
      z [ i ] = ( limb_t ) s;
      s >>= LIMB_BITS;
    }
    return ( limb_t ) s;
  }

};

// Выбор нужного набора функций сложения

using namespace Add1;

/**
 * Прибавить вектор 'u' к вектору 'v', когда оба могут иметь разный размер.
 * Вектор 'z' должен иметь достаточный размер, чтобы вместить всю сумму.
 * Вернуть новый размер вектора 'z'.
 */
static size_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  // Если размер 'u' меньше размера 'v', меняем их местами,
  // так мы избежим рассмотрения симметричного случая.
  if ( size_u < size_v ) {
    swap ( size_u, size_v );
    swap ( u, v );
  }  
  limb_t carry = add ( z, u, v, size_v );  
  if ( size_u > size_v )    
    carry = add ( z + size_v, u + size_v, size_u - size_v, carry );
  if ( carry )  z [ size_u ++ ] = carry;
  return size_u;
}

/**
 * Та же функция, только "на месте"
 */
static size_t __fastcall add ( limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  // Если размер 'u' меньше размера 'v', 
  // то придётся скопировать "хвост" 'v' в 'u', а затем сложить общие части.
  if ( size_u < size_v ) {
    for ( size_t i = size_u; i < size_v; i ++ )
      u [ i ] = v [ i ];
    limb_t carry = add ( u, u, v, size_u );
    if ( carry ) {
      carry = add ( u + size_u, size_v - size_u, carry );
      if ( carry )  u [ size_v ++ ] = carry;
    }
    return size_v;
  }  
  // Здесь size_u >= size_v
  limb_t carry = add ( u, u, v, size_v );  
  if ( size_u > size_v )    
    carry = add ( u + size_v, size_u - size_v, carry );
  if ( carry )  u [ size_u ++ ] = carry;
  return size_u;
}
