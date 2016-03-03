/**
 * ����� ������� �������� ������� ��� ������ � �������� �������
 */

#include <assert.h>

/**
 * ������� ���� ������ (��� ��������)
 */
typedef unsigned int limb_t;         // ���� ���� - 32 ���� ��� �����
typedef unsigned long long dlimb_t;  // ������� ���� - 64 ���� ��� �����
typedef char sign_t;                 // ���� (-1, 0, 1)
typedef int offset_t;                // �������� ������ �������� �����

/**
 * ������� ���������
 */
const size_t LIMB_SIZE = sizeof ( limb_t );
const size_t LIMB_BITS = LIMB_SIZE * 8;
const limb_t LIMB_T_MAX = 0xFFFFFFFF;

// ��������� ������� ��������� ����������� � ��������� ������
// ��� �������� � �������
#include "misc.cpp" // ��������������� ������� (abs, min, max, sign, ...).
#include "add.cpp"  // ��, ��� �������� �������� ��������.
#include "sub.cpp"  // ��������� ��������.
#include "mul.cpp"  // ��������� ��������.

class num_t;

/**
 * ������ ������. 
 * ������ ��� ������� ������ � �������.
 */
class vec_t {
  public:    
    size_t size;     // ������ (�� ����������� ����������������) �������.
    limb_t * limbs;  // ������ ������.

    /** 
     * ������������ � ���������� 
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
    // ��������� ������ ��� ����� � ������� �� ������ (��� � "Alias", ����).
    vec_t ( const num_t & n );
    ~ vec_t ( ) { delete [ ] limbs; }

    /**
     * ��������� ������ ��� ����� � ������� �� ������.
     * ������ ������ �� ������ (size=abs(n.size)), 
     * � ��������� �� ����� ��������������� �� ��������� n.limbs.
     */
    void Alias ( const num_t & n );

    /**
     * �������� �������� ������.
     * ��� ����� ����� �������� �������, ����� ��� ������ �����������
     * �� ������� ��� ������ �� ������, � �������� �� �����������.
     */
    void FreeAlias ( ) {
      limbs = nullptr;
    }

    /**
     * ������������� ������.
     * ������ size �� ��������� ���������� ���������� ������ � limbs.
     * ���������� ��������������� ������.
     */
    size_t Normalize ( ) {
      while ( size > 0 && limbs [ size - 1 ] == 0 )
        size --;
      return size;
    }

    /**
     * �������� � ������ �������� ���� �� �������.
     * size = v.size
     * size > 0
     * ���������� ��������� ��������� � ���� �����.
     */
    sign_t CompareEqualSize ( const vec_t & v ) const {
      size_t i = size;
      while ( -- i > 0 && limbs [ i ] == v . limbs [ i ] );
      if ( limbs [ i ] < v . limbs [ i ] )  return -1;
      if ( limbs [ i ] > v . limbs [ i ] )  return 1;
      return 0;
    }

    /**
     * �������� � ������ ��������.
     * ��� ������� ������ ���� ����������������.
     * ���������� ��������� ��������� � ���� �����.
     */
    sign_t Compare ( const vec_t & v ) const {
      if ( size < v . size )  return -1;
      if ( size > v . size )  return 1;
      if ( size == 0 )  return 0;
      return CompareEqualSize ( v );
    }

    /**
     * ��������� ���� �������� ������� �������.
     * ������ ����� ������ ������ ���� ������� �������.
     * ���������� ������ �� ����.
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
     * ����������� ���� �������� ������� �������.
     * ���������� ����� ������ ���������.
     * ���������� ������ �� ����.
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
     * ��������������� ������� [] ��� ����� �������� ��������� � ������ �������.
     */
    limb_t & operator [ ] ( size_t index ) {  return limbs [ index ];  }
    const limb_t & operator [ ] ( size_t index ) const {  return limbs [ index ];  }

    /**
     * ��������� ������ 'v'
     * ����������� ������ ������ ���� �������� �������.
     */
    size_t add ( const vec_t & v ) {
      // ���������� ������� �� ����� add.cpp 
      return size = :: add ( limbs, size, v . limbs, v . size );
    }

