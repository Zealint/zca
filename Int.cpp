/**
 * Здесь собраны основные функции для работы с длинными числами
 */

#include <assert.h>

/**
 * Базовые типы данных (для удобства)
 */
typedef unsigned int limb_t;         // Один лимб - 32 бита без знака
typedef signed int slimb_t;          // "Лимб" со знаком
typedef unsigned short int hlimb_t;  // Половинный лимб - 16 битов без знака
typedef unsigned long long dlimb_t;  // Двойной лимб - 64 бита без знака
typedef char sign_t;                 // Знак (-1, 0, 1)
typedef int offset_t;                // Знаковый размер длинного числа
typedef unsigned char u8;            // 8 бит без знака

/**
 * Базовые константы
 */
const size_t LIMB_SIZE = sizeof ( limb_t );
const size_t LIMB_BITS = LIMB_SIZE * 8;
const limb_t LIMB_T_MAX = 0xFFFFFFFF;

// Некоторые функции намеренно реализованы в указанных файлах
// для удобства и порядка
#include "misc.cpp" // Вспомогательные фукнции (abs, min, max, sign, ...).
#include "add.cpp"  // Всё, что касается сложения векторов.
#include "sub.cpp"  // Вычитание векторов.
#include "mul.cpp"  // Умножение векторов.
#include "div.cpp"  // Деление векторов.

class num_t;

/**
 * Вектор лимбов.
 * Обёртка для массива лимбов и размера.
 */
class vec_t {
  public:
    size_t size;     // Размер (не обязательно нормализованного) вектора.
    limb_t * limbs;  // Массив лимбов.

    /**
     * Конструкторы и деструктор
     */
    vec_t ( ) : size ( 0 ), limbs ( nullptr ) { }
    vec_t ( size_t s ) : size ( s ), limbs ( new limb_t [ s ] ) { }
    vec_t ( const vec_t & v ) : size ( v . size ), limbs ( new limb_t [ v . size ] ) {
      for ( size_t i = 0; i < v . size; i ++ )
        limbs [ i ] = v . limbs [ i ];
    }
    //vec_t ( const num_t & n ) : size ( abs ( n . size ) ), limbs ( n . limbs ) { }
    vec_t ( vec_t && v ) : size ( v . size ), limbs ( v . limbs ) {
      v . limbs = nullptr;
    }
    // Привязать вектор без знака к вектору со знаком (как в "Alias", ниже).
    vec_t ( const num_t & n );
    ~ vec_t ( ) { delete [ ] limbs; }

    /**
     * Привязать вектор без знака к вектору со знаком.
     * Размер берётся по модулю (size=abs(n.size)),
     * а указатель на лимбы устанавливается на указатель n.limbs.
     */
    void Alias ( const num_t & n );

    /**
     * Отвязать знаковый вектор.
     * Это нужно после привязки вектора, чтобы при вызове деструктора
     * не удалить тот вектор со знаком, к которому мы привязались.
     */
    void FreeAlias ( ) {
      limbs = nullptr;
    }

    /**
     * Нормализовать вектор.
     * Размер size не превышает количество выделенных лимбов в limbs.
     * Возвращает нормализованный размер.
     */
    size_t Normalize ( ) {
      while ( size > 0 && limbs [ size - 1 ] == 0 )
        size --;
      return size;
    }

    /**
     * Сравнить с другим вектором того же размера.
     * size = v.size
     * size > 0
     * Возвращает результат сравнения в виде знака.
     */
    sign_t CompareEqualSize ( const vec_t & v ) const {
      size_t i = size;
      while ( -- i > 0 && limbs [ i ] == v . limbs [ i ] );
      if ( limbs [ i ] < v . limbs [ i ] )  return -1;
      if ( limbs [ i ] > v . limbs [ i ] )  return 1;
      return 0;
    }

    /**
     * Сравнить с другим вектором.
     * Оба вектора должны быть нормализованными.
     * Возвращает результат сравнения в виде знака.
     */
    sign_t Compare ( const vec_t & v ) const {
      if ( size < v . size )  return -1;
      if ( size > v . size )  return 1;
      if ( size == 0 )  return 0;
      return CompareEqualSize ( v );
    }

