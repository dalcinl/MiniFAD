#define PRAGMA_CLANG(x)
#define PRAGMA_GCC(x)

#if defined(__INTEL_COMPILER)
#elif defined(__clang__)
#undef  PRAGMA_CLANG
#define PRAGMA_CLANG(x) _Pragma(#x)
#elif defined(__GNUC__)
#undef  PRAGMA_GCC
#define PRAGMA_GCC(x) _Pragma(#x)
#endif

#include <cstdio>
#include <cassert>

#ifdef DISABLE_WARNING_CONVERSION
PRAGMA_CLANG( clang diagnostic ignored "-Wconversion"       )
#if defined(__GNUC__) && __GNUC__ > 5
PRAGMA_GCC  ( GCC   diagnostic ignored "-Wfloat-conversion" )
#else
PRAGMA_GCC  ( GCC   diagnostic ignored "-Wconversion" )
#endif
#endif

#if defined(__INTEL_COMPILER)
#pragma warning(disable:2012)
#pragma warning(disable:2014)
#pragma warning(disable:2015)
#pragma warning(disable:2017)
#pragma warning(disable:2022)
#pragma warning(disable:2259)
#endif

#include <minifad.hpp>

#if MINIFAD_HAVE_COMPLEX_C99
template <typename T> struct c99complex {};
template <> struct c99complex<float> { typedef float _Complex type; };
template <> struct c99complex<double> { typedef double _Complex type; };
template <> struct c99complex<long double> { typedef long double _Complex type; };
#if MINIFAD_HAVE_FLOAT128
template <> struct c99complex<__float128> { typedef __float128 _Complex type; };
#endif
#endif

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

template <typename T> inline
long double Abs(const T& x) {
  typedef long double result_t;
  return result_t(MiniFAD::math::abs(x));
}
#if MINIFAD_HAVE_FLOAT128
template<> inline
long double Abs(const __float128& x) {
  __float128 abs_x = (x >= 0) ? x : -x;
  typedef long double result_t;
  return result_t(abs_x);
}
#if MINIFAD_HAVE_COMPLEX_C99
template<> inline
long double Abs(const __float128 _Complex& z) {
  __float128 r = __real__ z;
  __float128 c = __imag__ z;
  typedef long double result_t;
  return hypotl(result_t(r),result_t(c));
}
#endif
#endif

template <typename T, typename U> inline
bool close(const T& x, const U& y){
  typedef typename MiniFAD::promote<T,U>::type type;
  const type dxy = type(x) - type(y);
  const long double tol = 1e-6l;
  return Abs(dxy) < tol;
}

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#pragma GCC diagnostic pop
#endif

#define assertclose(x,y) assert(close(x,y))