    /**
     * ��������� ������ 'v'
     * ������ ��� add (����).
     */
    const vec_t & operator += ( const vec_t & v ) {      
      this -> add ( v );
      return * this;
    }

    /**
     * ������ ������ 'v'.
     * ������ 'v' ������ ���� �� ������ ������������ (this).
     */
    size_t sub ( const vec_t & v ) {
      // ���������� ������� �� ����� sub.cpp
      bool isCorrect ( :: sub ( limbs, size, v . limbs, v . size ) );
      assert ( isCorrect ); // ��������, ��� �� ���� ����.
      return Normalize ( ); // ��� ��������� ������������ �����������.
    }

    /**
     * ������ ������ 'v'.
     * ������ ��� sub (����).
     */
    const vec_t & operator -= ( const vec_t & v ) {      
      this -> sub ( v );
      return * this;
    }

    /**
     * �������� �� ����
     * ����������� ������ ������ ���� �������� �������.
     */
    size_t mul1 ( limb_t v ) {
      if ( size == 0 || v == 0 ) return size = 0;      
      // ���������� ������� �� ����� mul.cpp 
      return size = :: mul1 ( limbs, size, v );
    }

    /**
     * �������� �� ����
     * ������ ��� mul1 (����).
     */
    const vec_t & operator *= ( limb_t v ) {      
      this -> mul1 ( v );
      return * this;
    }
    
    /**
     * ������� ���������
     */
    friend const bool operator < ( const vec_t &, const vec_t & );
    friend const bool operator > ( const vec_t &, const vec_t & );
    friend const bool operator <= ( const vec_t &, const vec_t & );
    friend const bool operator >= ( const vec_t &, const vec_t & );
    friend const bool operator == ( const vec_t &, const vec_t & );
    friend const bool operator != ( const vec_t &, const vec_t & );

    friend vec_t operator + ( const vec_t &, const vec_t & );
    friend vec_t operator - ( const vec_t &, const vec_t & );

};

/**
 * ���������� ������� ����������
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
 * ���������� ������� ��� �������� �������� vec_t ����� ������� �� add.cpp
 * ������ � 'z' ������ ���� �������� ������� � ����������� ������.
 */
static void add ( vec_t & z, const vec_t & u, const vec_t & v ) {
  // ���������� ������� �� ����� add.cpp  
  if ( z . limbs == u . limbs ) // �������� "�� �����" u += v
    z . size = :: add ( z . limbs, u . size, v . limbs, v . size );
  else if ( z . limbs == v . limbs )  // �������� "�� �����" v += u
    z . size = :: add ( z . limbs, v . size, u . limbs, u . size );
  else // �� "�� �����"
    z . size = :: add ( z . limbs, u . limbs, u . size, v . limbs, v . size );
}

/**
 * �������� u + v. 
 * ������ ������ ���������� ����� ��.
 */
vec_t operator + ( const vec_t & u, const vec_t & v ) {  
  vec_t z ( max ( u . size, v . size ) + 1 );
  add ( z, u, v );
  return z;
}

/**
 * ���������� ������� ��� ��������� �������� vec_t ����� ������� �� sub.cpp
 * ������ � 'z' ������ ���� �������� ������� � ����������� ������.
 * u >= v.
 */
static void sub ( vec_t & z, const vec_t & u, const vec_t & v ) {  
  if ( z . limbs == u . limbs ) // "�� �����", u-=v.
    :: sub ( z . limbs, u . size, v . limbs, v . size );  
  else // �� "�� �����".
    :: sub ( z . limbs, u . limbs, u . size, v . limbs, v . size );  
  z . size = u . size;
  z . Normalize ( );
}

/**
 * ��������� u - v.
 * u >= v. 
 * ������ ���������� ����� ��.
 */
vec_t operator - ( const vec_t & u, const vec_t & v ) {
  vec_t z ( u . size );
  sub ( z, u, v );
  return z;
}

/**
 * ���������� ������� ��� ��������� ������� vec_t �� ���� ����� ������� �� mul.cpp
 * ������ � 'z' ������ ���� �������� ������� � ����������� ������.
 */