    /**
     * Присвоить себе значение другого вектора.
     * Нужный объём памяти должен быть выделен заранее.
     * Возвращает ссылку на себя.
     */
    vec_t & operator = ( const vec_t & v ) {
      if ( this == & v )
        return * this;
      size = v . size;
      for ( size_t i = 0; i < size; i ++ )
        limbs [ i ] = v [ i ];
      return * this;
    }

    /**
     * Переместить себе значение другого вектора.
     * Выделенная ранее память удаляется.
     * Возвращает ссылку на себя.
     */
    vec_t & operator = ( vec_t && v ) {
      if ( this == & v )
        return * this;
      delete [ ] limbs;
      size = v . size;
      limbs = v . limbs;
      v . limbs = nullptr;
      return * this;
    }

    /**
     * Вспомогательные функции [] для более удобного обращения к лимбам вектора.
     */
    limb_t & operator [ ] ( size_t index ) {  return limbs [ index ];  }
    const limb_t & operator [ ] ( size_t index ) const {  return limbs [ index ];  }

    /**
     * Прибавить вектор 'v'
     * Необходимая память должна быть выделена заранее.
     */
    size_t add ( const vec_t & v ) {
      // Вызывается функция из файла add.cpp
      return size = :: add ( limbs, size, v . limbs, v . size );
    }

    /**
     * Прибавить вектор 'v'
     * Обёртка для add (выше).
     */
    const vec_t & operator += ( const vec_t & v ) {
      this -> add ( v );
      return * this;
    }

    /**
     * Отнять вектор 'v'.
     * Вектор 'v' должен быть не больше уменьшаемого (this).
     */
    size_t sub ( const vec_t & v ) {
      // Вызывается функция из файла sub.cpp
      bool isCorrect ( :: sub ( limbs, size, v . limbs, v . size ) );
      assert ( isCorrect ); // Убедимся, что не было заёма.
      return Normalize ( ); // При вычитании нормализация обязательна.
    }

    /**
     * Отнять вектор 'v'.
     * Обёртка для sub (выше).
     */
    const vec_t & operator -= ( const vec_t & v ) {
      this -> sub ( v );
      return * this;
    }

    /**
     * Умножить на лимб
     * Необходимая память должна быть выделена заранее.
     */
    size_t mul1 ( limb_t v ) {
      if ( size == 0 || v == 0 ) return size = 0;
      // Вызывается функция из файла mul.cpp
      return size = :: mul1 ( limbs, size, v );
    }

    /**
     * Умножить на лимб
     * Обёртка для mul1 (выше).
     */
    const vec_t & operator *= ( limb_t v ) {
      this -> mul1 ( v );
      return * this;
    }

    /**
     * Умножить на вектор
     * Необходимая память должна быть выделена заранее.
     */
    size_t mul ( const vec_t & v ) {
      if ( size == 0 || v . size == 0 ) return size = 0;
      if ( v . size == 1 ) return mul1 ( v [ 0 ] );
      vec_t w ( * this );
      // Вызывается функция из файла mul.cpp
      return size = :: mul ( limbs, w . limbs, w . size, v . limbs, v . size );
    }

    /**
     * Умножить на вектор
     * Обёртка для mul (выше).
     */
    const vec_t & operator *= ( const vec_t & v ) {
      this -> mul ( v );
      return * this;
    }

    /**
     * Прибавить вектор, умноженный на лимб
     * Необходимая память должна быть выделена заранее.
     */
    size_t addmul ( const vec_t & u, limb_t v ) {
      if ( u . size == 0 || v == 0 ) return size;
      // Вызывается функция из файла mul.cpp
      return size = :: addmul ( limbs, size, u . limbs, u . size, v );
    }

    /**
     * Отнять вектор, умноженный на лимб
     * Необходимая память должна быть выделена заранее.
     * Уменьшаемое должно быть не меньше вычитаемого.
     */
    size_t submul ( const vec_t & u, limb_t v ) {
      if ( u . size == 0 || v == 0 ) return size;
      // Вызывается функция из файла mul.cpp
      :: submul ( limbs, size, u . limbs, u . size, v );
      return Normalize ( );
    }

