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

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"

namespace intervalxt {

template <typename Coordinate>
using Quotient = std::conditional_t<std::is_integral_v<Coordinate>, Coordinate, mpz_class>;

// The length of a vector in ℝ².
template <typename Coordinate>
class Length : boost::totally_ordered<Length<Coordinate>>, boost::additive<Length<Coordinate>>, boost::multipliable<Length<Coordinate>, Quotient<Coordinate>> {
 public:
  Length();
  Length(const Coordinate& length);
  Length(const Coordinate& x, const Coordinate y);

  bool operator==(const Length&) const noexcept;
  bool operator<(const Length&) const noexcept;
  Length& operator+=(const Length&) noexcept;
  Length& operator-=(const Length&) noexcept;
  Length& operator*=(const Quotient<Coordinate>&) noexcept;

  // Return the floor of the division of this length by the argument.
  Quotient<Coordinate> operator/(const Length&);
  
  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const Length<C>&);
 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

}

#endif