static void mul1 ( vec_t & z, const vec_t & u, limb_t v ) {
  if ( u . size == 0 || v == 0 ) 
    z . size = 0;
  else 
    // ���������� ������� �� ����� mul.cpp  
    z . size = mul1 ( z . limbs, u . limbs, u . size, v );
}

/**
 * ��������� u * v. 
 * ������ ������ ���������� ����� ��.
 */
vec_t operator * ( const vec_t & u, limb_t v ) {  
  vec_t z ( u . size + 1 );
  mul1 ( z, u, v );
  return z;
}

/**
 * ������� �� �������� ������� �������.
 * ��������������� ������� ��� ��������� �������� �������� (����).
 */
static sign_t fullSub ( vec_t & z, const vec_t & u, const vec_t & v ) {  
  sign_t s = u . Compare ( v );  // ��� ������?
  if ( s == 0 ) z . size = 0; // � ������ ��������� ��������� 0.
  else if ( s > 0 ) {         // u > v
    if ( z . limbs == u . limbs ) z -= v; // ���� ��� ��������� "�� �����".
    else :: sub ( z, u, v ); // �� "�� �����".
  }
  else { // u < v
    if ( z . limbs == v . limbs ) z -= u;  // "�� �����".
    else :: sub ( z, v, u ); // �� "�� �����".
  }
  return s;
}

/**
 * �������� ������ ������. 
 * ������ ��� ������� ������ � ��������� �������.
 */
class num_t {
  public:    
    offset_t size;   // �������� ������ (�� ����������� ����������������) �������.
    limb_t * limbs;  // ������ ������.

    /** 
     * ������������ � ���������� 
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
     * �������� ����.
     */
    sign_t Sign ( ) const {
      return sign ( size );
    }

    /**
     * ������������� ������.
     * ������ |size| �� ��������� ���������� ���������� ������ � limbs.
     * ���������� ��������������� ������.
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
     * �������� � ������ �������� ��������.
     * ��� ������� ������ ���� ����������������.
     * ���������� ��������� ��������� � ���� �����.
     */
    sign_t Compare ( const num_t & n ) const {
      if ( size < n . size )  return -1;
      if ( size > n . size )  return 1;
      if ( size == 0 )  return 0;
      // ��������� � ����������� ��������.
      vec_t vec ( * this ), vec_n ( n );      
      sign_t res = vec . CompareEqualSize ( vec_n );
      vec . FreeAlias ( );
      vec_n . FreeAlias ( );
      return res * Sign ( );
    }

    /**
     * ��������� ���� �������� ������� �������.
     * ������ ����� ������ ������ ���� ������� �������.
     * ���������� ������ �� ����.
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
     * ����������� ���� �������� ������� �������.
     * ���������� ����� ������ ���������.
     * ���������� ������ �� ����.
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
     * ��������������� ������� [] ��� ����� �������� ��������� � ������ �������.
     */
    limb_t & operator [ ] ( size_t index ) {  return limbs [ index ];  }
    const limb_t & operator [ ] ( size_t index ) const {  return limbs [ index ];  }

