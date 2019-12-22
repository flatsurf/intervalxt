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

#include <vector>
#include <ostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <range/v3/view/transform.hpp>

#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/component.hpp"
#include "../intervalxt/label.hpp"

#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/component.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"

#include "util/assert.ipp"

namespace intervalxt {

using ranges::view::transform;
using ranges::to;
using boost::lexical_cast;
using std::string;

DynamicalDecomposition::DynamicalDecomposition(const IntervalExchangeTransformation& iet) :
  impl(spimpl::make_unique_impl<Implementation>(iet)) {}

bool DynamicalDecomposition::decompose(std::function<bool(const Component&)> target, int limit) {
  auto components = this->components();
  return std::all_of(components.begin(), components.end(), [&](auto& component) {
    return component.decompose(target, limit); });
}

std::vector<Component>  DynamicalDecomposition::components() const {
  return impl->decomposition->components | transform([&](auto& component) {
    return ::intervalxt::Implementation<Component>::make(impl->decomposition, &component);
  }) | to<std::vector<Component>>();
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

std::string Implementation<DynamicalDecomposition>::render(std::shared_ptr<DecompositionState> decomposition, Label label) {
  return Implementation<IntervalExchangeTransformation>::render(begin(decomposition->components)->iet, label);
}

std::ostream& operator<<(std::ostream& os, const DynamicalDecomposition& self) {
  auto components = self.components();
  return os << "DynamicalDecomposition(" << boost::algorithm::join(
    components | transform([](const auto& component) { return lexical_cast<string>(component); }), ", ");
}

}