    /**
     * Поделить на лимб с остатком.
     * r может быть nullptr.
     */
    size_t div_qr ( limb_t * r, limb_t v ) {
      if ( size == 0 || v == 1 ) {
        if ( r != nullptr ) * r = 0;
        return size;
      }
      // Вызывается функция из файла div.cpp
      return size = :: div_qr ( limbs, r, limbs, size, v );
    }

    /**
     * Поделить на лимб
     */
    const vec_t & operator /= ( limb_t v ) {
      this -> div_qr ( nullptr, v );
      return * this;
    }

    /**
     * Остаток от деления на лимб
     */
    const vec_t & operator %= ( limb_t v ) {
      limb_t r;
      this -> div_qr ( & r, v );
      if ( r > 0 ) {
        limbs [ 0 ] = r;
        size = 1;
      } else size = 0;
      return * this;
    }

    /**
     * Поделить на вектор с остатком.
     * r может быть nullptr.
     */
    size_t div_qr ( vec_t * r, const vec_t & d ) {
      if ( size == 0 ) {
        if ( r != nullptr ) * r = * this;
        return size;
      }
      // Вызывается функция из файла div.cpp
      if ( r != nullptr ) {
        * r = * this;
        size = :: div_qr ( limbs, r -> limbs, r -> size, d . limbs, d . size );
      }
      else {
        r = new vec_t ( * this );
        size = :: div_qr ( limbs, r -> limbs, r -> size, d . limbs, d . size );
        delete r;
      }
      return size;
    }

    /**
     * Внешние операторы
     */
    friend const bool operator < ( const vec_t &, const vec_t & );
    friend const bool operator > ( const vec_t &, const vec_t & );
    friend const bool operator <= ( const vec_t &, const vec_t & );
    friend const bool operator >= ( const vec_t &, const vec_t & );
    friend const bool operator == ( const vec_t &, const vec_t & );
    friend const bool operator != ( const vec_t &, const vec_t & );

    friend vec_t operator + ( const vec_t &, const vec_t & );
    friend vec_t operator - ( const vec_t &, const vec_t & );
    friend vec_t operator * ( const vec_t &, const vec_t & );
    friend vec_t operator / ( const vec_t &, limb_t );
    friend limb_t operator % ( const vec_t &, limb_t );

};

/**
 * Реализация внешних операторов
 */
const bool operator < ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) < 0;
}

const bool operator > ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) > 0;
}

const bool operator <= ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) <= 0;
}

const bool operator >= ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) >= 0;
}

const bool operator == ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) == 0;
}

const bool operator != ( const vec_t & u, const vec_t & v ) {
  return u . Compare ( v ) != 0;
}

/**
 * Переходная функция для сложения векторов vec_t через функции из add.cpp
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void add ( vec_t & z, const vec_t & u, const vec_t & v ) {
  // Вызывается функция из файла add.cpp
  if ( z . limbs == u . limbs ) // Сложение "на месте" u += v
    z . size = :: add ( z . limbs, u . size, v . limbs, v . size );
  else if ( z . limbs == v . limbs )  // Сложение "на месте" v += u
    z . size = :: add ( z . limbs, v . size, u . limbs, u . size );
  else // Не "на месте"
    z . size = :: add ( z . limbs, u . limbs, u . size, v . limbs, v . size );
}

/**
 * Сложение u + v.
 * Нужная память выделяется здесь же.
 */
vec_t operator + ( const vec_t & u, const vec_t & v ) {
  vec_t z ( max ( u . size, v . size ) + 1 );
  add ( z, u, v );
  return z;
}

/**
 * Переходная функция для вычитания векторов vec_t через функции из sub.cpp
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 * u >= v.
 */
static void sub ( vec_t & z, const vec_t & u, const vec_t & v ) {
  if ( z . limbs == u . limbs ) // "На месте", u-=v.
    :: sub ( z . limbs, u . size, v . limbs, v . size );
  else // Не "на месте".
    :: sub ( z . limbs, u . limbs, u . size, v . limbs, v . size );
  z . size = u . size;
  z . Normalize ( );
}

