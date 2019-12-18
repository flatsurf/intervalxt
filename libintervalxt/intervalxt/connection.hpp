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

#ifndef LIBINTERVALXT_CONNECTION_HPP
#define LIBINTERVALXT_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace intervalxt {

template <typename Length>
class Connection : boost::equality_comparable<Connection<Length>> {
  // Connections can not be created directly (other than copying & moving them.)
  // They are created as products of DynamicalDecomposition.
  Connection();

 public:
  Connection operator-() const noexcept;

  MaybeConnection<Length> source() const noexcept;

  MaybeConnection<Length> nextInBoundary() const noexcept;

  bool operator==(const Connection&) const noexcept;

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const Connection<T>&);

  friend class DynamicalDecomposition<Length>;
  friend class Component<Length>;
  friend class MaybeConnection<Length>;
};

#include "detail/dynamical_decomposition.ipp"

}  // namespace intervalxt

#endif
