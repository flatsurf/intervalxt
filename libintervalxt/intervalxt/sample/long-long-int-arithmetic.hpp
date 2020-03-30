/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Vincent Delecroix
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

#ifndef LIBINTERVALXT_SAMPLE_MPZ_ARITHMETIC_HPP
#define LIBINTERVALXT_SAMPLE_MPZ_ARITHMETIC_HPP

#include <string>

#include "arithmetic.hpp"

namespace intervalxt::sample {

template <>
struct Arithmetic<long long int> {
  using T = long long int;

  static std::vector<mpq_class> coefficients(const T& value) {
    return std::vector{mpq_class(std::to_string(value).c_str())};
  }

  static T floorDivision(const T& divident, const T& divisor) { return divident / divisor; }
};

}  // namespace intervalxt::sample

#endif
