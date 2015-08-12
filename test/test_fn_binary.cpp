#if defined(__GNUC__) && (__GNUC__*10+__GNUC_MINOR__) > 46
#include <quadmath.h>
#endif
#define  DISABLE_WARNING_CONVERSION 1
#include "minifadtest.hpp"

template <typename ad_type> static
void test_binary_fn_atan2() {
  ad_type f,x,y;
  y = 0; y.def(0);
  x = 1; x.def(1);
  f = atan2(y,x); assertclose(f.val(),0); assertclose(f.grd(0),1); assertclose(f.grd(1),0);
}

template <typename ad_type> static
void test_binary_fn_pow() {
  using namespace MiniFAD;
  ad_type f,x(4,0),y(2,1);
  typedef typename ad_type::value_type value_type;
  const value_type a=3, c4=4, c9=9, c16=16, log_a=math::log(a), log_4=math::log(c4);
  f = pow(x,y); assertclose(f.val(),16); assertclose(f.grd(0), 8); assertclose(f.grd(1),c16*log_4);
  f = pow(x,a); assertclose(f.val(),64); assertclose(f.grd(0),48); assertclose(f.grd(1),        0);
  f = pow(y,a); assertclose(f.val(), 8); assertclose(f.grd(0), 0); assertclose(f.grd(1),       12);
  f = pow(a,y); assertclose(f.val(), 9); assertclose(f.grd(0), 0); assertclose(f.grd(1), c9*log_a);
  const float b=3;
  f = pow(x,b); assertclose(f.val(),64); assertclose(f.grd(0),48); assertclose(f.grd(1),        0);
  f = pow(y,b); assertclose(f.val(), 8); assertclose(f.grd(0), 0); assertclose(f.grd(1),       12);
  f = pow(b,y); assertclose(f.val(), 9); assertclose(f.grd(0), 0); assertclose(f.grd(1), c9*log_a);
  const double c=3;
  f = pow(x,c); assertclose(f.val(),64); assertclose(f.grd(0),48); assertclose(f.grd(1),        0);
  f = pow(y,c); assertclose(f.val(), 8); assertclose(f.grd(0), 0); assertclose(f.grd(1),       12);
  f = pow(c,y); assertclose(f.val(), 9); assertclose(f.grd(0), 0); assertclose(f.grd(1), c9*log_a);
  const long double d=3;
  f = pow(x,d); assertclose(f.val(),64); assertclose(f.grd(0),48); assertclose(f.grd(1),        0);
  f = pow(y,d); assertclose(f.val(), 8); assertclose(f.grd(0), 0); assertclose(f.grd(1),       12);
  f = pow(d,y); assertclose(f.val(), 9); assertclose(f.grd(0), 0); assertclose(f.grd(1), c9*log_a);
  const int n=3;
  f = pow(x,n); assertclose(f.val(),64); assertclose(f.grd(0),48); assertclose(f.grd(1),        0);
  f = pow(y,n); assertclose(f.val(), 8); assertclose(f.grd(0), 0); assertclose(f.grd(1),       12);
  f = pow(n,y); assertclose(f.val(), 9); assertclose(f.grd(0), 0); assertclose(f.grd(1), c9*log_a);
}

template <typename ad_type> static
void test_binary_fn_hypot() {
  ad_type f,x(4,0),y(3,1);
  typedef typename ad_type::value_type value_type;
  const value_type c3=3,c4=4,c5=5;
  f = hypot(x,y); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),c3/c5);
  f = hypot(y,x); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),c3/c5);
  const value_type a=3;
  f = hypot(x,a); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),    0);
  f = hypot(a,x); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),    0);
  f = hypot(x,3); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),    0);
  f = hypot(3,x); assertclose(f.val(),5); assertclose(f.grd(0),c4/c5); assertclose(f.grd(1),    0);
  const value_type b=4;
  f = hypot(y,b); assertclose(f.val(),5); assertclose(f.grd(0),    0); assertclose(f.grd(1),c3/c5);
  f = hypot(b,y); assertclose(f.val(),5); assertclose(f.grd(0),    0); assertclose(f.grd(1),c3/c5);
  f = hypot(y,4); assertclose(f.val(),5); assertclose(f.grd(0),    0); assertclose(f.grd(1),c3/c5);
  f = hypot(4,y); assertclose(f.val(),5); assertclose(f.grd(0),    0); assertclose(f.grd(1),c3/c5);
}

