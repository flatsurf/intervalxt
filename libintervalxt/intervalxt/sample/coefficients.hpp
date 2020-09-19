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

#ifndef LIBINTERVALXT_SAMPLE_COEFFICIENTS_HPP
#define LIBINTERVALXT_SAMPLE_COEFFICIENTS_HPP

#include <gmpxx.h>

#include <type_traits>
#include <vector>
#include <tuple>

namespace intervalxt::sample {

namespace {

// Given an element x returns the coefficients of x over some appropriate
// rational vector space. E.g., given a number field element, writes that
// element in some power basis and returns the coefficients over this basis.
// You must provide a specialization of this class for each type that you want
// to use in a sample::Lengths. See the other files in this directory for some
// examples.
template <typename S = void, typename _ = void>
class Coefficients {
  template <typename>
  static constexpr bool false_t = false;

 public:
  using T = S;

  std::vector<std::vector<mpq_class>> operator()(const std::vector<T>& elements) {
    static_assert(false_t<T>, "operator() returning rational coefficients of this element not implemented; did you include the appropriate coefficients header?");
    throw std::logic_error("not implemented: Coefficients::operator(); did you include the appropriate coefficients header?");
  }
};

// A wrapper for the general Coefficients which applies to tuples of fixed
// length instead of vectors.
template <>
class Coefficients<void, void> {
  template <typename T, std::size_t... I>
  static auto makeTuple(const std::vector<T>& elements, std::index_sequence<I...>) {
    return std::make_tuple(elements[I]...);
  }

 public:
  // Helper struct that applies C::operator() to the arguments.
  template <typename C>
  struct Map {
    template <typename... Args>
    auto operator()(Args&&... elements) {
      constexpr int N = sizeof...(Args);
      if constexpr (N == 0) {
        return std::tuple<>();
      } else {
        using T = typename std::decay_t<typename std::tuple_element<0, std::tuple<Args...>>::type>;
        auto coefficients = C()(std::vector<T>{std::forward<Args>(elements)...});
        return makeTuple(coefficients, std::make_index_sequence<N>());
      }
    }
  };

  // Apply Coefficients<T>::operator() to the arguments, i.e., return a tuple
  // of vectors of rational numbers representing the coefficients of the
  // elements in some common vector space.
  template <typename... Args>
  auto operator()(Args&&... elements) {
    constexpr int N = sizeof...(Args);
    if constexpr (N == 0) {
      return std::tuple<>();
    } else {
      using T = typename std::decay_t<typename std::tuple_element<0, std::tuple<Args...>>::type>;
      return Map<Coefficients<T>>()(std::forward<Args>(elements)...);
    }
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
