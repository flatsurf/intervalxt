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

#ifndef LIBINTERVALXT_LENGTH_WITH_COEFFICIENTS_HPP
#define LIBINTERVALXT_LENGTH_WITH_COEFFICIENTS_HPP

#include <vector>
#include <boost/operators.hpp>

#include "intervalxt/forward.hpp"

namespace intervalxt {

template <typename Length, typename Coefficients>
class LengthWithCoefficients : boost::totally_ordered<LengthWithCoefficients<Length>>, boost::additive<LengthWithCoefficients<Length>>, boost::multipliable<LengthWithCoefficients<Length>, typename Length::Quotient> {
 public:
  using Quotient = typename Length::Quotient;
  using ModuleCoefficient = typename Length::ModuleCoefficient;
  using Value = typename Length::Value;

  LengthWithCoefficients();
  LengthWithCoefficients(const Value&, const Coefficients&);

  bool operator==(const LengthWithCoefficients&) const noexcept;
  bool operator<(const LengthWithCoefficients&) const noexcept;
  LengthWithCoefficients& operator+=(const LengthWithCoefficients&) noexcept;
  LengthWithCoefficients& operator-=(const LengthWithCoefficients&) noexcept;
  LengthWithCoefficients& operator*=(const Quotient&) noexcept;

  explicit operator bool() const noexcept;

  // Return the module coefficients as a (fixed length) vector of integers (discarding the denominator)
  size_t degree() const;
  std::vector<ModuleCoefficient> moduleCoefficients() const;

  // Return the floor of the division of this length by the argument.
  Quotient operator/(const LengthWithCoefficients&);

  template <typename L, typename C>
  friend std::ostream& operator<<(std::ostream&, const LengthWithCoefficients<L, C>&);

  const Value& length() const;

 private:
  Length value;
  Coefficients coefficients;
};
}  // namespace intervalxt

#endif