/**
 * Вычитание u - v.
 * u >= v.
 * Память выделяется здесь же.
 */
vec_t operator - ( const vec_t & u, const vec_t & v ) {
  vec_t z ( u . size );
  sub ( z, u, v );
  return z;
}

/**
 * Переходная функция для умножения вектора vec_t на лимб через функции из mul.cpp
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void mul1 ( vec_t & z, const vec_t & u, limb_t v ) {
  if ( u . size == 0 || v == 0 )
    z . size = 0;
  else
    // Вызывается функция из файла mul.cpp
    z . size = mul1 ( z . limbs, u . limbs, u . size, v );
}

/**
 * Прибавить к вектору другой вектор, умноженный на лимб.
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void addmul ( vec_t & z, const vec_t & u, const vec_t & v, limb_t w ) {
  if ( v . size == 0 || w == 0 ) {
    if ( z . limbs == u . limbs ) return;
    z = u; // Копирование без выделения памяти.
  }
  else {
    if ( z . limbs == u . limbs )
      z . size = :: addmul ( z . limbs, u . size, v . limbs, v . size, w );
    else
      z . size = :: addmul ( z . limbs, u . limbs, u . size, v . limbs, v . size, w );
  }
}

/**
 * Отнять от вектора другой вектор, умноженный на лимб.
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void submul ( vec_t & z, const vec_t & u, const vec_t & v, limb_t w ) {
  if ( v . size == 0 || w == 0 ) {
    if ( z . limbs == u . limbs ) return;
    z = u; // Копирование без выделения памяти.
  }
  else {
    if ( z . limbs == u . limbs )
      :: submul ( z . limbs, u . size, v . limbs, v . size, w );
    else
      :: submul ( z . limbs, u . limbs, u . size, v . limbs, v . size, w );
    z . size = u . size;
    z . Normalize ( );
  }
}

/**
 * Умножение u * v.
 * Нужная память выделяется здесь же.
 */
vec_t operator * ( const vec_t & u, limb_t v ) {
  vec_t z ( u . size + 1 );
  mul1 ( z, u, v );
  return z;
}

/**
 * Переходная функция для умножения вектора на вектор через функции из mul.cpp
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void mul ( vec_t & z, const vec_t & u, const vec_t & v ) {
  if ( u . size == 0 || v . size == 0 )
    z . size = 0;
  else if ( v . size == 1 )
    // Вызывается функция из файла mul.cpp
    z . size = :: mul1 ( z . limbs, u . limbs, u . size, v [ 0 ] );
  else
    // Вызывается функция из файла mul.cpp
    z . size = :: mul ( z . limbs, u . limbs, u . size, v . limbs, v . size );
}

/**
 * Умножение u * v.
 * Нужная память выделяется здесь же.
 */
vec_t operator * ( const vec_t & u, const vec_t & v ) {
  vec_t z ( u . size + v . size );
  mul ( z, u, v );
  return z;
}

/**
 * Вычесть из большего вектора меньший.
 * Вспомогательная функция для вычитания знаковых векторов (ниже).
 */
static sign_t fullSub ( vec_t & z, const vec_t & u, const vec_t & v ) {
  sign_t s = u . Compare ( v );  // Кто больше?
  if ( s == 0 ) z . size = 0; // В случае равенства результат 0.
  else if ( s > 0 ) {         // u > v
    if ( z . limbs == u . limbs ) z -= v; // Если это вычитание "на месте".
    else :: sub ( z, u, v ); // Не "на месте".
  }
  else { // u < v
    if ( z . limbs == v . limbs ) z -= u;  // "На месте".
    else :: sub ( z, v, u ); // Не "на месте".
  }
  return s;
}

/**
 * Переходная функция для деления вектора vec_t на лимб через функции из div.cpp
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 * z и r могут быть равны nullptr.
 */
static void div_qr ( vec_t * z, limb_t * r, const vec_t & u, limb_t v ) {
  if ( u . size == 0 ) {
    if ( z != nullptr ) z -> size = 0;
    if ( r != nullptr ) * r = 0;
  }
  else
    // Вызывается функция из файла div.cpp
    if ( z != nullptr ) z -> size = :: div_qr ( z -> limbs, r, u . limbs, u . size, v );
    else :: div_qr ( nullptr, r, u . limbs, u . size, v );
}

