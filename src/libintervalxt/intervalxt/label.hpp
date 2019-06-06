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

#ifndef LIBINTERVALXT_LABEL_HPP
#define LIBINTERVALXT_LABEL_HPP

#include <vector>
#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"
#include "intervalxt/length.hpp"

namespace intervalxt {

// Data attached to a pair of matched intervals in an Interval Exchange
// Transformation.
template <typename Coordinate>
class Label : boost::equality_comparable<Label<Coordinate>> {
 public:
  using Length = intervalxt::Length<Coordinate>;

  // TODO: A label should take a shared_ptr to something gets notified of
  // subtract calls so that we can make the cocycle tracking optional.
  Label();
  Label(const Length& length);

  // Return whether two labels are considered the same. Note that equal labels
  // might not be identicial, e.g., when creating a copy of a label and the
  // modifying one of the copies, the two instances are still considered equal.
  bool operator==(const Label&) const noexcept;

  // Compare two labels so they can be used in STL containers. The comparison
  // has no semantic meaning but is compatible with the operator==.
  bool operator<(const Label&) const noexcept;

  const Length& length() const noexcept;
  Length& length() noexcept;

  void subtract(Label&, const Quotient<Coordinate>& multiplicity = 1) noexcept;

  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const Label<C>&);
 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

}  // namespace intervalxt

#endif
