/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Vincent Delecroix
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBINTERVALXT_SAMPLE_INTEGER_COEFFICIENTS_HPP
#define LIBINTERVALXT_SAMPLE_INTEGER_COEFFICIENTS_HPP

#include "../external/gmpxxll/mpz_class.hpp"
#include "coefficients.hpp"

namespace intervalxt::sample {

namespace {

template <typename S>
struct Coefficients<S, typename std::enable_if<std::is_integral_v<S>>::type> {
  using T = S;

  std::vector<std::vector<mpq_class>> operator()(const std::vector<T>& elements) {
    std::vector<std::vector<mpq_class>> ret(elements.size(), {mpq_class()});
    for (size_t i = 0; i < elements.size(); i++)
      ret[i][0].get_num() = gmpxxll::mpz_class(elements[i]);

    return ret;
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
