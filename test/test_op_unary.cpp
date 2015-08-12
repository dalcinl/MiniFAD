#include "minifadtest.hpp"

template <typename ad_type> static
void test_unary_op() {
  ad_type x(1,1),f;
  f = +x;      assertclose(f.val(),+1); assertclose(f.grd(0),0); assertclose(f.grd(1),+1);
  f = -x;      assertclose(f.val(),-1); assertclose(f.grd(0),0); assertclose(f.grd(1),-1);
  f = +(x+x);  assertclose(f.val(),+2); assertclose(f.grd(0),0); assertclose(f.grd(1),+2);
  f = -(x+x);  assertclose(f.val(),-2); assertclose(f.grd(0),0); assertclose(f.grd(1),-2);
}

template <typename value_type> static
void test_common() {
  typedef MiniFAD::Fad<value_type,0> ad_type_0;
  typedef MiniFAD::Fad<value_type,2> ad_type_2;
  typedef MiniFAD::Fad<value_type,3> ad_type_3;
  test_unary_op<ad_type_0>();
  test_unary_op<ad_type_2>();
  test_unary_op<ad_type_3>();
}

template <typename float_type> static
void test_real() {
  test_common<float_type>();
}

template <typename float_type> static
void test_cxx_complex() {
#if MINIFAD_HAVE_COMPLEX_CXX
  typedef std::complex<float_type> complex_type;
  test_common<complex_type>();
#endif
}

template <typename float_type> static
void test_c99_complex() {
#if MINIFAD_HAVE_COMPLEX_C99
  typedef typename c99complex<float_type>::type complex_type;
  test_common<complex_type>();
#endif
}

template <typename float_type> static
void test_complex() {
  test_cxx_complex<float_type>();
  test_c99_complex<float_type>();
}
#if MINIFAD_HAVE_FLOAT128
template <>
void test_complex<__float128>()
{test_c99_complex<__float128>();}
#endif

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
  #if MINIFAD_HAVE_FLOAT128
  test< __float128  >();
  #endif
  return 0;
}
