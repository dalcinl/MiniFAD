// Author:  Lisandro Dalcin
// Contact: dalcinl@gmail.com
// License: http://opensource.org/licenses/MIT

#ifndef MINIFAD_HPP
#define MINIFAD_HPP

// -----

#include <cstddef>

#ifndef MINIFAD_HAVE_COMPLEX_CXX
#  define MINIFAD_HAVE_COMPLEX_CXX 1
#endif

#ifndef MINIFAD_HAVE_COMPLEX_C99
#  if defined(__GNUC__) || defined(__clang__)
#    define MINIFAD_HAVE_COMPLEX_C99 1
#  else
#    define MINIFAD_HAVE_COMPLEX_C99 0
#  endif
#endif

#ifndef MINIFAD_HAVE_TR1_CMATH
#  if defined(_GLIBCXX_USE_C99_MATH_TR1)
#    define MINIFAD_HAVE_TR1_CMATH 1
#  else
#    define MINIFAD_HAVE_TR1_CMATH 0
#  endif
#endif

#ifndef MINIFAD_HAVE_TR1_COMPLEX
#  if defined(_GLIBCXX_USE_C99_COMPLEX_TR1)
#    define MINIFAD_HAVE_TR1_COMPLEX 1
#  else
#    define MINIFAD_HAVE_TR1_COMPLEX 0
#  endif
#endif

#ifndef MINIFAD_HAVE_FLOAT128
#  if defined(__GNUC__) && (__GNUC__*10+__GNUC_MINOR__) > 46
#    define MINIFAD_HAVE_FLOAT128 1
#  else
#    define MINIFAD_HAVE_FLOAT128 0
#  endif
#endif

#ifndef MINIFAD_HAVE_LIBQUADMATH
#  if defined(QUADMATH_H)
#    define MINIFAD_HAVE_LIBQUADMATH 1
#  else
#    define MINIFAD_HAVE_LIBQUADMATH 0
#  endif
#endif

// -----

#if MINIFAD_HAVE_COMPLEX_C99
#  include <complex.h>
#  if defined(__APPLE__) && defined(__clang__)
#    define __STDC_VERSION__ 0
#    include "/usr/include/complex.h" // @XXX
#    undef __STDC_VERSION__
#  endif
#endif

#if MINIFAD_HAVE_COMPLEX_CXX
#  undef complex
#  include <complex>
#  if MINIFAD_HAVE_TR1_COMPLEX
#    include <tr1/complex>
#  endif
#endif

#include <cmath>
#if MINIFAD_HAVE_TR1_CMATH
#  include <tr1/cmath>
#endif

#if MINIFAD_HAVE_LIBQUADMATH
#  include <quadmath.h>
#endif

// -----

// C++ std::complex<T> workarounds
#if MINIFAD_HAVE_COMPLEX_CXX
namespace MiniFAD {

#define MINIFAD_COMPLEX_CXX_(OP,U,T)                                 \
/* complex @ complex */                                              \
inline std::complex<U> operator OP                                   \
(const std::complex<T>& lhs, const std::complex<U>& rhs)             \
{return std::complex<U>(lhs) OP std::complex<U>(rhs);}               \
inline std::complex<U> operator OP                                   \
(const std::complex<U>& lhs, const std::complex<T>& rhs)             \
{return std::complex<U>(lhs) OP std::complex<U>(rhs);}               \
/* complex @ real */                                                 \
inline std::complex<U> operator OP                                   \
(const std::complex<T>& lhs, const U& rhs)                           \
{return std::complex<U>(lhs) OP std::complex<U>::value_type(rhs);}   \
inline std::complex<U> operator OP                                   \
(const std::complex<U>& lhs, const T& rhs)                           \
{return std::complex<U>(lhs) OP std::complex<U>::value_type(rhs);}   \
/* real @ complex */                                                 \
inline std::complex<U> operator OP                                   \
(const T& lhs, const std::complex<U>& rhs)                           \
{return std::complex<U>::value_type(lhs) OP std::complex<U>(rhs);}   \
inline std::complex<U> operator OP                                   \
(const U& lhs, const std::complex<T>& rhs)                           \
{return std::complex<U>::value_type(lhs) OP std::complex<U>(rhs);}  //

#if MINIFAD_HAVE_FLOAT128
#define MINIFAD_COMPLEX_CXX_OP(OP)                 \
MINIFAD_COMPLEX_CXX_(OP,double,float)              \
MINIFAD_COMPLEX_CXX_(OP,long double,float)         \
MINIFAD_COMPLEX_CXX_(OP,long double,double)        \
MINIFAD_COMPLEX_CXX_(OP,__float128,float)          \
MINIFAD_COMPLEX_CXX_(OP,__float128,double)         \
MINIFAD_COMPLEX_CXX_(OP,__float128,long double)   //
#else                                             //
#define MINIFAD_COMPLEX_CXX_OP(OP)                 \
MINIFAD_COMPLEX_CXX_(OP,double,float)              \
MINIFAD_COMPLEX_CXX_(OP,long double,float)         \
MINIFAD_COMPLEX_CXX_(OP,long double,double)       //
#endif

MINIFAD_COMPLEX_CXX_OP(+)
MINIFAD_COMPLEX_CXX_OP(-)
MINIFAD_COMPLEX_CXX_OP(*)
MINIFAD_COMPLEX_CXX_OP(/)

#undef MINIFAD_COMPLEX_CXX_
#undef MINIFAD_COMPLEX_CXX_OP

} // namespace MiniFAD
#endif // MINIFAD_HAVE_COMPLEX_CXX

// -----

