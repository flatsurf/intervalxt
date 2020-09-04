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

#include "../intervalxt/dynamical_decomposition.hpp"

#include <fmt/format.h>

#include <ostream>
#include <vector>

#include "../intervalxt/component.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/component.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "util/assert.ipp"

namespace intervalxt {

DynamicalDecomposition::DynamicalDecomposition(const IntervalExchangeTransformation& iet) :
  impl(spimpl::make_unique_impl<Implementation>(iet)) {}

bool DynamicalDecomposition::decompose(std::function<bool(const Component&)> target, int limit) {
  auto components = this->components();
  return std::all_of(components.begin(), components.end(), [&](auto& component) { return component.decompose(target, limit); });
}

std::vector<Component> DynamicalDecomposition::components() const {
  return impl->decomposition->components | rx::transform([&](auto& component) {
    return ::intervalxt::Implementation<Component>::make(impl->decomposition, &const_cast<ComponentState&>(component));
  }) |
         rx::to_vector();
}

Implementation<DynamicalDecomposition>::Implementation(const IntervalExchangeTransformation& iet) :
  decomposition(std::make_shared<DecompositionState>()) {
  createComponent(decomposition, IntervalExchangeTransformation(
                                     ::intervalxt::Implementation<IntervalExchangeTransformation>::withLengths(iet,
                                         [&](std::shared_ptr<Lengths> original) {
                                           return std::make_shared<Lengths>(LengthsWithConnections(original, decomposition));
                                         })));

  for (auto label : iet.top())
    decomposition->top[label] = {};

  for (auto label : iet.bottom())
    decomposition->bottom[label] = {};
}

Component Implementation<DynamicalDecomposition>::createComponent(std::shared_ptr<DecompositionState> decomposition, IntervalExchangeTransformation&& iet) {
  auto& state = decomposition->components.emplace_back(std::move(iet));
  return Implementation<Component>::make(decomposition, &state);
}

Component Implementation<DynamicalDecomposition>::createComponent(std::shared_ptr<DecompositionState> decomposition, Component& left, const Connection& connection, IntervalExchangeTransformation&& iet) {
  ASSERT(connection.parallel(), "separating connection must by convention go from bottom to top");
  auto right = createComponent(decomposition, std::move(iet));
  Implementation<Component>::registerSeparating(left, connection, right);
  return right;
}

std::ostream& operator<<(std::ostream& os, const DynamicalDecomposition& self) {
  return os << fmt::format("DynamicalDecomposition({})", fmt::join(self.components(), ", "));
}

}  // namespace intervalxt
