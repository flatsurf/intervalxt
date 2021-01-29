/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include "copyable.hpp"

namespace intervalxt {

class Connection : boost::equality_comparable<Connection> {
  // Connections can not be created directly (other than copying & moving them.)
  // They are created in the process of a DynamicalDecomposition.
  template <typename... Args>
  Connection(PrivateConstructor, Args&&... args);

 public:
  Connection operator-() const;

  // Return whether two connections are considered equal.
  // Note that we consider two connections as equal when they have the same
  // orientation and are attached to the same labels, i.e., even if they come
  // from different dynamical decompositions. We do this to make
  // DynamicalDecomposition::inject() work which can create pairs of
  // connections on different decompositions.
  bool operator==(const Connection&) const;

  // Whether the connection is going from bottom to top.
  bool parallel() const;
  // Whether the connection is going from top to bottom.
  bool antiparallel() const;

  Separatrix source() const;
  Separatrix target() const;

  friend std::ostream& operator<<(std::ostream&, const Connection&);

 private:
  Copyable<Connection> self;

  friend ImplementationOf<Connection>;
};

}  // namespace intervalxt

namespace std {

template <>
struct hash<intervalxt::Connection> { size_t operator()(const intervalxt::Connection&) const; };

}  // namespace std

#endif