// Type promotion rules
namespace MiniFAD {

template <typename T, typename U>
struct promote { typedef float type; };

#define MINIFAD_PROMOTE1(T)                                     \
template <typename X> struct promote<T,X> { typedef T type; };  \
template <typename X> struct promote<X,T> { typedef T type; };  \
template <> struct promote<T,T> { typedef T type; };           //
#define MINIFAD_PROMOTE2(T,U)                                   \
template <> struct promote<T,U> { typedef T type; };            \
template <> struct promote<U,T> { typedef T type; };           //
#define MINIFAD_PROMOTE3(T,U,V)                                 \
template <> struct promote<U,V> { typedef T type; };            \
template <> struct promote<V,U> { typedef T type; };           //

#define MINIFAD_REAL_PROMOTE1(Real,T,U) MINIFAD_PROMOTE1(T)
#define MINIFAD_REAL_PROMOTE2(Real,T,U) MINIFAD_PROMOTE2(T,U)
#define MINIFAD_COMPLEX_CXX(T) std::complex<T> //
#define MINIFAD_COMPLEX_C99(T) T _Complex      //
#define MINIFAD_COMPLEX_PROMOTE1(Complex,T,U)   \
MINIFAD_PROMOTE1( Complex(T)    )               \
MINIFAD_PROMOTE2( Complex(T), T )              //
#define MINIFAD_COMPLEX_PROMOTE2(Complex,T,U)   \
MINIFAD_PROMOTE2( Complex(T), U             )   \
MINIFAD_PROMOTE2( Complex(T), Complex(U)    )   \
MINIFAD_PROMOTE3( Complex(T), Complex(U), T )  //

#define MINIFAD_PROMOTE_FLOATSTD(CLASS,LANG)          \
CLASS##PROMOTE1(CLASS##LANG,float,float)              \
CLASS##PROMOTE2(CLASS##LANG,double,float)             \
CLASS##PROMOTE1(CLASS##LANG,double,double)            \
CLASS##PROMOTE2(CLASS##LANG,long double,float)        \
CLASS##PROMOTE2(CLASS##LANG,long double,double)       \
CLASS##PROMOTE1(CLASS##LANG,long double,long double) //
#define MINIFAD_PROMOTE_FLOAT128(CLASS,LANG)          \
CLASS##PROMOTE2(CLASS##LANG,__float128,float)         \
CLASS##PROMOTE2(CLASS##LANG,__float128,double)        \
CLASS##PROMOTE2(CLASS##LANG,__float128,long double)   \
CLASS##PROMOTE1(CLASS##LANG,__float128,__float128)   //
#if !MINIFAD_HAVE_FLOAT128
#undef  MINIFAD_PROMOTE_FLOAT128
#define MINIFAD_PROMOTE_FLOAT128(CLASS,LANG)
#endif

#define MINIFAD_PROMOTE(CLASS,LANG)                \
MINIFAD_PROMOTE_FLOATSTD(MINIFAD_##CLASS##_,LANG)  \
MINIFAD_PROMOTE_FLOAT128(MINIFAD_##CLASS##_,LANG) //

MINIFAD_PROMOTE(REAL,CXX)
#if MINIFAD_HAVE_COMPLEX_CXX
MINIFAD_PROMOTE(COMPLEX,CXX)
#endif
#if MINIFAD_HAVE_COMPLEX_C99
MINIFAD_PROMOTE(COMPLEX,C99)
#endif

#undef MINIFAD_PROMOTE1
#undef MINIFAD_PROMOTE2
#undef MINIFAD_PROMOTE3
#undef MINIFAD_REAL_PROMOTE1
#undef MINIFAD_REAL_PROMOTE2
#undef MINIFAD_COMPLEX_CXX
#undef MINIFAD_COMPLEX_C99
#undef MINIFAD_COMPLEX_PROMOTE1
#undef MINIFAD_COMPLEX_PROMOTE2
#undef MINIFAD_PROMOTE_FLOATSTD
#undef MINIFAD_PROMOTE_FLOAT128
#undef MINIFAD_PROMOTE

} // namespace MiniFAD

// -----

// Mathematical functions
namespace MiniFAD { namespace math {

#define MINIFAD_REAL_CXX(T)  T
#define MINIFAD_REAL_C99(T)  T
#define MINIFAD_CMPLX_CXX(T) std::complex<T>
#define MINIFAD_CMPLX_C99(T) T _Complex

#define MINIFAD_FN_1(CLASS,LANG,type,name,call)  \
inline MINIFAD_##CLASS##_##LANG(type)            \
name(const MINIFAD_##CLASS##_##LANG(type)& x)    \
{return call(x);}                               //
#define MINIFAD_FN_2(CLASS,LANG,type,name,call)  \
inline MINIFAD_##CLASS##_##LANG(type)            \
name(const MINIFAD_##CLASS##_##LANG(type)& x,    \
     const MINIFAD_##CLASS##_##LANG(type)& y)    \
{return call(x,y);}                             //
#define MINIFAD_FN_3(CLASS,LANG,type,name,call)  \
inline type                                      \
name(const MINIFAD_##CLASS##_##LANG(type)& x)    \
{return call(x);}

#define MINIFAD_FN_R_nil(ID,name,call)                        //
#define MINIFAD_FN_R_STD(ID,name,call)                         \
MINIFAD_FN_##ID(REAL, CXX, float      , name, std::call)       \
MINIFAD_FN_##ID(REAL, CXX, double     , name, std::call)       \
MINIFAD_FN_##ID(REAL, CXX, long double, name, std::call)      //
#define MINIFAD_FN_R_TR1(ID,name,call)                         \
MINIFAD_FN_##ID(REAL, CXX, float      , name, std::tr1::call)  \
MINIFAD_FN_##ID(REAL, CXX, double     , name, std::tr1::call)  \
MINIFAD_FN_##ID(REAL, CXX, long double, name, std::tr1::call) //
#define MINIFAD_FN_R_C99(ID,name,call)                         \
MINIFAD_FN_##ID(REAL, C99, float      , name, ::call##f)       \
MINIFAD_FN_##ID(REAL, C99, double     , name, ::call   )       \
MINIFAD_FN_##ID(REAL, C99, long double, name, ::call##l)      //
#define MINIFAD_FN_R_LQM(ID,name,call)                         \
MINIFAD_FN_##ID(REAL, C99, __float128 , name, ::call##q)      //

#define MINIFAD_FN_C_nil(ID,name,call)                         //
#define MINIFAD_FN_C_STD(ID,name,call)                         \
MINIFAD_FN_##ID(CMPLX,CXX, float      , name, std::call)       \
MINIFAD_FN_##ID(CMPLX,CXX, double     , name, std::call)       \
MINIFAD_FN_##ID(CMPLX,CXX, long double, name, std::call)      //
#define MINIFAD_FN_C_TR1(ID,name,call)                         \
MINIFAD_FN_##ID(CMPLX,CXX, float      , name, std::tr1::call)  \
MINIFAD_FN_##ID(CMPLX,CXX, double     , name, std::tr1::call)  \
MINIFAD_FN_##ID(CMPLX,CXX, long double, name, std::tr1::call) //
#define MINIFAD_FN_C_C99(ID,name,call)                         \
MINIFAD_FN_##ID(CMPLX,C99, float      , name, ::c##call##f)    \
MINIFAD_FN_##ID(CMPLX,C99, double     , name, ::c##call   )    \
MINIFAD_FN_##ID(CMPLX,C99, long double, name, ::c##call##l)   //
#define MINIFAD_FN_C_LQM(ID,name,call)                         \
MINIFAD_FN_##ID(CMPLX,C99, __float128 , name, ::c##call##q)   //

#if !MINIFAD_HAVE_TR1_CMATH
#undef  MINIFAD_FN_R_TR1
#define MINIFAD_FN_R_TR1(ID,name,call) MINIFAD_FN_R_C99(ID,name,call)
#endif
#if !MINIFAD_HAVE_TR1_COMPLEX
#undef  MINIFAD_FN_C_TR1
#define MINIFAD_FN_C_TR1(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#endif
#if !MINIFAD_HAVE_COMPLEX_CXX
#undef  MINIFAD_FN_C_STD
#define MINIFAD_FN_C_STD(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#undef  MINIFAD_FN_C_TR1
#define MINIFAD_FN_C_TR1(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#endif
#if !MINIFAD_HAVE_COMPLEX_C99
#undef  MINIFAD_FN_C_C99
#define MINIFAD_FN_C_C99(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#undef  MINIFAD_FN_C_LQM
#define MINIFAD_FN_C_LQM(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#endif
#if !MINIFAD_HAVE_FLOAT128 || !MINIFAD_HAVE_LIBQUADMATH
#undef  MINIFAD_FN_R_LQM
#define MINIFAD_FN_R_LQM(ID,name,call) MINIFAD_FN_R_nil(ID,name,call)
#undef  MINIFAD_FN_C_LQM
#define MINIFAD_FN_C_LQM(ID,name,call) MINIFAD_FN_C_nil(ID,name,call)
#endif

#define MINIFAD_FN_R_CXX98(ID,name,call) MINIFAD_FN_R_STD(ID,name,call)
#define MINIFAD_FN_C_CXX98(ID,name,call) MINIFAD_FN_C_STD(ID,name,call)
#if __cplusplus >= 201103L
#define MINIFAD_FN_R_CXX11(ID,name,call) MINIFAD_FN_R_STD(ID,name,call)
#define MINIFAD_FN_C_CXX11(ID,name,call) MINIFAD_FN_C_STD(ID,name,call)
#else
#define MINIFAD_FN_R_CXX11(ID,name,call) MINIFAD_FN_R_TR1(ID,name,call)
#define MINIFAD_FN_C_CXX11(ID,name,call) MINIFAD_FN_C_TR1(ID,name,call)
#endif

#define MINIFAD_FN(ID,R_STD,R_LQM,C_STD,C_C99,C_LQM,name) \
MINIFAD_FN_R_##R_STD(ID,name,name)                        \
MINIFAD_FN_R_##R_LQM(ID,name,name)                        \
MINIFAD_FN_C_##C_STD(ID,name,name)                        \
MINIFAD_FN_C_##C_C99(ID,name,name)                        \
MINIFAD_FN_C_##C_LQM(ID,name,name)                       //

MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , sin   )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , cos   )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , tan   )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX11 , C99 , LQM , asin  )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX11 , C99 , LQM , acos  )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX11 , C99 , LQM , atan  )
MINIFAD_FN ( 2 , CXX98 , LQM ,  nil  , nil , nil , atan2 )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , sinh  )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , cosh  )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , tanh  )
MINIFAD_FN ( 1 , CXX11 , LQM , CXX11 , C99 , LQM , asinh )
MINIFAD_FN ( 1 , CXX11 , LQM , CXX11 , C99 , LQM , acosh )
MINIFAD_FN ( 1 , CXX11 , LQM , CXX11 , C99 , LQM , atanh )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , exp   )
MINIFAD_FN ( 1 , CXX11 , LQM ,  nil  , nil , nil , expm1 )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , log   )
MINIFAD_FN ( 1 , CXX11 , LQM ,  nil  , nil , nil , log1p )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , nil , LQM , log10 )
MINIFAD_FN ( 2 , CXX98 , LQM , CXX98 , C99 , LQM , pow   )
MINIFAD_FN ( 1 , CXX98 , LQM , CXX98 , C99 , LQM , sqrt  )
MINIFAD_FN ( 1 , CXX11 , LQM ,  nil  , nil , nil , cbrt  )
MINIFAD_FN ( 2 , CXX11 , LQM ,  nil  , nil , nil , hypot )
MINIFAD_FN ( 3 , CXX98 , nil , CXX98 , C99 , LQM , abs   )
MINIFAD_FN ( 3 , CXX98 , LQM ,  nil  , nil , nil , fabs  )

