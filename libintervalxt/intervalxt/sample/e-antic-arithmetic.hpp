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

#ifndef LIBINTERVALXT_SAMPLE_E_ANTIC_ARITHMETIC_HPP
#define LIBINTERVALXT_SAMPLE_E_ANTIC_ARITHMETIC_HPP

#include <e-antic/renfxx.h>

#include "arithmetic.hpp"

namespace intervalxt::sample {

template <>
struct Arithmetic<eantic::renf_elem_class> {
  using T = eantic::renf_elem_class;

  static std::vector<mpq_class> coefficients(const T& value) {
    std::vector<mpq_class> ret;
    auto d = value.den();
    for (auto i : value.num_vector()) {
      auto dat = mpq_class(i, d);
      dat.canonicalize();
      ret.push_back(dat);
    }
    return ret;
  }

  static auto floorDivision(const T& divident, const T& divisor) { return (divident / divisor).floor(); }
};

}  // namespace intervalxt::sample

#endif
