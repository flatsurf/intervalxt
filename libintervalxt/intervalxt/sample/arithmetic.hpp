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

#ifndef LIBINTERVALXT_SAMPLE_ARITHMETIC_HPP
#define LIBINTERVALXT_SAMPLE_ARITHMETIC_HPP

#include <type_traits>
#include <vector>

#include <gmpxx.h>
#include <boost/lexical_cast.hpp>

namespace intervalxt::sample {

template <typename Arithmetic>
using QuotientFloorDivision = typename std::invoke_result_t<decltype(&Arithmetic::floorDivision), const typename Arithmetic::T&, const typename Arithmetic::T&>;

template <typename S, typename _ = void>
struct Arithmetic {
  using T = S;

  static std::vector<mpq_class> coefficients(const T& value) {
    if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
      return std::vector<mpq_class>{mpq_class(boost::lexical_cast<std::string>(value))};
    } else {
      return std::vector<mpq_class>{value};
    }
  }
  static auto floorDivision(const T& divident, const T& divisor) { return divident / divisor; }
};

}  // namespace intervalxt::sample

#endif
