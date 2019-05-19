#include <gmpxx.h>

#include "intervalxt/iet.hpp"

// Floor divisions
// (needed for Zorich acceleration)

template <>
unsigned long fdiv<unsigned long, unsigned long>(unsigned long& a, unsigned long& b) {
  return a / b;
}

template <>
unsigned long fdiv<mpz_class, unsigned long>(mpz_class& a, mpz_class& b) {
  mpz_class r;
  mpz_fdiv_q(r.__get_mp(), a.__get_mp(), b.__get_mp());
  if (!mpz_fits_ulong_p(r.__get_mp()))
    throw std::runtime_error("overflow");
  return mpz_get_ui(r.__get_mp());
}

template <>
mpz_class fdiv<mpz_class, mpz_class>(mpz_class& a, mpz_class& b) {
  mpz_class res;
  mpz_fdiv_q(res.__get_mp(), a.__get_mp(), b.__get_mp());
  return res;
}