#if defined(__GLIBC__)
MINIFAD_FN_C_C99   ( 1 , log10 , log10 )
#endif
MINIFAD_FN_R_LQM   ( 3 , abs   , fabs  )
MINIFAD_FN_C_CXX98 ( 3 , fabs  , abs   )
MINIFAD_FN_C_C99   ( 3 , fabs  , abs   )
MINIFAD_FN_C_LQM   ( 3 , fabs  , abs   )

#undef MINIFAD_REAL_CXX
#undef MINIFAD_REAL_C99
#undef MINIFAD_CMPLX_CXX
#undef MINIFAD_CMPLX_C99
#undef MINIFAD_FN_1
#undef MINIFAD_FN_2
#undef MINIFAD_FN_3
#undef MINIFAD_FN_R_nil
#undef MINIFAD_FN_R_STD
#undef MINIFAD_FN_R_TR1
#undef MINIFAD_FN_R_C99
#undef MINIFAD_FN_R_LQM
#undef MINIFAD_FN_C_nil
#undef MINIFAD_FN_C_STD
#undef MINIFAD_FN_C_TR1
#undef MINIFAD_FN_C_C99
#undef MINIFAD_FN_C_LQM
#undef MINIFAD_FN_R_CXX98
#undef MINIFAD_FN_R_CXX11
#undef MINIFAD_FN_C_CXX98
#undef MINIFAD_FN_C_CXX11
#undef MINIFAD_FN

#if __cplusplus < 201103L && MINIFAD_HAVE_COMPLEX_CXX
template <typename T> inline std::complex<T>
asin(const std::complex<T>& z)
{ const std::complex<T> j(0,1);
  return -j*log(j*z+sqrt(1.0f-z*z)); }

template <typename T> inline std::complex<T>
acos(const std::complex<T>& z)
{ const std::complex<T> j(0,1);
  return j*log(z-j*sqrt(1.0f-z*z)); }

