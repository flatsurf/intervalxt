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

#ifndef LIBINTERVALXT_UNMATCHED_CONNECTION_HPP
#define LIBINTERVALXT_UNMATCHED_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace intervalxt {

template <typename Length>
class MaybeConnection : boost::equality_comparable<MaybeConnection<Length>> {
  // Connections can not be created directly (other than copying & moving them.)
  // They are created as products of DynamicalDecomposition.
  MaybeConnection();

 public:
  // Return the component that contains this connection on its inside or
  // boundary, i.e., the component on the left if this is a connection "from
  // bottom to top" or the connection on its right if this is a connection
  // "from top to bottom".
  const Component<Length>& component() const noexcept;

  // Return the next connection at the source of this singularity in counter-clockwise order.
  MaybeConnection nextAtSingularity() const noexcept;

  // Return the next connection at the source of this singularity in clockwise order.
  MaybeConnection previousAtSingularity() const noexcept;

  Label<Length> before() const noexcept;
  Label<Length> after() const noexcept;

  bool operator==(const MaybeConnection<Length>&) const noexcept;

  std::optional<Connection<Length>> connection() const noexcept;

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const MaybeConnection<T>&);

  friend class DynamicalDecomposition<Length>;
  friend class Component<Length>;
  friend class Connection<Length>;
};

#include "detail/dynamical_decomposition.ipp"

}  // namespace intervalxt

#endif
