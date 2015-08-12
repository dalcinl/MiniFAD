#if defined(__GNUC__) && (__GNUC__*10+__GNUC_MINOR__) > 46
#include <quadmath.h>
#endif
#include "minifadtest.hpp"

template <typename ad_type> static
void test_unary_fn() {
  double log_10 = MiniFAD::math::log(10.0);
  ad_type f,x;
  x =  0; x.def(0); f = sin(x);   assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = cos(x);   assertclose(f.val(),1); assertclose(f.grd(0),0);
  x =  0; x.def(0); f = tan(x);   assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = asin(x);  assertclose(f.val(),0);
  x =  1; x.def(0); f = acos(x);  assertclose(f.val(),0);
  x =  0; x.def(0); f = atan(x);  assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = sinh(x);  assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = cosh(x);  assertclose(f.val(),1); assertclose(f.grd(0),0);
  x =  0; x.def(0); f = tanh(x);  assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = asinh(x); assertclose(f.val(),0);
  x =  1; x.def(0); f = acosh(x); assertclose(f.val(),0);
  x =  0; x.def(0); f = atanh(x); assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = exp(x);   assertclose(f.val(),1); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = expm1(x); assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  1; x.def(0); f = log(x);   assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  0; x.def(0); f = log1p(x); assertclose(f.val(),0); assertclose(f.grd(0),1);
  x =  1; x.def(0); f = log10(x); assertclose(f.val(),0); assertclose(f.grd(0),1.0f/log_10);
  x = 10; x.def(0); f = log10(x); assertclose(f.val(),1); assertclose(f.grd(0),1.0f/log_10/10);
  x =  4; x.def(0); f = sqrt(x);  assertclose(f.val(),2); assertclose(f.grd(0),1.0f/4.0f);
  x =  8; x.def(0); f = cbrt(x);  assertclose(f.val(),2); assertclose(f.grd(0),1.0f/12.0f);
}

template <typename ad_type> static
void test_unary_fn_abs() {
  ad_type f,x;
  x = +2; x.def(0); f = abs(x);  assertclose(f.val(),2); assertclose(f.grd(0),+1);
  x = -3; x.def(0); f = abs(x);  assertclose(f.val(),3); assertclose(f.grd(0),-1);
  x = +3; x.def(0); f = fabs(x); assertclose(f.val(),3); assertclose(f.grd(0),+1);
  x = -2; x.def(0); f = fabs(x); assertclose(f.val(),2); assertclose(f.grd(0),-1);
}

template <typename value_type> static
void test_common() {
  typedef MiniFAD::Fad<value_type,0> ad_type_0;
  typedef MiniFAD::Fad<value_type,2> ad_type_2;
  typedef MiniFAD::Fad<value_type,3> ad_type_3;
  test_unary_fn<ad_type_0>();
  test_unary_fn<ad_type_2>();
  test_unary_fn<ad_type_3>();
}

template <typename float_type> static
void test_real() {
  test_common<float_type>();
  typedef MiniFAD::Fad<float_type,0> ad_type_0;
  typedef MiniFAD::Fad<float_type,1> ad_type_1;
  typedef MiniFAD::Fad<float_type,2> ad_type_2;
  test_unary_fn_abs<ad_type_0>();
  test_unary_fn_abs<ad_type_1>();
  test_unary_fn_abs<ad_type_2>();
}

template <typename float_type> static
void test_complex_cxx() {
#if MINIFAD_HAVE_COMPLEX_CXX
  typedef std::complex<float_type> complex_type;
  test_common<complex_type>();
#endif
}
#if MINIFAD_HAVE_FLOAT128
template <> inline
void test_complex_cxx<__float128>() {}
#endif

template <typename float_type> static
void test_complex_c99() {
#if MINIFAD_HAVE_COMPLEX_C99
  typedef typename c99complex<float_type>::type complex_type;
  test_common<complex_type>();
#endif
}

template <typename float_type> static
void test_complex() {
  test_complex_cxx<float_type>();
  test_complex_c99<float_type>();
}

template <typename float_type> static
void test() {
  test_real<float_type>();
  test_complex<float_type>();
}

int main(int,char *[])
{
  test< float       >();
  test< double      >();
  test< long double >();
  #if MINIFAD_HAVE_FLOAT128 && MINIFAD_HAVE_LIBQUADMATH
  test< __float128 >();
  #endif
  return 0;
}