template <typename T> inline std::complex<T>
atan(const std::complex<T>& z)
{ const std::complex<T> j(0,1);
  return 0.5f*j*log((1.0f-j*z)/(1.0f+j*z)); }
#endif

#if __cplusplus < 201103L
template<typename Type> inline Type
asinh(const Type& z) { return log(z+sqrt(z*z+1.0f)); }

template<typename Type> inline Type
acosh(const Type& z) { return log(z+sqrt(z*z-1.0f)); }

template<typename Type> inline Type
atanh(const Type& z) { return 0.5f*log((1.0f+z)/(1.0f-z)); }
#endif

template<typename Type> inline Type
expm1(const Type& z) { return exp(z)-1.0f; }

template<typename Type> inline Type
log1p(const Type& z) { return log(1.0f+z); }

template<typename Type> inline Type
log10(const Type& z) { return log(z)/log(Type(10)); }

template<typename Type> inline Type
pow(const Type& x,int n) { return pow(x,Type(n)); }
#if MINIFAD_HAVE_COMPLEX_CXX
template <typename T> inline std::complex<T>
pow(const std::complex<T>& z,int n)
{ return std::complex<T>(std::pow(z,n)); }
#endif
inline float pow(float x,int n) { return float(std::pow(x,n)); }
inline double pow(double x,int n) { return std::pow(x,n); }
inline long double pow(long double x,int n) { return std::pow(x,n); }

template<typename Type> inline Type
cbrt(const Type& x) { return pow(x,1.0f/Type(3)); }
#if MINIFAD_HAVE_COMPLEX_CXX
template <typename T> inline std::complex<T>
cbrt(const std::complex<T>& z) { return std::pow(z,1.0f/T(3)); }
#endif

}} // namespace MiniFAD::math

// -----

namespace MiniFAD {

// Forward AD type
template <typename, int> class Fad;

// Expression
template <typename> class Expr;

// Constant
template <typename> class Const;

} // namespace MiniFAD

// -----

namespace MiniFAD {

template <typename T, int N=0>
class Fad {

public:
  typedef T value_type;

protected:
  value_type m_val;
  value_type m_grd[static_cast<unsigned>(N)];

  void cls()
  { for (int i=0; i<N; i++) m_grd[i] = 0.0f; }

public:

  Fad() : m_val(), m_grd() { }

  template <typename A>
  Fad(const A& a) : m_val(a), m_grd()  { }

  template <typename A>
  Fad(const A& a, int i) : m_val(a), m_grd()
  { m_grd[i] = 1.0f; }

  template <typename A>
  Fad(const A& a, int i, int) : m_val(a), m_grd()
  { m_grd[i] = 1.0f; }

  Fad(const Fad& x) : m_val(x.val())
  { for (int i=0; i<N; i++) m_grd[i] = x.grd(i); }

  template <typename U>
  Fad(const Fad<U,N>& x) : m_val(x.val())
  { for (int i=0; i<N; i++) m_grd[i] = x.grd(i); }

  template <typename E>
  Fad(const Expr<E>& x) : m_val(x.val())
  { for (int i=0; i<N; i++) m_grd[i] = x.grd(i); }

  template <typename A>
  value_type& def(const A& a, int i, int) { m_val = a; return def(i); }
  value_type& def(int i,int) { return def(i); }
  value_type& def(int i) { cls(); return m_grd[i] = 1.0f; }
  const value_type& val() const { return m_val; }
  const value_type& grd(int i) const { return m_grd[i]; }
  const value_type* grd() const { return &m_grd[0]; }
  int num() const { return N; }

  value_type& diff(int i) { return def(i); }
  value_type& diff(int i, int) { return def(i); }
  const value_type& d(int i) const { return grd(i); }
  const value_type* d() const { return grd(); }
  const value_type& dx(int i) const { return grd(i); }
  const value_type* dx() const { return grd(); }
  int size() const { return num();}

  template <typename A>
  Fad& operator=(const A& a) {
    cls();
    m_val = a;
    return *this;
  }
  template <typename A>
  Fad& operator+=(const A& a) {
    m_val += a;
    return *this;
  }
  template <typename A>
  Fad& operator-=(const A& a) {
    m_val -= a;
    return *this;
  }
  template <typename A>
  Fad& operator*=(const A& a) {
    for (int i=0; i<N; i++) m_grd[i] *= a;
    m_val *= a;
    return *this;
  }
  template <typename A>
  Fad& operator/=(const A& a) {
    for (int i=0; i<N; i++) m_grd[i] /= a;
    m_val /= a;
    return *this;
  }

  Fad& operator=(const Fad& x) {
    if (this == &x) return *this;
    for (int i=0; i<N; i++) m_grd[i] = x.grd(i);
    m_val = x.val();
    return *this;
  }
  template <typename U>
  Fad& operator=(const Fad<U,N>& x) {
    for (int i=0; i<N; i++) m_grd[i] = x.grd(i);
    m_val = x.val();
    return *this;
  }
  template <typename U>
  Fad& operator+=(const Fad<U,N>& x) {
    for (int i=0; i<N; i++) m_grd[i] += x.grd(i);
    m_val += x.val();
    return *this;
  }
  template <typename U>
  Fad& operator-=(const Fad<U,N>& x) {
    for (int i=0; i<N; i++) m_grd[i] -= x.grd(i);
    m_val -= x.val();
    return *this;
  }
  template <typename U>
  Fad& operator*=(const Fad<U,N>& x) {
    const typename Fad<U,N>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] * x_val + m_val * x.grd(i);
    m_val *= x_val;
    return *this;
  }
  template <typename U>
  Fad& operator/=(const Fad<U,N>& x) {
    const typename Fad<U,N>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] / x_val - m_val/(x_val*x_val) * x.grd(i);
    m_val /= x_val;
    return *this;
  }

  template <typename E>
  Fad& operator=(const Expr<E>& x) {
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] = x.grd(i);
    m_val = x_val;
    return *this;
  }
  template <typename E>
  Fad& operator+=(const Expr<E>& x) {
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] += x.grd(i);
    m_val += x_val;
    return *this;
  }
  template <typename E>
  Fad& operator-=(const Expr<E>& x) {
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] -= x.grd(i);
    m_val -= x_val;
    return *this;
  }
  template <typename E>
  Fad& operator*=(const Expr<E>& x) {
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] * x_val + m_val * x.grd(i);
    m_val *= x_val;
    return *this;
  }
  template <typename E>
  Fad& operator/=(const Expr<E>& x) {
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] / x_val - m_val/(x_val*x_val) * x.grd(i);
    m_val /= x_val;
    return *this;
  }

};

