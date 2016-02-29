/*
**  Здесь реализованы функции вычитания векторов.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
**  В начале файла описаны три прототипа для реализации на ассемблере, 
**  а затем разные реализации функций на Си
**  выполнены в нескольких пространствах имён ниже.
*/

/**
 * Функции вычитания, в которых заём определяется сравнением
 */
namespace SubAsm {

  /**
   * Вычесть один лимб 's' из вектора 'u', результат поместить в 'z'.
   * Вернуть заём из старшего разряда.
   * Особенность: если size_u=0, то возвращаемый заём будет равен 's',
   * тогда как в остальных случаях он будет равен нулю или единице.
   */
  limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s );
  
  /**
   * Та же функция, только "на месте" (результат остаётся в 'u').
   */
  limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s );

  /**
   * Вычесть из вектора 'u' из вектор 'v', когда оба имеют одинаковый размер.
   * Результат поместить в 'z'.
   * Вернуть самый старший заём - ноль или единицу.
   */
  limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size );

}

/**
 * Функции вычитания, в которых заём определяется сравнением
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
 * Функции вычитания, в которых заём определяется битовым трюком.
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
 * Функции вычитания с двойным лимбом.
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

// Выбор нужного набора функций вычитания

using namespace Sub1;

/**
 * Вычесть из вектора 'u' вектор 'v', когда оба могут иметь разный размер.
 * Вектор 'u' должен быть не меньше вектора 'v', в противном случае результат непредсказуем.
 * Вектор 'z' должен иметь достаточный размер, чтобы вместить всю разность.
 * Вернуть true, если u >= v, и false в противном случае.
 */
static bool __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {  
  limb_t borrow = sub ( z, u, v, size_v );  
  if ( size_u > size_v )    
    borrow = sub ( z + size_v, u + size_v, size_u - size_v, borrow );  
  return borrow == 0;
}

/**
 * Та же функция, только "на месте"
 */
static bool __fastcall sub ( limb_t * u, size_t size_u, const limb_t * v, size_t size_v ) {
  limb_t borrow = sub ( u, u, v, size_v );
  if ( size_u > size_v )
    borrow = sub ( u + size_v, size_u - size_v, borrow );
  return borrow == 0;
}
