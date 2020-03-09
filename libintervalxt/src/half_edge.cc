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

#include <ostream>

#include "../intervalxt/component.hpp"
#include "../intervalxt/half_edge.hpp"
#include "../intervalxt/separatrix.hpp"

#include "impl/component.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/half_edge.impl.hpp"
#include "impl/separatrix.impl.hpp"

#include "util/assert.ipp"

namespace intervalxt {

using std::ostream;
using Contour = Implementation<HalfEdge>::Contour;

HalfEdge::HalfEdge(const Component& component, const Label& label) :
  impl(spimpl::make_impl<Implementation>(::intervalxt::Implementation<Component>::parent(component), component, label, Contour::TOP)) {
}

Component HalfEdge::component() const {
  ASSERT([&]() {
    if (top()) {
      auto top = impl->component.topContour();
      return std::find(begin(top), end(top), *this) != end(top);
    } else {
      auto bottom = impl->component.bottomContour();
      return std::find(begin(bottom), end(bottom), *this) != end(bottom);
    }
  }(),
      "half edge " << *this << " is not in the component " << impl->component << " for which it was created anymore");
  return impl->component;
}

bool HalfEdge::top() const noexcept {
  return impl->contour == Contour::TOP;
}

bool HalfEdge::bottom() const noexcept {
  return not top();
}

HalfEdge HalfEdge::operator-() const noexcept {
  HalfEdge ret = *this;
  ret.impl->contour = ret.impl->contour == Contour::BOTTOM ? Contour::TOP : Contour::BOTTOM;
  return ret;
}

std::optional<Separatrix> HalfEdge::separatrix() const {
  if (not next().has_value())
    return {};

  return top()
             ? ::intervalxt::Implementation<Separatrix>::atTop(impl->decomposition, impl->label)
             : ::intervalxt::Implementation<Separatrix>::atBottom(impl->decomposition, impl->label);
}

std::optional<HalfEdge> HalfEdge::next() const {
  return ::intervalxt::Implementation<Component>::next(impl->component, *this);
}

std::optional<HalfEdge> HalfEdge::previous() const {
  return ::intervalxt::Implementation<Component>::previous(impl->component, *this);
}

HalfEdge::operator Label() const noexcept {
  return impl->label;
}

bool HalfEdge::operator==(const HalfEdge& rhs) const {
  if (impl->label == rhs.impl->label) {
    ASSERT(impl->decomposition != rhs.impl->decomposition || impl->component == rhs.impl->component, "half edge is not in the component for which it was created anymore");
    return impl->component == rhs.impl->component && impl->contour == rhs.impl->contour;
  }
  return false;
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
  if (top())
    return impl->decomposition->top.at(*this).left;
  else
    return impl->decomposition->bottom.at(*this).left;
}

std::list<Connection> HalfEdge::right() const {
  if (top())
    return impl->decomposition->top.at(*this).right;
  else
    return impl->decomposition->bottom.at(*this).right;
}

Implementation<HalfEdge>::Implementation(std::shared_ptr<DecompositionState> decomposition, const Component& component, Label label, Contour contour) :
  decomposition(decomposition),
  component(component),
  label(label),
  contour(contour) {}

ostream& operator<<(ostream& os, const HalfEdge& self) {
  if (self.impl->contour == Contour::TOP)
    os << "-";
  return os << "[" << self.impl->decomposition->render(self.impl->label) << "]";
}

}  // namespace intervalxt

namespace std {

using namespace intervalxt;

size_t hash<HalfEdge>::operator()(const HalfEdge& self) const noexcept {
  return hash<Label>()(self);
}

}  // namespace std
