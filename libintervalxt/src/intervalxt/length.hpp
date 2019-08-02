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

#include <gmpxx.h>
#include <boost/operators.hpp>

#include "intervalxt/forward.hpp"

namespace intervalxt {

// A sample implementation of a length of a vector in ℝ² as a simple T.
template <typename T>
class Length : boost::totally_ordered<Length<T>>, boost::additive<Length<T>>, boost::multipliable<Length<T>, mpz_class> {
 public:
  using Quotient = mpz_class;

  Length();
  Length(const T&);

  bool operator==(const Length&) const noexcept;
  bool operator<(const Length&) const noexcept;
  Length& operator+=(const Length&) noexcept;
  Length& operator-=(const Length&) noexcept;
  Length& operator*=(const Quotient&) noexcept;

  explicit operator bool() const noexcept;

  // Return the floor of the division of this length by the argument.
  Quotient operator/(const Length&);

  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const Length<C>&);

  const T& length() const;

 private:
  T value;
};

}  // namespace intervalxt

#endif
