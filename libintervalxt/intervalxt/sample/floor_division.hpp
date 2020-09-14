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

#ifndef LIBINTERVALXT_SAMPLE_FLOOR_DIVISION_HPP
#define LIBINTERVALXT_SAMPLE_FLOOR_DIVISION_HPP

#include <gmpxx.h>

#include <type_traits>

namespace intervalxt::sample {

namespace {

// Implements floor divisions for a type S. You must provide a specialization
// of this struct for each type that you want to use in a sample::Lengths. See
// the other files in this directory for some examples.
template <typename S, typename R = std::conditional_t<std::is_integral_v<S>, S, mpz_class>, typename _ = void>
struct FloorDivision {
 private:
  template <typename>
  static constexpr bool false_t = false;

 public:
  using T = S;

  R operator()(const T&, const T&) {
    static_assert(false_t<T>, "operator() performing a floor division not implemented for this type");
    throw std::logic_error("not implemented: FloorDivision::operator()");
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