    /**
     * ��������� ������ 'n'
     * ����������� ������ ������ ���� �������� �������.
     */
    offset_t add ( const num_t & n ) {      
      vec_t vec ( * this ), vec_n ( n );      
      sign_t s;
      // ���������� ������� �����.
      if ( ( Sign ( ) ^ n . Sign ( ) ) < 0 ) {
        s = :: fullSub ( vec, vec, vec_n );
        if ( Sign ( ) < 0 )  s = -s;
      }
      // ���������� ������ �����.
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
     * ��������� ������ 'n'
     * ������ ��� add (����).
     */
    const num_t & operator += ( const num_t & n ) {
      this -> add ( n );
      return * this;
    }

    /**
     * ������ ������ 'n'.
     */
    offset_t sub ( const num_t & n ) {
      vec_t vec ( * this ), vec_n ( n );      
      sign_t s;
      // ���������� ������� �����.
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
     * ������ ������ 'n'
     * ������ ��� sub (����).
     */
    const num_t & operator -= ( const num_t & n ) {      
      this -> sub ( n );
      return * this;
    }

    /**
     * �������� �� ����
     * ����������� ������ ������ ���� �������� �������.
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
     * �������� �� ����
     * ������ ��� mul1 (����).
     */
    const num_t & operator *= ( limb_t v ) {      
      this -> mul1 ( v );
      return * this;
    }

    /**
     * ������� ���������
     */
    friend const bool operator < ( const num_t &, const num_t & );
    friend const bool operator > ( const num_t &, const num_t & );
    friend const bool operator <= ( const num_t &, const num_t & );
    friend const bool operator >= ( const num_t &, const num_t & );
    friend const bool operator == ( const num_t &, const num_t & );
    friend const bool operator != ( const num_t &, const num_t & );

    friend num_t operator + ( const num_t &, const num_t & );
    friend num_t operator - ( const num_t &, const num_t & );

};

/**
 * ���������� �������� ������� ��� ����� � ������� �� ������.
 */
vec_t :: vec_t ( const num_t & n ) {
  Alias ( n );
}

void vec_t :: Alias ( const num_t & n ) {
  size = abs ( n . size );
  limbs = n . limbs;
}

/**
 * ���������� ������� ���������� ��� num_t
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
 * �������� z = u + v.
 * ������ ��� z ������ ���� �������� �������.
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
 * ��������. ������ ���������� ����� ��.
 */
num_t operator + ( const num_t & u, const num_t & v ) {
  num_t z ( max ( abs ( u . size ), abs ( v . size ) ) + 1 );
  add ( z, u, v );
  return z;
}

/**
 * ��������� z = u - v.
 * ������ ��� 'z' ������ ���� �������� �������.
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
 * ���������. ������ ���������� ����� ��.
 */
num_t operator - ( const num_t & u, const num_t & v ) {
  num_t z ( max ( abs ( u . size ), abs ( v . size ) ) + 1 );
  sub ( z, u, v );
  return z;
}

/**
 * ��������� �� ����
 * ������ � 'z' ������ ���� �������� ������� � ����������� ������.
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
 * ��������� u * v. 
 * ������ ������ ���������� ����� ��.
 */
num_t operator * ( const num_t & u, limb_t v ) {  
  num_t z ( abs ( u . size ) + 1 );
  mul1 ( z, u, v );
  return z;
}

/**
 * ����� ��� ��������� ��������������� ��������.
 */
class rnd_t {
  private:
    const limb_t A = 19993;
    limb_t r = 239;
  public:

    /**
     * ������������� ����.     
     */
    limb_t operator ( ) ( ) {
      return r = A * r + 1;
    }

    /**
     * ������������� ��������������� ������ �������� u . size
     * ����� ���������� ����� ������ ��� limbs ������ ���� �� ������ u . size
     */
    void operator ( ) ( vec_t & u ) {      
      for ( size_t i = 0; i < u . size; i ++ )
        u . limbs [ i ] = this -> operator ( ) ( );
      // ��������� ����� ������� ���� �� ��� ����� ����.
      while ( u . limbs [ u . size - 1 ] == 0 )
        u . limbs [ u . size - 1 ] = this -> operator ( ) ( );
    }

    /**
     * ������������� ��������������� ������ �������� s.
     * ������ ���������� ������, ������ ������ ���������.
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
     * ������������� ��������������� �������� ������ �������� n.size
     * ����� ���������� ����� ������ ��� limbs ������ ���� �� ������ |n.size|
     */
    void operator ( ) ( num_t & n ) {      
      vec_t vec_n;
      vec_n . Alias ( n );
      this -> operator ( ) ( vec_n );
      vec_n . FreeAlias ( );
    }

    /**
     * ������������� ��������������� �������� ������ �������� s.
     * ������ ���������� ������, ������ ������ ���������.
     */
    void operator ( ) ( num_t & n, size_t s ) {
      vec_t vec_n;
      vec_n . Alias ( n );
      this -> operator ( ) ( vec_n, s );
      vec_n . FreeAlias ( );
    }

};

rnd_t rnd;
