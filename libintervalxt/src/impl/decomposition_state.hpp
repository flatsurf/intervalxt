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
#include <memory>
#include <unordered_map>
#include <list>

#include "forward.hpp"
#include "component_state.hpp"
#include "lengths_with_connections.hpp"

namespace intervalxt {

template <typename T>
using stable_container = std::deque<T>;

using std::list;
using std::unordered_map;

// TODO: When the Labels happen to be hashed continuously, use a vector for
// top/bottom instead, i.e., build a map with fixed key set and try to hash
// optimally (at least when that's trivial to do.)

struct DecompositionState {
  struct HalfEdgeConnections {
    // Connections to the left of this half edge, from top to bottom. The half
    // edge is attached at the top of these connections.
    list<Connection> left;
    // Connections to the right of this half edge, from bottom to top. The half
    // edge is attached at the bottom of these connections.
    list<Connection> right;
  };

  stable_container<ComponentState> components;
  // Previously detected connections attached to top half edges.
  unordered_map<Label, HalfEdgeConnections> top;
  // Previously detected connections attached to bottom half edges.
  unordered_map<Label, HalfEdgeConnections> bottom;

  friend std::ostream& operator<<(std::ostream&, const DecompositionState&);
};

}

#endif

