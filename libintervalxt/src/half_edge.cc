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
    ASSERT(self->decomposition != rhs.self->decomposition || self->component == rhs.self->component, "half edge is not in the component for which it was created anymore");
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
      connections.push_back(connection);
    connections.push_back(*this);
    auto pos = end(connections);
    for (auto connection : left())
      pos = connections.insert(pos, connection);
  } else {
    for (auto connection : left())
      connections.push_front(connection);
    connections.push_back(*this);
    for (auto connection : right())
      connections.push_back(connection);
  }
  return connections;
}

std::list<Connection> HalfEdge::left() const {
  const auto makeConnection = [&](const auto& connection) { return ImplementationOf<Connection>::make(self->decomposition, connection); };
  if (top())
    return self->connections(*this).topLeft | rx::transform(makeConnection) | rx::to_list();
  else
    return self->connections(*this).bottomLeft | rx::transform(makeConnection) | rx::to_list();
}

std::list<Connection> HalfEdge::right() const {
  const auto makeConnection = [&](const auto& connection) { return ImplementationOf<Connection>::make(self->decomposition, connection); };
  if (top())
    return self->connections(*this).topRight | rx::transform(makeConnection) | rx::to_list();
  else
    return self->connections(*this).bottomRight | rx::transform(makeConnection) | rx::to_list();
}

std::optional<HalfEdge> HalfEdge::next() const {
  return ::intervalxt::ImplementationOf<Component>::next(self->component, *this);
}

std::optional<HalfEdge> HalfEdge::previous() const {
  return ::intervalxt::ImplementationOf<Component>::previous(self->component, *this);
}

Component HalfEdge::component() const {
  ASSERT([&]() {
    if (top()) {
      auto top = self->component.topContour();
      return std::find(begin(top), end(top), *this) != end(top);
    } else {
      auto bottom = self->component.bottomContour();
      return std::find(begin(bottom), end(bottom), *this) != end(bottom);
    }
  }(),
      "half edge " << *this << " is not in the component " << self->component << " for which it was created anymore");
  return self->component;
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

ImplementationOf<HalfEdge>::ImplementationOf(const DynamicalDecomposition& decomposition, const Component& component, Label label, Contour contour) :
  ImplementationOfDecomposition(decomposition),
  component(component),
  label(label),
  contour(contour) {}

HalfEdge ImplementationOf<HalfEdge>::make(const DynamicalDecomposition& decomposition, const Component& component, Label label, Contour contour) {
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
