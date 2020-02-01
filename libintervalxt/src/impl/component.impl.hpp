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

#ifndef LIBINTERVALXT_COMPONENT_IMPL_HPP
#define LIBINTERVALXT_COMPONENT_IMPL_HPP

#include <memory>

#include "../../intervalxt/component.hpp"

#include "component_state.hpp"

#include "forward.hpp"

namespace intervalxt {

template <>
class Implementation<Component> {
 public:
  Implementation(std::shared_ptr<DecompositionState>, ComponentState*);

  static Component make(std::shared_ptr<DecompositionState>, ComponentState*);
  static int boshernitzanCost(const IntervalExchangeTransformation&);
  static std::vector<Side> horizontal(const Component& component, bool top);
  static void registerSeparating(Component& left, const Connection&, Component& right);
  static std::optional<HalfEdge> next(const Component&, const HalfEdge&);
  static std::optional<HalfEdge> previous(const Component&, const HalfEdge&);

  static std::shared_ptr<DecompositionState> parent(const Component&);

  // Return the connections for walking clockwise from the counterclockwise end
  // of from to the clockwise end of to.
  std::list<Side> walkClockwise(HalfEdge from, HalfEdge to) const;

  // Return the connections for walking counterclockwise from the clockwise end
  // of from to the counterclockwise end of to.
  std::list<Side> walkCounterclockwise(HalfEdge from, HalfEdge to) const;

  const std::shared_ptr<DecompositionState> decomposition;
  ComponentState& state;
};

}

#endif

