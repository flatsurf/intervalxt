/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *                      2019-2020 Julian Rüth
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

#ifndef LIBINTERVALXT_SAMPLE_MPZ_COEFFICIENTS_HPP
#define LIBINTERVALXT_SAMPLE_MPZ_COEFFICIENTS_HPP

#include <gmpxx.h>

#include "coefficients.hpp"

namespace intervalxt::sample {

namespace {

template <>
struct Coefficients<mpz_class> {
  using T = mpz_class;

  std::vector<std::vector<mpq_class>> operator()(const std::vector<T>& elements) {
    std::vector<std::vector<mpq_class>> ret;
    for (auto x : elements)
      ret.push_back({x});

    return ret;
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