/**
 * Деление на один лимб u / v.
 * Нужная память выделяется здесь же.
 */
vec_t operator / ( const vec_t & u, limb_t v ) {
  vec_t z ( u . size );
  div_qr ( & z, nullptr, u, v );
  return z;
}

/**
 * Остаток от деления на один лимб u / v.
 */
limb_t operator % ( const vec_t & u, limb_t v ) {
  limb_t r;
  div_qr ( nullptr, & r, u, v );
  return r;
}



/**
 * Знаковый вектор лимбов.
 * Обёртка для массива лимбов и знакового размера.
 */
class num_t {
  public:
    offset_t size;   // Знаковый размер (не обязательно нормализованного) вектора.
    limb_t * limbs;  // Массив лимбов.

    /**
     * Конструкторы и деструктор
     */
    num_t ( ) : size ( 0 ), limbs ( nullptr ) { }
    num_t ( offset_t s ) : size ( s ), limbs ( new limb_t [ abs ( s ) ] ) { }
    num_t ( const num_t & n ) : size ( n . size ), limbs ( new limb_t [ abs ( n . size ) ] ) {
      for ( size_t i = 0; i < abs ( n . size ); i ++ )
        limbs [ i ] = n . limbs [ i ];
    }
    num_t ( num_t && n ) : size ( n . size ), limbs ( n . limbs ) {
      n . limbs = nullptr;
    }
    ~ num_t ( ) { delete [ ] limbs; }

    /**
     * Получить знак.
     */
    sign_t Sign ( ) const {
      return sign ( size );
    }

    /**
     * Нормализовать вектор.
     * Размер |size| не превышает количество выделенных лимбов в limbs.
     * Возвращает нормализованный размер.
     */
    offset_t Normalize ( ) {
      vec_t vec ( * this );
      vec . Normalize ( );
      if ( size < 0 ) size = - ( offset_t ) vec . size;
      else size = ( offset_t ) vec . size;
      vec . FreeAlias ( );
      return size;
    }

    /**
     * Сравнить с другим знаковым вектором.
     * Оба вектора должны быть нормализованными.
     * Возвращает результат сравнения в виде знака.
     */
    sign_t Compare ( const num_t & n ) const {
      if ( size < n . size )  return -1;
      if ( size > n . size )  return 1;
      if ( size == 0 )  return 0;
      // Переходим к беззнаковым векторам.
      vec_t vec ( * this ), vec_n ( n );
      sign_t res = vec . CompareEqualSize ( vec_n );
      vec . FreeAlias ( );
      vec_n . FreeAlias ( );
      return res * Sign ( );
    }

    /**
     * Присвоить себе значение другого вектора.
     * Нужный объём памяти должен быть выделен заранее.
     * Возвращает ссылку на себя.
     */
    num_t & operator = ( const num_t & n ) {
      if ( this == & n )
        return * this;
      size = n . size;
      for ( size_t i = 0; i < abs ( size ); i ++ )
        limbs [ i ] = n [ i ];
      return * this;
    }

    /**
     * Переместить себе значение другого вектора.
     * Выделенная ранее память удаляется.
     * Возвращает ссылку на себя.
     */
    num_t & operator = ( num_t && n ) {
      if ( this == & n )
        return * this;
      delete [ ] limbs;
      size = n . size;
      limbs = n . limbs;
      n . limbs = nullptr;
      return * this;
    }

    /**
     * Вспомогательные функции [] для более удобного обращения к лимбам вектора.
     */
    limb_t & operator [ ] ( size_t index ) {  return limbs [ index ];  }
    const limb_t & operator [ ] ( size_t index ) const {  return limbs [ index ];  }

