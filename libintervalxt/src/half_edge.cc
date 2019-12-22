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

#include "../intervalxt/half_edge.hpp"
#include "../intervalxt/component.hpp"
#include "../intervalxt/separatrix.hpp"

#include "impl/half_edge.impl.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/separatrix.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/component.impl.hpp"

#include "util/assert.ipp"

namespace intervalxt {

using std::ostream;
using Contour = Implementation<HalfEdge>::Contour;

HalfEdge::HalfEdge() :
  // We assume that the caller is aware that impl needs to be set manually.
  impl(nullptr) {}

Component HalfEdge::component() const {
  Implementation::check(*this);
  return impl->component;
}

bool HalfEdge::top() const noexcept {
  return impl->contour == Contour::TOP;
}

bool HalfEdge::bottom() const noexcept {
  return not top();
}

HalfEdge HalfEdge::operator-() const noexcept {
  Implementation::check(*this);
  return Implementation::make(impl->decomposition, impl->component, impl->label, top() ? Contour::BOTTOM : Contour::TOP);  
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

void Implementation<HalfEdge>::check(const HalfEdge& self) {
  ASSERT([&]() {
    if (self.top()) {
      auto top = self.impl->component.topContour();
      return std::find(begin(top), end(top), self) != end(top);
    } else {
      auto bottom = self.impl->component.bottomContour();
      return std::find(begin(bottom), end(bottom), self) != end(bottom);
    }
  }(), "half edge " << self << " is not in the component for which it was created anymore");
}

HalfEdge Implementation<HalfEdge>::make(std::shared_ptr<DecompositionState> decomposition, const Component& component, Label label, Contour contour) {
  HalfEdge edge;
  edge.impl = spimpl::make_impl<Implementation>(decomposition, component, label, contour);
  return edge;
}

Implementation<HalfEdge>::Implementation(std::shared_ptr<DecompositionState> decomposition, const Component& component, Label label, Contour contour) :
  decomposition(decomposition),
  component(component),
  label(label),
  contour(contour) {}

ostream& operator<<(ostream& os, const HalfEdge& self) {
  if (self.impl->contour == Contour::TOP)
    os << "-";
  return os << "[" << Implementation<DynamicalDecomposition>::render(self.impl->decomposition, self.impl->label) << "]";
}

}