template <typename E>
class Expr {
public:
  typedef typename E::value_type value_type;
protected:
  const E m_expr;
public:
  explicit Expr(const E& arg) : m_expr(arg) { }
  value_type val() const { return m_expr.val(); }
  value_type grd(int i) const { return m_expr.grd(i); }
  int num() const { return m_expr.num(); }
};

#define MINIFAD_DEF_BOOL1_OP(OP)                      \
                                                      \
template <typename E> inline bool                     \
operator OP (const Expr<E>& arg)                      \
{ return OP arg.val(); }                              \
                                                      \
template <typename T,int N> inline bool               \
operator OP (const Fad<T,N>& arg)                     \
{ return OP arg.val(); }                             //

#define MINIFAD_DEF_BOOL2_OP(OP)                      \
                                                      \
template <typename E> inline bool                     \
operator OP (const Expr<E>& x, const Expr<E>& y)      \
{ return x.val() OP y.val(); }                        \
                                                      \
template <typename E,typename T,int N> inline bool    \
operator OP (const Expr<E>& x, const Fad<T,N>& y)     \
{ return x.val() OP y.val(); }                        \
                                                      \
template <typename E,typename T,int N> inline bool    \
operator OP (const Fad<T,N>& x, const Expr<E>& y)     \
{ return x.val() OP y.val(); }                        \
                                                      \
template <typename E,typename A> inline bool          \
operator OP (const Expr<E>& x, const A& y)            \
{ return x.val() OP y; }                              \
                                                      \
template <typename E,typename A> inline bool          \
operator OP (const A& x, const Expr<E>& y)            \
{ return x OP y.val(); }                              \
                                                      \
template <typename T,int N,typename U> inline bool    \
operator OP (const Fad<T,N>& x, const Fad<U,N>& y)    \
{ return x.val() OP y.val(); }                        \
                                                      \
template <typename T,int N,typename A> inline bool    \
operator OP (const Fad<T,N>& x, const A& y)           \
{ return x.val() OP y; }                              \
                                                      \
template <typename T,int N,typename A> inline bool    \
operator OP (const A& x, const Fad<T,N>& y)           \
{ return x OP y.val(); }                             //

// Logical operators
MINIFAD_DEF_BOOL1_OP(! )
MINIFAD_DEF_BOOL2_OP(&&)
MINIFAD_DEF_BOOL2_OP(||)
// Comparison/relational operators
MINIFAD_DEF_BOOL2_OP(==)
MINIFAD_DEF_BOOL2_OP(!=)
MINIFAD_DEF_BOOL2_OP(> )
MINIFAD_DEF_BOOL2_OP(< )
MINIFAD_DEF_BOOL2_OP(>=)
MINIFAD_DEF_BOOL2_OP(<=)

#undef MINIFAD_DEF_BOOL1_OP
#undef MINIFAD_DEF_BOOL2_OP

} // namespace MiniFAD

/* --- */

#define MINIFAD_DEF_UNARY_OP(OP,NAME)                 \
                                                      \
namespace MiniFAD { namespace op {                    \
template <typename E>                                 \
class NAME {                                          \
public:                                               \
  typedef typename E::value_type value_type;          \
protected:                                            \
  NAME() { }                                          \
  const E& m_arg;                                     \
public:                                               \
  explicit NAME(const E& arg)                         \
    : m_arg(arg) { }                                  \
  value_type val() const                              \
  { return OP m_arg.val(); }                          \
  value_type grd(int i) const                         \
  { return OP m_arg.grd(i); }                         \
  int num() const                                     \
  { return m_arg.num(); }                             \
};                                                    \
}}                                                    \
                                                      \
namespace MiniFAD {                                   \
                                                      \
template <typename E> inline                          \
Expr< op::NAME< Expr<E> > >                           \
operator OP (const Expr<E>& arg) {                    \
  typedef op::NAME< Expr<E> > expr_t;                 \
  return Expr<expr_t>(expr_t(arg));                   \
}                                                     \
                                                      \
template <typename T,int N> inline                    \
Expr< op::NAME< Fad<T,N> > >                          \
operator OP (const Fad<T,N>& arg) {                   \
  typedef op::NAME< Fad<T,N> > expr_t;                \
  return Expr<expr_t>(expr_t(arg));                   \
}                                                     \
                                                      \
}                                                    //

// Unary operators
MINIFAD_DEF_UNARY_OP(+, plus)
MINIFAD_DEF_UNARY_OP(-, minus)

#define MINIFAD_DEF_UNARY_FN(NAME,               \
                             VALUE,              \
                             PARTIAL)            \
                                                 \
namespace MiniFAD { namespace op {               \
template <typename E>                            \
class NAME {                                     \
public:                                          \
  typedef typename E::value_type value_type;     \
protected:                                       \
  const E& m_arg;                                \
  mutable value_type partial;                    \
public:                                          \
  explicit NAME(const E& arg)                    \
    : m_arg(arg) { }                             \
  value_type val() const {                       \
    const value_type u = m_arg.val();            \
    const value_type NAME##_u = (VALUE);         \
    partial = (PARTIAL);                         \
    return NAME##_u;                             \
  }                                              \
  value_type grd(int i) const                    \
  { return m_arg.grd(i) * (partial); }           \
  int num() const                                \
  { return m_arg.num(); }                        \
};                                               \
}}                                               \
                                                 \
namespace MiniFAD {                              \
                                                 \
template <typename E> inline                     \
Expr< op::NAME< Expr<E> > >                      \
NAME (const Expr<E>& arg) {                      \
  typedef op::NAME< Expr<E> > expr_t;            \
  return Expr<expr_t>(expr_t(arg));              \
}                                                \
                                                 \
template <typename T,int N> inline               \
Expr< op::NAME< Fad<T,N> > >                     \
NAME (const Fad<T,N>& arg) {                     \
  typedef op::NAME< Fad<T,N> > expr_t;           \
  return Expr<expr_t>(expr_t(arg));              \
}                                                \
                                                 \
}                                               //

// Trigonometric functions
MINIFAD_DEF_UNARY_FN(sin
                     , math::sin(u)
                     , math::cos(u)
                     )
MINIFAD_DEF_UNARY_FN(cos
                     ,  math::cos(u)
                     , -math::sin(u)
                     )
MINIFAD_DEF_UNARY_FN(tan
                     , math::tan(u)
                     , 1.0f+tan_u*tan_u
                     )