    /**
     * Прибавить вектор 'n'
     * Необходимая память должна быть выделена заранее.
     */
    offset_t add ( const num_t & n ) {
      vec_t vec ( * this ), vec_n ( n );
      sign_t s;
      // Переменные разного знака.
      if ( ( Sign ( ) ^ n . Sign ( ) ) < 0 ) {
        s = :: fullSub ( vec, vec, vec_n );
        if ( Sign ( ) < 0 )  s = -s;
      }
      // Переменные одного знака.
      else {
        vec += vec_n;
        s = Sign ( ) < 0 ? -1 : 1;
      }
      size = ( offset_t ) s * vec . size;
      vec . FreeAlias ( );
      vec_n . FreeAlias ( );
      return size;
    }

    /**
     * Прибавить вектор 'n'
     * Обёртка для add (выше).
     */
    const num_t & operator += ( const num_t & n ) {
      this -> add ( n );
      return * this;
    }

    /**
     * Отнять вектор 'n'.
     */
    offset_t sub ( const num_t & n ) {
      vec_t vec ( * this ), vec_n ( n );
      sign_t s;
      // Переменные разного знака.
      if ( ( Sign ( ) ^ n . Sign ( ) ) < 0 ) {
        vec += vec_n;
        s = Sign ( ) < 0 ? -1 : 1;
      }
      else {
        s = :: fullSub ( vec, vec, vec_n );
        if ( Sign ( ) < 0 )  s = -s;
      }
      size = ( offset_t ) s * vec . size;
      vec . FreeAlias ( );
      vec_n . FreeAlias ( );
      return Normalize ( );
    }

    /**
     * Отнять вектор 'n'
     * Обёртка для sub (выше).
     */
    const num_t & operator -= ( const num_t & n ) {
      this -> sub ( n );
      return * this;
    }

    /**
     * Умножить на лимб
     * Необходимая память должна быть выделена заранее.
     */
    offset_t mul1 ( limb_t v ) {
      vec_t vec ( * this );
      vec *= v;
      if ( size < 0 ) size = - ( offset_t ) vec . size;
      else size = ( offset_t ) vec . size;
      vec . FreeAlias ( );
      return size;
    }

    /**
     * Умножить на лимб
     * Обёртка для mul1 (выше).
     */
    const num_t & operator *= ( limb_t v ) {
      this -> mul1 ( v );
      return * this;
    }

   /**
     * Умножить на вектор
     * Необходимая память должна быть выделена заранее.
     */
    offset_t mul ( const num_t & v ) {
      vec_t vec ( * this ), vec_v ( v );
      vec *= vec_v;
      if ( ( size ^ v . size ) < 0 ) size = - ( offset_t ) vec . size;
      else size = ( offset_t ) vec . size;
      vec . FreeAlias ( );
      vec_v . FreeAlias ( );
      return size;
    }

    /**
     * Умножить на вектор
     * Обёртка для mul (выше).
     */
    const num_t & operator *= ( const num_t & v ) {
      this -> mul ( v );
      return * this;
    }

    /**
     * Поделить на знаковый лимб
     */
    offset_t div_qr ( slimb_t * r, slimb_t v ) {
      vec_t vec ( * this );
      offset_t s = size;
      bool is_neg = ( Sign ( ) < 0 ) ^ ( v < 0 );
      vec . div_qr ( ( limb_t * ) r, ( limb_t ) abs ( v ) );
      if ( r != nullptr && s < 0 ) * r = - * r;
      if ( is_neg ) size = - ( offset_t ) vec . size;
      else size = ( offset_t ) vec . size;
      vec . FreeAlias ( );
      return size;
    }

    /**
     * Поделить на знаковый лимб
     */
    const num_t & operator /= ( slimb_t v ) {
      this -> div_qr ( nullptr, v );
      return * this;
    }

    /**
     * Остаток от деления на знаковый лимб
     */
    const num_t & operator %= ( slimb_t v ) {
      slimb_t r;
      this -> div_qr ( & r, v );
      if ( r != 0 ) {
        limbs [ 0 ] = abs ( r );
        size = r > 0 ? 1 : -1;
      } else size = 0;
      return * this;
    }


    /**
     * Внешние операторы
     */
    friend const bool operator < ( const num_t &, const num_t & );
    friend const bool operator > ( const num_t &, const num_t & );
    friend const bool operator <= ( const num_t &, const num_t & );
    friend const bool operator >= ( const num_t &, const num_t & );
    friend const bool operator == ( const num_t &, const num_t & );
    friend const bool operator != ( const num_t &, const num_t & );

