/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Vincent Delecroix
 *        Copyright (C) 2020 Julian Rüth
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

#include "impl/implementation_of_decomposition.hpp"

#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"

namespace intervalxt {

ImplementationOfDecomposition::ImplementationOfDecomposition(const intervalxt::DynamicalDecomposition& decomposition) :
  decomposition(decomposition) {}

std::deque<DecompositionState::Component>& ImplementationOfDecomposition::components() {
  return state().components;
}

const std::deque<DecompositionState::Component>& ImplementationOfDecomposition::components() const {
  return state().components;
}

DecompositionState::Connections& ImplementationOfDecomposition::connections(const Label& at) {
  return state().connections.at(at);
}

const DecompositionState::Connections& ImplementationOfDecomposition::connections(const Label& at) const {
  return state().connections.at(at);
}

ImplementationOf<DynamicalDecomposition>& ImplementationOfDecomposition::impl() {
  return ImplementationOf<DynamicalDecomposition>::self(decomposition);
}

const ImplementationOf<DynamicalDecomposition>& ImplementationOfDecomposition::impl() const {
  return ImplementationOf<DynamicalDecomposition>::self(decomposition);
}

DecompositionState& ImplementationOfDecomposition::state() {
  return impl().decomposition;
}

const DecompositionState& ImplementationOfDecomposition::state() const {
  return impl().decomposition;
}

std::string ImplementationOfDecomposition::render(Label label) const {
  return ImplementationOf<IntervalExchangeTransformation>::render(begin(components())->iet, label);
}

}  // namespace intervalxt
