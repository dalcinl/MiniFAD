#include "minifadtest.hpp"

#define PROMOTE_FLOATSTD(NS,T)                                    \
namespace NS {                                                    \
  typedef MiniFAD::promote< int, T >::type t0;                    \
  typedef MiniFAD::promote< float, T >::type t1;                  \
  typedef MiniFAD::promote< double, T >::type t2;                 \
  typedef MiniFAD::promote< long double, T >::type t3;            \
}
#if MINIFAD_HAVE_FLOAT128
#define PROMOTE_FLOAT128(NS,T)                                    \
namespace NS {                                                    \
  typedef MiniFAD::promote< __float128, T >::type t3x;            \
}
#endif


#if MINIFAD_HAVE_COMPLEX_CXX
#define PROMOTE_COMPLEX_CXX(NS,T)                                     \
namespace NS {                                                        \
  typedef MiniFAD::promote< std::complex<float>, T >::type t4;        \
  typedef MiniFAD::promote< std::complex<double>, T >::type t5;       \
  typedef MiniFAD::promote< std::complex<long double>, T >::type t6;  \
}
#endif

#if MINIFAD_HAVE_COMPLEX_C99
#define PROMOTE_COMPLEX_C99(NS,T)                                 \
namespace NS {                                                    \
  typedef MiniFAD::promote< float _Complex, T >::type t7;         \
  typedef MiniFAD::promote< double _Complex, T >::type t8;        \
  typedef MiniFAD::promote< long double _Complex, T >::type t9;   \
}
#endif

#ifndef PROMOTE_FLOAT128
#define PROMOTE_FLOAT128(NS,T)
#endif
#ifndef PROMOTE_COMPLEX_CXX
#define PROMOTE_COMPLEX_CXX(NS,T)
#endif
#ifndef PROMOTE_COMPLEX_C99
#define PROMOTE_COMPLEX_C99(NS,T)
#endif

#define \
PROMOTE(NS,T) \
PROMOTE_FLOATSTD(NS,T) \
PROMOTE_FLOAT128(NS,T) \
PROMOTE_COMPLEX_CXX(NS,T) \
PROMOTE_COMPLEX_C99(NS,T)
namespace A {
PROMOTE(N1,char)
PROMOTE(N2,signed char)
PROMOTE(N3,unsigned char)
PROMOTE(N4,short)
PROMOTE(N5,unsigned short)
PROMOTE(N6,int)
PROMOTE(N7,unsigned int)
PROMOTE(N8,long)
PROMOTE(N9,unsigned long)
}
#undef PROMOTE

#define \
PROMOTE(NS,T) \
PROMOTE_FLOATSTD(NS,T) \
PROMOTE_FLOAT128(NS,T) \
PROMOTE_COMPLEX_CXX(NS,T)
namespace B {
PROMOTE(N1,float)
PROMOTE(N2,double)
PROMOTE(N3,long double)
#if MINIFAD_HAVE_FLOAT128
PROMOTE(N4,__float128)
#endif
#if MINIFAD_HAVE_COMPLEX_CXX
PROMOTE(N5,std::complex<float>)
PROMOTE(N6,std::complex<double>)
PROMOTE(N7,std::complex<long double>)
#if MINIFAD_HAVE_FLOAT128
PROMOTE(N8,std::complex<__float128>)
#endif
#endif
}
#undef PROMOTE

#define \
PROMOTE(NS,T) \
PROMOTE_FLOATSTD(NS,T) \
PROMOTE_FLOAT128(NS,T) \
PROMOTE_COMPLEX_C99(NS,T)
namespace C {
PROMOTE(N1,float)
PROMOTE(N2,double)
PROMOTE(N3,long double)
#if MINIFAD_HAVE_FLOAT128
PROMOTE(N4,__float128)
#endif
#if MINIFAD_HAVE_COMPLEX_C99
PROMOTE(N5,float _Complex)
PROMOTE(N6,double _Complex)
PROMOTE(N7,long double _Complex)
#if MINIFAD_HAVE_FLOAT128
PROMOTE(N8,__float128 _Complex)
#endif
#endif
}
#undef PROMOTE

int main(int,char *[])
{
  return 0;
}