MINIFAD_DEF_UNARY_FN(asin
                     , math::asin(u)
                     , 1.0f/math::sqrt(1.0f-u*u)
                     )
MINIFAD_DEF_UNARY_FN(acos
                     , math::acos(u)
                     , -1.0f/math::sqrt(1.0f-u*u)
                     )
MINIFAD_DEF_UNARY_FN(atan
                     , math::atan(u)
                     , 1.0f/(1.0f+u*u)
                     )

// Hyperbolic functions
MINIFAD_DEF_UNARY_FN(sinh
                     , math::sinh(u)
                     , math::cosh(u)
                     )
MINIFAD_DEF_UNARY_FN(cosh
                     , math::cosh(u)
                     , math::sinh(u)
                     )
MINIFAD_DEF_UNARY_FN(tanh
                     , math::tanh(u)
                     , 1.0f-tanh_u*tanh_u
                     )
MINIFAD_DEF_UNARY_FN(asinh
                     , math::asinh(u)
                     , 1.0f/math::sqrt(1.0f+u*u)
                     )
MINIFAD_DEF_UNARY_FN(acosh
                     , math::acosh(u)
                     , 1.0f/math::sqrt(u*u-1.0f)
                     )
MINIFAD_DEF_UNARY_FN(atanh
                     , math::atanh(u)
                     , 1.0f/(1.0f-u*u)
                     )

// Exponential and logarithmic functions
MINIFAD_DEF_UNARY_FN(exp
                     , math::exp(u)
                     , exp_u
                     )
MINIFAD_DEF_UNARY_FN(expm1
                     , math::expm1(u)
                     , expm1_u+1.0f
                     )
MINIFAD_DEF_UNARY_FN(log
                     , math::log(u)
                     , 1.0f/u
                     )
MINIFAD_DEF_UNARY_FN(log1p
                     , math::log1p(u)
                     , 1.0f/(1.0f+u)
                     )
MINIFAD_DEF_UNARY_FN(log10
                     , math::log10(u)
                     , 1.0f/(math::log(value_type(10))*u)
                     )

// Power functions
MINIFAD_DEF_UNARY_FN(sqrt
                     , math::sqrt(u)
                     , 0.5f/sqrt_u
                     )
MINIFAD_DEF_UNARY_FN(cbrt
                     , math::cbrt(u)
                     , 1.0f/(3.0f*cbrt_u*cbrt_u)
                     )

// Other functions
MINIFAD_DEF_UNARY_FN(abs
                     , math::abs(u)
                     , /*std::signbit*/(u >= 0) ? 1 : -1
                     )
MINIFAD_DEF_UNARY_FN(fabs
                     , math::fabs(u)
                     , /*std::signbit*/(u >= 0) ? 1 : -1
                     )

#define MINIFAD_DEF_BINARY_OP(OP,NAME,                \
                              VALUE,                  \
                              PARTIAL_U,              \
                              PARTIAL_V)              \
                                                      \
namespace MiniFAD { namespace op {                    \
template < typename L, typename R >                   \
class NAME {                                          \
  typedef typename L::value_type T;                   \
  typedef typename R::value_type U;                   \
public:                                               \
  typedef typename promote<T,U>::type value_type;     \
protected:                                            \
  const L& m_lhs;                                     \
  const R& m_rhs;                                     \
  mutable value_type partial_u;                       \
  mutable value_type partial_v;                       \
public:                                               \
  NAME(const L& lhs, const R& rhs)                    \
    : m_lhs(lhs), m_rhs(rhs) { }                      \
  value_type val() const {                            \
    const value_type u = m_lhs.val();                 \
    const value_type v = m_rhs.val();                 \
    const value_type value = (VALUE);                 \
    partial_u = (PARTIAL_U);                          \
    partial_v = (PARTIAL_V);                          \
    return (value);                                   \
  }                                                   \
  value_type grd(int i) const                         \
  { return m_lhs.grd(i) * (partial_u) +               \
      /**/ m_rhs.grd(i) * (partial_v); }              \
  int num() const                                     \
  { int n_lhs = m_lhs.num(), n_rhs = m_rhs.num();     \
    return (n_lhs < n_rhs) ? n_rhs : n_lhs; }         \
};                                                    \
}}                                                    \
                                                      \
namespace MiniFAD { namespace op {                    \
template < typename E, typename A >                   \
class NAME< E, Const<A> > {                           \
  typedef typename E::value_type T;                   \
  typedef A U;                                        \
public:                                               \
  typedef typename promote<T,U>::type value_type;     \
protected:                                            \
  const E& m_lhs;                                     \
  const value_type m_rhs;                             \
  mutable value_type partial_u;                       \
public:                                               \
  NAME(const E& lhs, const A& rhs)                    \
    : m_lhs(lhs), m_rhs(rhs) { }                      \
  value_type val() const {                            \
    const value_type  u = m_lhs.val();                \
    const value_type& v = m_rhs;                      \
    const value_type  value = (VALUE);                \
    partial_u = (PARTIAL_U);                          \
    return (value);                                   \
  }                                                   \
  value_type grd(int i) const                         \
  { return m_lhs.grd(i) * (partial_u); }              \
  int num() const                                     \
  { return m_lhs.num(); }                             \
};                                                    \
}}                                                    \
                                                      \
namespace MiniFAD { namespace op {                    \
template < typename A, typename E >                   \
class NAME< Const<A>, E > {                           \
  typedef typename E::value_type T;                   \
  typedef A U;                                        \
public:                                               \
  typedef typename promote<T,U>::type value_type;     \
protected:                                            \
  const value_type m_lhs;                             \
  const E& m_rhs;                                     \
  mutable value_type partial_v;                       \
public:                                               \
  NAME(const A& lhs, const E& rhs)                    \
    : m_lhs(lhs), m_rhs(rhs) { }                      \
  value_type val() const {                            \
    const value_type& u = m_lhs;                      \
    const value_type  v = m_rhs.val();                \
    const value_type  value = (VALUE);                \
    partial_v = (PARTIAL_V);                          \
    return (value);                                   \
  }                                                   \
  value_type grd(int i) const                         \
  { return m_rhs.grd(i) * (partial_v); }              \
  int num() const                                     \
  { return m_rhs.num(); }                             \
};                                                    \
}}                                                    \
                                                      \
