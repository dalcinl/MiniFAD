MiniFAD
=======

Overview
--------

*MiniFAD* is a minimalistic forward-mode automatic differentiation
(AD) library for C++ codes. It is based on expression templates with
caching of partial derivatives.

Features:

* Single, self-contained C++ header file working in
  C++98, C++03, and C++11 mode.
* Support for single, double, extended, and quadruple precision.
  Quadruple precision requires GCC 4.6+ or Intel C++ compiler
  and [libquadmath](http://gcc.gnu.org/onlinedocs/libquadmath/).
* Support for C++ and C99 complex numbers.
* Static and dynamic storage of derivative arrays.

Additional information and resources:

* [Wikipedia](http://en.wikipedia.org/wiki/Automatic_differentiation)
* [Community portal on AD](http://www.autodiff.org/)
* [Sacado](http://trilinos.org/packages/sacado/) package
* [FADBAD++](http://www.fadbad.com/fadbad.html) package

Installation
------------

Just copy the `minifad.hpp` header file to whatever place you prefer
and adjust the include path of your C++ compiler accordingly.
