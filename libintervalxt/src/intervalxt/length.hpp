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

#ifndef LIBINTERVALXT_LENGTH_HPP
#define LIBINTERVALXT_LENGTH_HPP

#include <utility>

#include <gmpxx.h>

#include <boost/operators.hpp>

#include "intervalxt/forward.hpp"

namespace intervalxt {

// The operations needed to implement a Length<T>.
// We expect T to support an operator* and operator+, comparison with integers and mpz_class.
template <typename T>
struct LengthArithmetic {
  static std::vector<mpq_class> coefficients(const T& value);
  static std::conditional_t<std::is_integral_v<T>, T, mpz_class> floorDivision(const T& divident, const T& divisor);

  using QuotientFloorDivision = typename std::invoke_result_t<decltype(&LengthArithmetic::floorDivision),const T&, const T&>;
};

// A sample implementation of a length of a vector in ℝ² as a simple T.
template <typename T>
class Length : boost::totally_ordered<Length<T>>,
               boost::totally_ordered<Length<T>, T>,
               boost::totally_ordered<Length<T>, std::pair<T, T>>,
               boost::additive<Length<T>>,
               boost::multipliable<Length<T>, typename LengthArithmetic<T>::QuotientFloorDivision> {
 public:
  Length();
  Length(const T&);
  template <typename S, typename std::enable_if_t<std::is_convertible_v<S, T>, bool> Enabled = true>
  Length(const Length<S>&);

  bool operator==(const Length&) const noexcept;
  bool operator<(const Length&) const noexcept;
  bool operator==(const T&) const noexcept;
  bool operator<(const T&) const noexcept;
  bool operator>(const T&) const noexcept;
  // Compare this length with the length of the vector (x, y)
  bool operator==(const std::pair<T, T>&) const noexcept;
  bool operator<(const std::pair<T, T>&) const noexcept;
  bool operator>(const std::pair<T, T>&) const noexcept;
  Length& operator+=(const Length<T>&) noexcept;
  Length& operator-=(const Length<T>&) noexcept;
  Length& operator*=(const typename LengthArithmetic<T>::QuotientFloorDivision&) noexcept;

  explicit operator bool() const noexcept;

  // Return the coefficients of this length written as a linear combination in
  // a suitable basis that makes all coefficients rational, such as a number
  // field basis, or a basis of random reals.
  std::vector<mpq_class> coefficients() const;

  // Return the floor of the division of this length by the argument.
  typename LengthArithmetic<T>::QuotientFloorDivision operator/(const Length<T>&);

  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const Length<C>&);

 private:
  T value;

  template <typename Archive, typename S>
  friend void load(Archive&, Length<S>&);
  template <typename Archive, typename S>
  friend void save(Archive&, const Length<S>&);
};

}  // namespace intervalxt

#include "intervalxt/detail/length.ipp"

#endif
