/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019      Vincent Delecroix
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

#ifndef LIBINTERVALXT_HALF_EDGE_HPP
#define LIBINTERVALXT_HALF_EDGE_HPP

#include <boost/operators.hpp>
#include <functional>
#include <list>
#include <utility>

#include "copyable.hpp"

namespace intervalxt {

class LIBINTERVALXT_API HalfEdge : boost::equality_comparable<HalfEdge> {
  // Half edges cannot be created directly but only taken from the contours of a Component.
  template <typename... Args>
  HalfEdge(PrivateConstructor, Args&&...);

 public:
  Component component() const;

  // Whether this is a HalfEdge in the top contour.
  bool top() const;
  // Whether this is a HalfEdge in the bottom contour.
  bool bottom() const;

  // Return the equally labeled HalfEdge in the other contour.
  HalfEdge operator-() const;

  // Return the Separatrix following this HalfEdge (if this is not the last one)
  // the name of this half edge after skipping over all the collapsed
  // connections at its right hand end.
  std::optional<Separatrix> separatrix() const;

  // Return the next HalfEdge in the contour (if this is not the last one.)
  std::optional<HalfEdge> next() const;
  // Return the previous HalfEdge in the contour (if this is not the first one.)
  std::optional<HalfEdge> previous() const;

  // Return the connections to go (counterclockwise, i.e., inside the component) across this HalfEdge.
  std::list<Side> cross() const;

  // Connections to the left of this half edge, from bottom to top, each oriented from top to bottom.
  std::list<Connection> left() const;
  // Connections to the right of this half edge, from bottom to top, each oriented from bottom to top.
  std::list<Connection> right() const;

  operator Label() const;

  bool operator==(const HalfEdge& rhs) const;

  LIBINTERVALXT_API friend std::ostream& operator<<(std::ostream&, const HalfEdge&);

 private:
  Copyable<HalfEdge> self;

  friend ImplementationOf<HalfEdge>;
};

}  // namespace intervalxt

namespace std {

template <>
struct LIBINTERVALXT_API hash<intervalxt::HalfEdge> { size_t operator()(const intervalxt::HalfEdge&) const; };

}  // namespace std

#endif
