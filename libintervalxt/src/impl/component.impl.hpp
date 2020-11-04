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
#include "../../intervalxt/half_edge.hpp"
#include "../../intervalxt/interval_exchange_transformation.hpp"
#include "connection.impl.hpp"
#include "decomposition_state.hpp"
#include "implementation_of_decomposition.hpp"

namespace intervalxt {

template <>
class ImplementationOf<Component> : public ImplementationOfDecomposition {
 public:
  ImplementationOf(const DynamicalDecomposition&, DecompositionState::Component*);
  static Component make(const DynamicalDecomposition&, DecompositionState::Component*);

  static std::optional<int> boshernitzanCost(const IntervalExchangeTransformation&);
  static std::vector<Side> horizontal(const Component& component, bool top);
  static void registerSeparating(Component& left, DecompositionState::Connection, Component& right);
  static std::optional<HalfEdge> next(DecompositionState::Component*, const HalfEdge&, const DynamicalDecomposition&);
  static std::optional<HalfEdge> previous(DecompositionState::Component*, const HalfEdge&, const DynamicalDecomposition&);

  // Return the connections for walking clockwise from the counterclockwise end
  // of from to the clockwise end of to.
  std::list<Side> walkClockwise(HalfEdge from, HalfEdge to) const;

  // Return the connections for walking counterclockwise from the clockwise end
  // of from to the counterclockwise end of to.
  std::list<Side> walkCounterclockwise(HalfEdge from, HalfEdge to) const;

  DecompositionState::Component* component;
};

template <typename... Args>
Component::Component(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<Component>>(std::forward<Args>(args)...)) {}

}  // namespace intervalxt

#endif
