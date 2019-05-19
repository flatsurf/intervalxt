/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <gmpxx.h>

#include "intervalxt/iet.hpp"

namespace intervalxt {
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
}  // namespace intervalxt
