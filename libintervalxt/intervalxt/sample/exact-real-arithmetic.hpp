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

#ifndef LIBINTERVALXT_SAMPLE_EXACT_REAL_ARITHMETIC_HPP
#define LIBINTERVALXT_SAMPLE_EXACT_REAL_ARITHMETIC_HPP

#include <exact-real/element.hpp>

#include "arithmetic.hpp"

namespace intervalxt::sample {

template <typename Ring>
struct Arithmetic<exactreal::Element<Ring>> {
  using T = exactreal::Element<Ring>;

  static std::vector<mpq_class> coefficients(const T& value) { return value.template coefficients<mpq_class>(); }
  static auto floorDivision(const T& divident, const T& divisor) {
    return divident.floordiv(divisor);
  }
};

}

#endif

