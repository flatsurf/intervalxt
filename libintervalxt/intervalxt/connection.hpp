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

class Connection : boost::equality_comparable<Connection> {
  // Connections can not be created directly (other than copying & moving them.)
  // They are created in the process of a DynamicalDecomposition.
  Connection();

 public:
  Connection operator-() const noexcept;

  bool operator==(const Connection&) const noexcept;

  // Whether the connection is going from bottom to top.
  bool parallel() const noexcept;
  // Whether the connection is going from top to bottom.
  bool antiparallel() const noexcept;

  Separatrix source() const noexcept;
  Separatrix target() const noexcept;

  friend std::ostream& operator<<(std::ostream&, const Connection&);

 private:
  using Implementation = ::intervalxt::Implementation<Connection>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;
};

}  // namespace intervalxt

namespace std {

template<>
struct hash<intervalxt::Connection> { size_t operator()(const intervalxt::Connection&) const noexcept; };

}

#endif