    friend num_t operator + ( const num_t &, const num_t & );
    friend num_t operator - ( const num_t &, const num_t & );
    friend num_t operator * ( const num_t &, const num_t & );

};

/**
 * Реализация привязки вектора без знака к вектору со знаком.
 */
vec_t :: vec_t ( const num_t & n ) {
  Alias ( n );
}

void vec_t :: Alias ( const num_t & n ) {
  size = abs ( n . size );
  limbs = n . limbs;
}

/**
 * Реализация внешних операторов для num_t
 */
const bool operator < ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) < 0;
}

const bool operator > ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) > 0;
}

const bool operator <= ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) <= 0;
}

const bool operator >= ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) >= 0;
}

const bool operator == ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) == 0;
}

const bool operator != ( const num_t & u, const num_t & v ) {
  return u . Compare ( v ) != 0;
}

/**
 * Сложение z = u + v.
 * Память для z должна быть выделена заранее.
 */
static void add ( num_t & z, const num_t & u, const num_t & v ) {
  vec_t vec_z ( z ), vec_u ( u ), vec_v ( v );
  sign_t s;
  if ( ( u . Sign ( ) ^ v . Sign ( ) ) < 0 ) {
    s = :: fullSub ( vec_z, vec_u, vec_v );
    if ( u . Sign ( ) < 0 )  s = - s;
  }
  else {
    add ( vec_z, vec_u, vec_v );
    s = u . Sign ( ) < 0 ? -1 : 1;
  }
  z . size = ( offset_t ) s * vec_z . size;
  vec_u . FreeAlias ( );
  vec_v . FreeAlias ( );
  vec_z . FreeAlias ( );
}

/**
 * Сложение. Память выделяется здесь же.
 */
num_t operator + ( const num_t & u, const num_t & v ) {
  num_t z ( max ( abs ( u . size ), abs ( v . size ) ) + 1 );
  add ( z, u, v );
  return z;
}

/**
 * Вычитание z = u - v.
 * Память для 'z' должна быть выделена заранее.
 */
static void sub ( num_t & z, const num_t & u, const num_t & v ) {
  vec_t vec_z ( z ), vec_u ( u ), vec_v ( v );
  sign_t s;
  if ( ( u . Sign ( ) ^ v . Sign ( ) ) < 0 ) {
    :: add ( vec_z, vec_u, vec_v );
    s = u . Sign ( ) < 0 ? -1 : 1;
  }
  else {
    s = :: fullSub ( vec_z, vec_u, vec_v );
    if ( u . Sign ( ) < 0 ) s = - s;
  }
  z . size = ( offset_t ) s * vec_z . size;
  vec_u . FreeAlias ( );
  vec_v . FreeAlias ( );
  vec_z . FreeAlias ( );
}

/**
 * Вычитание. Память выделяется здесь же.
 */
num_t operator - ( const num_t & u, const num_t & v ) {
  num_t z ( max ( abs ( u . size ), abs ( v . size ) ) + 1 );
  sub ( z, u, v );
  return z;
}

/**
 * Умножение на лимб
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void mul1 ( num_t & z, const num_t & u, limb_t v ) {
  vec_t vec_z ( z ), vec_u ( u );
  mul1 ( vec_z, vec_u, v );
  if ( u . size < 0 ) z . size = - ( offset_t ) vec_z . size;
  else z . size = ( offset_t ) vec_z . size;
  vec_z . FreeAlias ( );
  vec_u . FreeAlias ( );
}

/**
 * Умножение u * v.
 * Нужная память выделяется здесь же.
 */
num_t operator * ( const num_t & u, limb_t v ) {
  num_t z ( abs ( u . size ) + 1 );
  mul1 ( z, u, v );
  return z;
}

/**
 * Умножение на вектор
 * Память в 'z' должна быть выделена заранее в достаточном объёме.
 */
