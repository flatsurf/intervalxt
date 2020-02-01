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

#ifndef LIBINTERVALXT_DECOMPOSITION_STATE_HPP
#define LIBINTERVALXT_DECOMPOSITION_STATE_HPP

#include <deque>
#include <list>
#include <memory>
#include <unordered_map>

#include "component_state.hpp"
#include "forward.hpp"
#include "lengths_with_connections.hpp"

namespace intervalxt {

template <typename T>
using stable_container = std::deque<T>;

using std::list;
using std::unordered_map;

struct DecompositionState {
  struct HalfEdgeConnections {
    // Connections to the left of this half edge, from bottom to top, each oriented from top to bottom.
    list<Connection> left;
    // Connections to the right of this half edge, from bottom to top, each oriented from bottom to top.
    list<Connection> right;
  };

  stable_container<ComponentState> components;
  // Previously detected connections attached to top half edges.
  unordered_map<Label, HalfEdgeConnections> top;
  // Previously detected connections attached to bottom half edges.
  unordered_map<Label, HalfEdgeConnections> bottom;

  void check() const;

  friend std::ostream& operator<<(std::ostream&, const DecompositionState&);
};

}  // namespace intervalxt

#endif
