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

#ifndef LIBINTERVALXT_SAMPLE_LENGTH_HPP
#define LIBINTERVALXT_SAMPLE_LENGTH_HPP

#include <utility>
#include <vector>
#include <type_traits>

#include <gmpxx.h>

#include <boost/operators.hpp>

#include "arithmetic.hpp"

namespace intervalxt::sample {

// A sample implementation of a length of a vector in ℝ² as a simple T.
template <typename Arithmetic>
class Length : boost::totally_ordered<Length<Arithmetic>>,
               boost::totally_ordered<Length<Arithmetic>, typename Arithmetic::T>,
               boost::totally_ordered<Length<Arithmetic>, std::pair<typename Arithmetic::T, typename Arithmetic::T>>,
               boost::additive<Length<Arithmetic>>,
               boost::multipliable<Length<Arithmetic>, QuotientFloorDivision<Arithmetic>> {
 public:
  using T = typename Arithmetic::T;

  Length();
  Length(const T&);

  bool operator==(const Length&) const noexcept;
  bool operator<(const Length&) const noexcept;
  bool operator==(const T&) const noexcept;
  bool operator<(const T&) const noexcept;
  bool operator>(const T&) const noexcept;
  // Compare this length with the length of the vector (x, y)
  bool operator==(const std::pair<T, T>&) const noexcept;
  bool operator<(const std::pair<T, T>&) const noexcept;
  bool operator>(const std::pair<T, T>&) const noexcept;
  Length& operator+=(const Length&) noexcept;
  Length& operator-=(const Length&) noexcept;
  Length& operator*=(const QuotientFloorDivision<Arithmetic>&) noexcept;

  explicit operator bool() const noexcept;

  template <typename S, typename std::enable_if_t<std::is_convertible_v<T, S>, bool> Enabled = true>
  operator Length<::intervalxt::sample::Arithmetic<S>>() const;
  template <typename S, typename std::enable_if_t<std::is_constructible_v<S, T>, bool> Enabled = true>
  explicit operator Length<::intervalxt::sample::Arithmetic<S>>() const;
  // Conversion from integral S via mpz_class
  template <typename S, typename std::enable_if_t<!std::is_constructible_v<S, T> && std::is_integral_v<T> && std::is_constructible_v<S, mpz_class>, bool> Enabled = true>
  explicit operator Length<::intervalxt::sample::Arithmetic<S>>() const;

  // Return the coefficients of this length written as a linear combination in
  // a suitable basis that makes all coefficients rational, such as a number
  // field basis, or a basis of random reals.
  std::vector<mpq_class> coefficients() const;

  // Return the floor of the division of this length by the argument.
  QuotientFloorDivision<Arithmetic> operator/(const Length&);

  template <typename A>
  friend std::ostream& operator<<(std::ostream&, const Length<A>&);

 protected:
  T value;

  template <typename Archive, typename A>
  friend void load(Archive&, Length<A>&);
  template <typename Archive, typename A>
  friend void save(Archive&, const Length<A>&);
};

}  // namespace intervalxt::sample

#include "detail/length.ipp"

#endif