static void mul ( num_t & z, const num_t & u, const num_t & v ) {
  vec_t vec_z ( z ), vec_u ( u ), vec_v ( v );
  mul ( vec_z, vec_u, vec_v );
  if ( ( u . size ^ v . size ) < 0 ) z . size = - ( offset_t ) vec_z . size;
  else z . size = ( offset_t ) vec_z . size;
  vec_z . FreeAlias ( );
  vec_u . FreeAlias ( );
  vec_v . FreeAlias ( );
}

/**
 * Умножение u * v.
 * Нужная память выделяется здесь же.
 */
num_t operator * ( const num_t & u, const num_t & v ) {
  num_t z ( abs ( u . size ) + abs ( v . size ) );
  mul ( z, u, v );
  return z;
}

/**
 * Деление на лимб
 */
static void div_qr ( num_t * z, slimb_t * r, const num_t & u, slimb_t v ) {
  bool is_neg = ( u . Sign ( ) < 0 ) ^ ( v < 0 );
  if ( z != nullptr ) {
    vec_t vec_z ( * z ), vec_u ( u );
    div_qr ( & vec_z, ( limb_t * ) r, vec_u, ( limb_t ) abs ( v ) );
    if ( is_neg ) z -> size = - ( offset_t ) vec_z . size;
    else z -> size = ( offset_t ) vec_z . size;
    vec_z . FreeAlias ( );
    vec_u . FreeAlias ( );
  } else {
    vec_t vec_u ( u );
    div_qr ( nullptr, ( limb_t * ) r, vec_u, ( limb_t ) abs ( v ) );
    vec_u . FreeAlias ( );
  }
  if ( r != nullptr && u . Sign ( ) < 0 ) * r = - * r;
}

/**
 * Деление u / v.
 * Нужная память выделяется здесь же.
 */
num_t operator / ( const num_t & u, slimb_t v ) {
  num_t z ( abs ( u . size ) );
  div_qr ( & z, nullptr, u, v );
  return z;
}

/**
 * Остаток от деления u % v.
 * Нужная память выделяется здесь же.
 */
slimb_t operator % ( const num_t & u, slimb_t v ) {
  slimb_t r;
  div_qr ( nullptr, & r, u, v );
  return r;
}



/**
 * Класс для генерации псевдослучайных векторов.
 */
class rnd_t {
  private:
    const limb_t A = 19993;
    limb_t r = 239;
  public:

    /**
     * Сгенерировать лимб.
     */
    limb_t operator ( ) ( ) {
      return r = A * r + 1;
    }

    /**
     * Сгенерировать нормализованный вектор размером u . size
     * Объём выделенной ранее памяти для limbs должен быть не меньше u . size
     */
    void operator ( ) ( vec_t & u ) {
      for ( size_t i = 0; i < u . size; i ++ )
        u . limbs [ i ] = this -> operator ( ) ( );
      // Обеспечим чтобы старший лимб не был равен нулю.
      while ( u . limbs [ u . size - 1 ] == 0 )
        u . limbs [ u . size - 1 ] = this -> operator ( ) ( );
    }

    /**
     * Сгенерировать нормализованный вектор размером s.
     * Память выделяется заново, старый вектор стирается.
     */
    void operator ( ) ( vec_t & u, size_t s ) {
      u . size = s;
      delete [ ] u . limbs;
      if ( s == 0 )  {
        u . limbs = nullptr;
        return;
      }
      u . limbs = new limb_t [ s ];
      this -> operator ( ) ( u );
    }

    /**
     * Сгенерировать нормализованный знаковый вектор размером n.size
     * Объём выделенной ранее памяти для limbs должен быть не меньше |n.size|
     */
    void operator ( ) ( num_t & n ) {
      vec_t vec_n;
      vec_n . Alias ( n );
      this -> operator ( ) ( vec_n );
      vec_n . FreeAlias ( );
    }

    /**
     * Сгенерировать нормализованный знаковый вектор размером s.
     * Память выделяется заново, старый вектор стирается.
     */
    void operator ( ) ( num_t & n, size_t s ) {
      vec_t vec_n;
      vec_n . Alias ( n );
      this -> operator ( ) ( vec_n, s );
      vec_n . FreeAlias ( );
    }

};

rnd_t rnd;
