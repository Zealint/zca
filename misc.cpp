/*
**  Здесь реализованы разные функции, упрощающие кодирование.
**  Данный файл является вспомогательным для Int.cpp и включается в него.
*/

template < typename T > void swap ( T & a, T & b ) {
  T t = a;
  a = b;
  b = t;
}

template < typename T > T max ( T a, T b ) {
  if ( a >= b )  return a;
  return b;
}

template < typename T > T min ( T a, T b ) {
  if ( a <= b )  return a;
  return b;
}

size_t abs ( offset_t a ) {
  if ( a < 0 )  a = -a;
  return ( size_t ) a;    
}

/*
limb_t abs ( slimb_t a ) {
  if ( a < 0 )  a = -a;
  return ( limb_t ) a;    
}
*/

sign_t sign ( offset_t a ) {
  return sign_t ( a > 0 ) - sign_t ( a < 0 );
}

// Подсчёт лидирующих нулей в лимби
static u8 __fastcall count_lz ( limb_t x ) {
  u8 count = 0;
  if ( ( x & 0xFFFF0000 ) == 0 )  count += 16;
  else  x >>= 16;
  if ( ( x & 0xFF00 ) == 0 )  count += 8;
  else  x >>= 8;
  if ( ( x & 0xF0 ) == 0 )  count += 4;
  else  x >>= 4;
  if ( ( x & 0xC ) == 0 )  count += 2;
  else  x >>= 2;
  if ( ( x & 2 ) == 0 )  count += 1;
  else  x >>= 1;
  if ( x == 0 )  count += 1;
  return count;
}
