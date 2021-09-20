/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
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

#include <ostream>
// Work around https://github.com/simonask/rx-ranges/pull/48
#include <limits>

#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/implementation_of_decomposition.hpp"
#include "impl/separatrix.impl.hpp"
#include "util/assert.ipp"

namespace intervalxt {

bool Separatrix::parallel() const {
  return self->separatrix.orientation == DecompositionState::Separatrix::Orientation::PARALLEL;
}

bool Separatrix::antiparallel() const {
  return not parallel();
}

bool Separatrix::operator==(const Separatrix& rhs) const {
  return self->separatrix.label == rhs.self->separatrix.label && self->separatrix.orientation == rhs.self->separatrix.orientation;
}

ImplementationOf<Separatrix>::ImplementationOf(const DynamicalDecomposition& decomposition, DecompositionState::Separatrix separatrix) :
  ImplementationOfDecomposition(decomposition),
  separatrix(std::move(separatrix)) {}

Separatrix ImplementationOf<Separatrix>::make(const DynamicalDecomposition& decomposition, DecompositionState::Separatrix separatrix) {
  return Separatrix(PrivateConstructor{}, decomposition, std::move(separatrix));
}

std::ostream& operator<<(std::ostream& os, const Separatrix& self) {
  return os << self.self->render(self.self->separatrix.label) << (self.parallel() ? "+" : "-");
}

DecompositionState::Separatrix ImplementationOf<Separatrix>::makeAtTop(const DynamicalDecomposition& decomposition, Label label) {
  DecompositionState::Separatrix separatrix = {label, DecompositionState::Separatrix::Orientation::ANTIPARALLEL};
  ImplementationOf<DynamicalDecomposition>::self(decomposition).decomposition.connections.at(label).topRight | rx::reverse() | rx::for_each([&](const auto& connection) {
    LIBINTERVALXT_ASSERT(make(decomposition, separatrix) == make(decomposition, connection.target), "connection separatrices do not form a continuous chain");
    separatrix.label = connection.source.label;
  });

  return separatrix;
}

DecompositionState::Separatrix ImplementationOf<Separatrix>::makeAtBottom(const DynamicalDecomposition& decomposition, Label label) {
  DecompositionState::Separatrix separatrix = {label, DecompositionState::Separatrix::Orientation::PARALLEL};
  ImplementationOf<DynamicalDecomposition>::self(decomposition).decomposition.connections.at(label).bottomRight | rx::for_each([&](const auto& connection) {
    LIBINTERVALXT_ASSERT(make(decomposition, separatrix) == make(decomposition, connection.source), "connection separatrices do not form a continuous chain");
    separatrix.label = connection.target.label;
  });

  return separatrix;
}

}  // namespace intervalxt

namespace std {

size_t hash<intervalxt::Separatrix>::operator()(const intervalxt::Separatrix& self) const {
  size_t hash = std::hash<intervalxt::Label>()(self.self->separatrix.label);
  return self.parallel() ? hash : -hash;
}

}  // namespace std
