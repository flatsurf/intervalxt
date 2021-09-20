/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../intervalxt/half_edge.hpp"

#include <ostream>
// Work around https://github.com/simonask/rx-ranges/pull/48
#include <limits>

#include "../intervalxt/component.hpp"
#include "../intervalxt/separatrix.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/component.impl.hpp"
#include "impl/connection.impl.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/half_edge.impl.hpp"
#include "impl/implementation_of_decomposition.hpp"
#include "impl/separatrix.impl.hpp"
#include "util/assert.ipp"

namespace intervalxt {

bool HalfEdge::top() const {
  return self->contour == ImplementationOf<HalfEdge>::Contour::TOP;
}

bool HalfEdge::bottom() const {
  return not top();
}

bool HalfEdge::operator==(const HalfEdge& rhs) const {
  if (self->label == rhs.self->label) {
    LIBINTERVALXT_ASSERT(self->decomposition != rhs.self->decomposition || self->component == rhs.self->component, "half edge is not in the component for which it was created anymore");
    return self->component == rhs.self->component && self->contour == rhs.self->contour;
  }
  return false;
}

HalfEdge::operator Label() const {
  return self->label;
}

std::list<Side> HalfEdge::cross() const {
  std::list<Side> connections;
  if (top()) {
    for (auto connection : right())
      connections.emplace_back(std::move(connection));
    connections.emplace_back(*this);
    auto pos = end(connections);
    for (auto connection : left())
      pos = connections.insert(pos, std::move(connection));
  } else {
    for (auto connection : left())
      connections.emplace_front(std::move(connection));
    connections.push_back(*this);
    for (auto connection : right())
      connections.emplace_back(std::move(connection));
  }
  return connections;
}

std::list<Connection> HalfEdge::left() const {
  const auto makeConnection = [&](const auto& connectionState) { return ImplementationOf<Connection>::make(self->decomposition, connectionState); };
  std::list<Connection> left;
  for (auto& connection : top() ? self->connections(*this).topLeft : self->connections(*this).bottomLeft)
    left.emplace_back(makeConnection(connection));
  return left;
}

std::list<Connection> HalfEdge::right() const {
  const auto makeConnection = [&](const auto& connectionState) { return ImplementationOf<Connection>::make(self->decomposition, connectionState); };
  std::list<Connection> right;
  for (auto& connection : top() ? self->connections(*this).topRight : self->connections(*this).bottomRight)
    right.emplace_back(makeConnection(connection));
  return right;
}

std::optional<HalfEdge> HalfEdge::next() const {
  return ::intervalxt::ImplementationOf<Component>::next(self->component, *this, self->decomposition);
}

std::optional<HalfEdge> HalfEdge::previous() const {
  return ::intervalxt::ImplementationOf<Component>::previous(self->component, *this, self->decomposition);
}

Component HalfEdge::component() const {
  Component component = ImplementationOf<Component>::make(self->decomposition, self->component);
  LIBINTERVALXT_ASSERT([&]() {
    if (top()) {
      auto top = component.topContour();
      return std::find(begin(top), end(top), *this) != end(top);
    } else {
      auto bottom = component.bottomContour();
      return std::find(begin(bottom), end(bottom), *this) != end(bottom);
    }
  }(),
      "half edge " << *this << " is not in the component " << component << " for which it was created anymore");
  return component;
}

HalfEdge HalfEdge::operator-() const {
  using Contour = ImplementationOf<HalfEdge>::Contour;
  HalfEdge ret = *this;
  ret.self->contour = ret.self->contour == Contour::BOTTOM ? Contour::TOP : Contour::BOTTOM;
  return ret;
}

std::optional<Separatrix> HalfEdge::separatrix() const {
  if (not next().has_value())
    return {};

  return ImplementationOf<Separatrix>::make(self->decomposition, top()
                                                                     ? ::intervalxt::ImplementationOf<Separatrix>::makeAtTop(self->decomposition, self->label)
                                                                     : ::intervalxt::ImplementationOf<Separatrix>::makeAtBottom(self->decomposition, self->label));
}

ImplementationOf<HalfEdge>::ImplementationOf(const DynamicalDecomposition& decomposition, DecompositionState::Component* component, Label label, Contour contour) :
  ImplementationOfDecomposition(decomposition),
  component(component),
  label(label),
  contour(contour) {}

HalfEdge ImplementationOf<HalfEdge>::make(const DynamicalDecomposition& decomposition, DecompositionState::Component* component, Label label, Contour contour) {
  return HalfEdge(PrivateConstructor{}, decomposition, component, label, contour);
}

std::ostream& operator<<(std::ostream& os, const HalfEdge& self) {
  if (self.top())
    os << "-";
  return os << "[" << self.self->render(self.self->label) << "]";
}

}  // namespace intervalxt

namespace std {

size_t hash<intervalxt::HalfEdge>::operator()(const intervalxt::HalfEdge& self) const {
  return hash<intervalxt::Label>()(self);
}

}  // namespace std
