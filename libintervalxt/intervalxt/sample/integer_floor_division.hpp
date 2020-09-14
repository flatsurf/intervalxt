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

#ifndef LIBINTERVALXT_SAMPLE_INTEGER_FLOOR_DIVISION_HPP
#define LIBINTERVALXT_SAMPLE_INTEGER_FLOOR_DIVISION_HPP

#include "floor_division.hpp"

namespace intervalxt::sample {

namespace {

template <typename S>
struct FloorDivision<S, S, typename std::enable_if<std::is_integral_v<S>>::type> {
  using T = S;

  T operator()(const T divident, const T divisor) {
    return divident / divisor;
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
