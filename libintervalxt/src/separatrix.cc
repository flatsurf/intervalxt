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

#include "impl/separatrix.impl.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/decomposition_state.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "util/assert.ipp"

namespace intervalxt {

using std::ostream;
using Orientation = Implementation<Separatrix>::Orientation;

Separatrix::Separatrix() :
  // We assume that the caller sets impl explicitly
  impl(nullptr) {}

bool Separatrix::parallel() const noexcept {
  return impl->orientation == Implementation::Orientation::PARALLEL;
}

bool Separatrix::antiparallel() const noexcept {
  return not parallel();
}

bool Separatrix::operator==(const Separatrix& rhs) const {
  return impl->label == rhs.impl->label
    && impl->orientation == rhs.impl->orientation
    && impl->decomposition == rhs.impl->decomposition;
}

Implementation<Separatrix>::Implementation(std::shared_ptr<DecompositionState> decomposition, Label label, Orientation orientation) :
  decomposition(decomposition),
  label(label),
  orientation(orientation) {}

Separatrix Implementation<Separatrix>::make(std::shared_ptr<DecompositionState> decomposition, Label label, Orientation orientation) {
  Separatrix separatrix;
  separatrix.impl = spimpl::make_impl<Implementation>(decomposition, label, orientation);
  return separatrix;
}

Separatrix Implementation<Separatrix>::atTop(std::shared_ptr<DecompositionState> decomposition, Label label) {
  Separatrix separatrix = ::intervalxt::Implementation<Separatrix>::make(decomposition, label, Orientation::ANTIPARALLEL); 
  decomposition->top.at(label).right | rx::reverse() | rx::for_each([&](const auto& connection) {
    ASSERT(separatrix == connection.target(), "connection separatrices do not form a continuous chain");
    separatrix = connection.source();
    separatrix = ::intervalxt::Implementation<Separatrix>::make(decomposition, separatrix.impl->label, Orientation::ANTIPARALLEL);
  });

  return separatrix;
}

Separatrix Implementation<Separatrix>::atBottom(std::shared_ptr<DecompositionState> decomposition, Label label) {
  Separatrix separatrix = ::intervalxt::Implementation<Separatrix>::make(decomposition, label, Orientation::PARALLEL); 
  decomposition->bottom.at(label).right | rx::for_each([&](const auto& connection) {
    ASSERT(separatrix == connection.source(), "connection separatrices do not form a continuous chain");
    separatrix = connection.target();
    separatrix = ::intervalxt::Implementation<Separatrix>::make(decomposition, separatrix.impl->label, Orientation::PARALLEL);
  });

  return separatrix;
}

ostream& operator<<(ostream& os, const Separatrix& self) {
  return os << Implementation<DynamicalDecomposition>::render(self.impl->decomposition, self.impl->label) << (self.impl->orientation == Orientation::PARALLEL ? "+" : "-");
}

}

namespace std {

using namespace intervalxt;

size_t hash<Separatrix>::operator()(const Separatrix& self) const noexcept {
  size_t hash = std::hash<Label>()(self.impl->label);
  return self.parallel() ? hash : -hash;
}

}