template <typename float_type> static
void test_real() {
  typedef float_type value_type;
  typedef MiniFAD::Fad<value_type,0> ad_type_0;
  typedef MiniFAD::Fad<value_type,2> ad_type_2;
  typedef MiniFAD::Fad<value_type,3> ad_type_3;
  test_binary_fn_atan2 <ad_type_0>();
  test_binary_fn_atan2 <ad_type_2>();
  test_binary_fn_atan2 <ad_type_3>();
  test_binary_fn_pow   <ad_type_0>();
  test_binary_fn_pow   <ad_type_2>();
  test_binary_fn_pow   <ad_type_3>();
  test_binary_fn_hypot <ad_type_0>();
  test_binary_fn_hypot <ad_type_2>();
  test_binary_fn_hypot <ad_type_3>();
}

#if MINIFAD_HAVE_COMPLEX_CXX || MINIFAD_HAVE_COMPLEX_C99
template <typename real_type, typename complex_type> static
void test_complex_common() {
  typedef MiniFAD::Fad<complex_type,0> ad_type_0;
  typedef MiniFAD::Fad<complex_type,2> ad_type_2;
  typedef MiniFAD::Fad<complex_type,3> ad_type_3;
  test_binary_fn_pow<ad_type_0>();
  test_binary_fn_pow<ad_type_2>();
  test_binary_fn_pow<ad_type_3>();
}
#endif

template <typename float_type> static
void test_complex_cxx() {
#if MINIFAD_HAVE_COMPLEX_CXX
  typedef std::complex<float_type> complex_type;
  test_complex_common<float_type,complex_type>();
  typedef MiniFAD::Fad<complex_type,0> ad_type_0;
  typedef MiniFAD::Fad<complex_type,2> ad_type_2;
  {
    typedef ad_type_0 ad_type;
    const float_type n2=2,n3=3;
    const complex_type j(0,1);
    const ad_type x(j,0);
    ad_type f;
    f = pow(x,2); assertclose(f.val(),-1); assertclose(f.grd(0),n2*pow(x,1).val()); assertclose(f.grd(1),0);
    f = pow(x,3); assertclose(f.val(),-j); assertclose(f.grd(0),n3*pow(x,2).val()); assertclose(f.grd(1),0);
  }
  {
    typedef ad_type_2 ad_type;
    const float_type n2=2,n3=3;
    const complex_type j(0,1);
    const ad_type x(j,0);
    ad_type f;
    f = pow(x,2); assertclose(f.val(),-1); assertclose(f.grd(0),n2*pow(x,1).val()); assertclose(f.grd(1),0);
    f = pow(x,3); assertclose(f.val(),-j); assertclose(f.grd(0),n3*pow(x,2).val()); assertclose(f.grd(1),0);
  }
#endif
}
#if MINIFAD_HAVE_FLOAT128
template <> //static
void test_complex_cxx<__float128>() {}
#endif

template <typename float_type> static
void test_complex_c99() {
#if MINIFAD_HAVE_COMPLEX_C99
  typedef typename c99complex<float_type>::type complex_type;
  test_complex_common<float_type,complex_type>();
#endif
}

template <typename float_type> static
void test_complex() {
  test_complex_cxx<float_type>();
  test_complex_c99<float_type>();
}

template <typename float_type>
static void test() {
  test_real<float_type>();
  test_complex<float_type>();
}

int main(int,char *[])
{
  test< float       >();
  test< double      >();
  test< long double >();
  #if MINIFAD_HAVE_FLOAT128 && MINIFAD_HAVE_QUADMATH
  test< __float128  >();
  #endif
  return 0;
}