namespace MiniFAD {                                   \
                                                      \
template <typename L,typename R> inline               \
Expr< op::NAME< Expr<L>, Expr<R> > >                  \
OP (const Expr<L>& lhs, const Expr<R>& rhs) {         \
  typedef op::NAME< Expr<L>, Expr<R> > expr_t;        \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename E,typename T,int N> inline         \
Expr < op::NAME< Expr<E>, Fad<T,N> > >                \
OP (const Expr<E>& lhs, const Fad<T,N>& rhs) {        \
  typedef op::NAME< Expr<E>, Fad<T,N> > expr_t;       \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename E,typename T,int N> inline         \
Expr < op::NAME< Fad<T,N>, Expr<E> > >                \
OP (const Fad<T,N>& lhs, const Expr<E>& rhs) {        \
  typedef op::NAME< Fad<T,N>, Expr<E> > expr_t;       \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename E,typename A> inline               \
Expr< op::NAME< Expr<E>, Const<A> > >                 \
OP (const Expr<E>& lhs, const A& rhs) {               \
  typedef op::NAME< Expr<E>, Const<A> > expr_t;       \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename E,typename A> inline               \
Expr< op::NAME< Const<A>, Expr<E> > >                 \
OP (const A& lhs, const Expr<E>& rhs) {               \
  typedef op::NAME< Const<A>, Expr<E> > expr_t;       \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename T,int N,typename U> inline         \
Expr< op::NAME< Fad<T,N>, Fad<U,N> > >                \
OP (const Fad<T,N>& lhs,const Fad<U,N>& rhs) {        \
  typedef op::NAME< Fad<T,N>, Fad<U,N> > expr_t;      \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename T,int N,typename A> inline         \
Expr < op::NAME< Fad<T,N>, Const<A> > >               \
OP (const Fad<T,N>& lhs, const A& rhs) {              \
  typedef op::NAME< Fad<T,N>, Const<A> > expr_t;      \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
template <typename T,int N,typename A> inline         \
Expr< op::NAME< Const<A>, Fad<T,N> > >                \
OP (const A& lhs, const Fad<T,N>& rhs) {              \
  typedef op::NAME< Const<A>, Fad<T,N> > expr_t;      \
  return Expr<expr_t>(expr_t(lhs,rhs));               \
}                                                     \
                                                      \
}                                                    //
#define MINIFAD_DEF_BINARY_FN(NAME,                   \
                              VALUE,                  \
                              PARTIAL_U,              \
                              PARTIAL_V)              \
        MINIFAD_DEF_BINARY_OP(NAME,NAME,              \
                              VALUE,                  \
                              PARTIAL_U,              \
                              PARTIAL_V)             //

// Binary operators
MINIFAD_DEF_BINARY_OP(operator+, add
                      , /*value:    */ u + v
                      , /*partial_u:*/ 1.0f
                      , /*partial_v:*/ 1.0f
                      )
MINIFAD_DEF_BINARY_OP(operator-, sub
                      , /*value:    */ u - v
                      , /*partial_u:*/ +1.0f
                      , /*partial_v:*/ -1.0f
                      )
MINIFAD_DEF_BINARY_OP(operator*, mul
                      , /*value:    */ u * v
                      , /*partial_u:*/ v
                      , /*partial_v:*/ u
                      )
MINIFAD_DEF_BINARY_OP(operator/, div
                      , /*value:    */ u / v
                      , /*partial_u:*/ 1.0f / v
                      , /*partial_v:*/ - u / (v * v)
                      )

// Trigonometric functions
MINIFAD_DEF_BINARY_FN(atan2
                      , /*value:    */ math::atan2(u,v)
                      , /*partial_u:*/  v / (u*u + v*v)
                      , /*partial_v:*/ -u / (u*u + v*v)
                      )

// Power functions
MINIFAD_DEF_BINARY_FN(pow
                      , /*value:    */ math::pow(u,v)
                      , /*partial_u:*/ value * v / u
                      , /*partial_v:*/ value * math::log(u)
                      )
MINIFAD_DEF_BINARY_FN(hypot
                      , /*value:    */ math::hypot(u,v)
                      , /*partial_u:*/ u / value
                      , /*partial_v:*/ v / value
                      )

namespace MiniFAD { namespace op {
template <typename E>
class pow< E, Const<int> > {
public:
  typedef typename E::value_type value_type;
protected:
  pow() { }
  const E&  m_lhs;
  const int m_rhs;
  mutable value_type partial;
public:
  pow(const E& arg, const int& n)
    : m_lhs(arg), m_rhs(n) { }
  const value_type val() const {
    const int n = m_rhs;
    const value_type u = m_lhs.val();
    const value_type ipow_u = math::pow(u,n);
    partial = n ? (value_type(n)*math::pow(u,n-1)) : 0.0f;
    return ipow_u;
  }
  const value_type grd(int i) const
  { return m_lhs.grd(i) * partial; }
  int num() const
  { return m_lhs.num(); }
};
}} // namespace MiniFAD::op

// -----

#include <cstdlib>

namespace MiniFAD {

template <typename T>
class Fad<T, 0> {

public:
  typedef T value_type;

protected:
  value_type  m_val;
  value_type* m_grd;
  int         m_num;

  template <typename U>
  static void cpy(T* p, const U* q, int s, int n)
  { for (int i=s; i<n; i++) p[i] = q[i]; }
  static void cls(T* p, int s, int n)
  { for (int i=s; i<n; i++) p[i] = 0.0f; }

  static T* mem(int n)
  { return n ? static_cast<T*>(std::malloc(size_t(n)*sizeof(T))) : NULL; }
  static void del(T* p)
  { std::free(p); }

  void resize(int n, bool init) {
    if (n == m_num) return;
    if (!m_num) {
      T* const q = mem(n);
      if (init) { cls(q, 0, n); }
      m_grd = q; m_num = n;
    } else if (!n) {
      del(m_grd); m_grd = NULL; m_num = 0;
    } else if (n > m_num ) {
      T* const q = mem(n);
      if (init) { cpy(q, m_grd, 0, m_num); cls(q, m_num, n); }
      del(m_grd); m_grd = q; m_num = n;
    } else /* (n < m_num ) */ {
      T* const q = mem(n);
      if (init) { cpy(q, m_grd, 0, n); }
      del(m_grd); m_grd = q; m_num = n;
    }
  }

  void cls() { cls(m_grd, 0, m_num); }

  void setnum(int n)
  { resize(n, 0); }

  template <typename U, int M>
  int setnum(const Fad<U,M>& x)
  { resize(x.num(), 0); return m_num; }

  template <typename E>
  int setnum(const Expr<E>& x)
  { resize(x.num(), 1); return m_num; }

public:

  ~Fad() { del(m_grd); }

  Fad() : m_val(), m_grd(), m_num() { }

  template <typename A>
  Fad(const A& a) : m_val(a), m_grd(), m_num()  { }

  template <typename A>
  Fad(const A& a, int i) : m_val(a), m_grd(mem(i+1)), m_num(i+1)
  { cls(); m_grd[i] = 1.0f; }

  template <typename A>
  Fad(const A& a, int i, int n) : m_val(a), m_grd(mem(n)), m_num(n)
  { cls(); m_grd[i] = 1.0f; }

  Fad(const Fad& x) : m_val(x.val()), m_grd(), m_num(x.num())
  { cpy(m_grd = mem(m_num), x.grd(), 0, m_num); }

  #if __cplusplus >= 201103L
  Fad(Fad&& x) : m_val(x.m_val), m_grd(x.m_grd), m_num(x.m_num)
  { x.m_val = 0.0f; x.m_grd = NULL; x.m_num = 0; }
  #endif

  template <typename U, int M>
  Fad(const Fad<U,M>& x) : m_val(x.val()), m_grd(), m_num(x.num())
  { cpy(m_grd = mem(m_num), x.grd(), 0, m_num); }

  template <typename E>
  Fad(const Expr<E>& x) : m_val(x.val()), m_grd(), m_num(x.num())
  { const int N = m_num; m_grd = mem(N);
    for (int i=0; i<N; i++) m_grd[i] = x.grd(i); }

  template <typename A>
  value_type& def(const A& a, int i, int n) { m_val = a; return def(i, n); }
  value_type& def(int i, int n) { setnum(n); cls(); return m_grd[i] = 1.0f; }
  value_type& def(int i) { return def(i, (i<m_num) ? m_num : (i+1)); }
  const value_type& val() const { return m_val; }
  const value_type  grd(int i) const { return (i<m_num) ? m_grd[i] : 0.0f; }
  const value_type* grd() const { return &m_grd[0]; }
  int num() const { return m_num; }

  value_type& diff(int i) { return def(i); }
  value_type& diff(int i, int n) { return def(i, n); }
  const value_type  d(int i) const { return grd(i); }
  const value_type* d() const { return grd(); }
  const value_type  dx(int i) const { return grd(i); }
  const value_type* dx() const { return grd(); }
  int size() const { return num(); }

  template <typename A>
  Fad& operator=(const A& a) {
    cls();
    m_val = a;
    return *this;
  }
  template <typename A>
  Fad& operator+=(const A& a) {
    m_val += a;
    return *this;
  }
  template <typename A>
  Fad& operator-=(const A& a) {
    m_val -= a;
    return *this;
  }
  template <typename A>
  Fad& operator*=(const A& a) {
    const int N = num();
    for (int i=0; i<N; i++) m_grd[i] *= a;
    m_val *= a;
    return *this;
  }
  template <typename A>
  Fad& operator/=(const A& a) {
    const int N = num();
    for (int i=0; i<N; i++) m_grd[i] /= a;
    m_val /= a;
    return *this;
  }

  #if __cplusplus >= 201103L
  Fad& operator=(Fad&& x) {
    del(m_grd);
    m_val = x.m_val; x.m_val = 0.0f;
    m_grd = x.m_grd; x.m_grd = 0;
    m_num = x.m_num; x.m_num = 0;
    return *this;
  }
  #endif
  Fad& operator=(const Fad& x) {
    if (this == &x) return *this;
    const int N = setnum(x);
    cpy(m_grd, x.grd(), 0, N);
    m_val = x.val();
    return *this;
  }
  template <typename U, int M>
  Fad& operator=(const Fad<U,M>& x) {
    const int N = setnum(x);
    cpy(m_grd, x.grd(), 0, N);
    m_val = x.val();
    return *this;
  }
  template <typename U, int M>
  Fad& operator+=(const Fad<U,M>& x) {
    const int N = setnum(x);
    const typename Fad<U,M>::value_type  x_val = x.val();
    const typename Fad<U,M>::value_type* x_grd = x.grd();
    for (int i=0; i<N; i++) m_grd[i] += x_grd[i];
    m_val += x_val;
    return *this;
  }
  template <typename U, int M>
  Fad& operator-=(const Fad<U,M>& x) {
    const int N = setnum(x);
    const typename Fad<U,M>::value_type  x_val = x.val();
    const typename Fad<U,M>::value_type* x_grd = x.grd();
    for (int i=0; i<N; i++) m_grd[i] -= x_grd[i];
    m_val -= x_val;
    return *this;
  }
  template <typename U, int M>
  Fad& operator*=(const Fad<U,M>& x) {
    const int N = setnum(x);
    const typename Fad<U,M>::value_type  x_val = x.val();
    const typename Fad<U,M>::value_type* x_grd = x.grd();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] * x_val + m_val * x_grd[i];
    m_val *= x_val;
    return *this;
  }
  template <typename U, int M>
  Fad& operator/=(const Fad<U,M>& x) {
    const int N = setnum(x);
    const typename Fad<U,M>::value_type  x_val = x.val();
    const typename Fad<U,M>::value_type* x_grd = x.grd();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] / x_val - m_val/(x_val*x_val) * x_grd[i];
    m_val /= x_val;
    return *this;
  }

  template <typename E>
  Fad& operator=(const Expr<E>& x) {
    const int N = setnum(x);
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] = x.grd(i);
    m_val = x_val;
    return *this;
  }
  template <typename E>
  Fad& operator+=(const Expr<E>& x) {
    const int N = setnum(x);
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] += x.grd(i);
    m_val += x_val;
    return *this;
  }
  template <typename E>
  Fad& operator-=(const Expr<E>& x) {
    const int N = setnum(x);
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++) m_grd[i] -= x.grd(i);
    m_val -= x_val;
    return *this;
  }
  template <typename E>
  Fad& operator*=(const Expr<E>& x) {
    const int N = setnum(x);
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] * x_val + m_val * x.grd(i);
    m_val *= x_val;
    return *this;
  }
  template <typename E>
  Fad& operator/=(const Expr<E>& x) {
    const int N = setnum(x);
    const typename Expr<E>::value_type x_val = x.val();
    for (int i=0; i<N; i++)
      m_grd[i] = m_grd[i] / x_val - m_val/(x_val*x_val) * x.grd(i);
    m_val /= x_val;
    return *this;
  }

};

} //namespace MiniFAD

#endif // MINIFAD_HPP
