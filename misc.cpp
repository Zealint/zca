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

sign_t sign ( offset_t a ) {
  return sign_t ( a > 0 ) - sign_t ( a < 0 );
}