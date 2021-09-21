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

#include <deque>
#include <ostream>
#include <vector>
#include <unordered_map>

#include "../intervalxt/component.hpp"
#include "../intervalxt/connection.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/component.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "impl/lengths_with_connections.hpp"
#include "util/assert.ipp"

namespace intervalxt {

DynamicalDecomposition::DynamicalDecomposition(const IntervalExchangeTransformation& iet) :
  self(std::make_shared<ImplementationOf<DynamicalDecomposition>>()) {
  self->insertComponent(*this,
      IntervalExchangeTransformation(
          ImplementationOf<IntervalExchangeTransformation>::withLengths(iet,
              [&](std::shared_ptr<Lengths> original) -> std::shared_ptr<Lengths> {
                return std::make_shared<Lengths>(LengthsWithConnections(original, self));
              })));

  for (auto label : iet.top())
    self->decomposition.connections[label];
}

std::vector<Component> DynamicalDecomposition::components() const {
  std::vector<Component> components;
  for (auto& component : self->decomposition.components)
    components.push_back(ImplementationOf<Component>::make(*this, &component));
  return components;
}

bool DynamicalDecomposition::decompose(std::function<bool(const Component&)> target, int limit) {
  auto components = this->components();
  return std::all_of(components.begin(), components.end(), [&](auto& component) { return component.decompose(target, limit); });
}

bool DynamicalDecomposition::operator==(const DynamicalDecomposition& rhs) const {
  return self == rhs.self;
}

ImplementationOf<DynamicalDecomposition>::ImplementationOf() {}

Component ImplementationOf<DynamicalDecomposition>::insertComponent(DynamicalDecomposition& decomposition, IntervalExchangeTransformation&& iet) {
  auto& state = self(decomposition).decomposition.components.emplace_back(DecompositionState::Component{std::move(iet)});
  return ImplementationOf<Component>::make(decomposition, &state);
}

Component ImplementationOf<DynamicalDecomposition>::insertComponent(DynamicalDecomposition& decomposition, Component& left, DecompositionState::Connection connection, IntervalExchangeTransformation&& iet) {
  auto right = insertComponent(decomposition, std::move(iet));
  ImplementationOf<Component>::registerSeparating(left, connection, right);
  return right;
}

void ImplementationOf<DynamicalDecomposition>::check(const DynamicalDecomposition& decomposition) {
  std::unordered_map<Separatrix, std::vector<Connection>> connections;

  for (const auto& halfEdge : self(decomposition).decomposition.connections) {
    for (const auto& left : {halfEdge.second.topLeft, halfEdge.second.bottomLeft}) {
      for (const auto& data : left) {
        const auto connection = ImplementationOf<Connection>::make(decomposition, data);
        LIBINTERVALXT_ASSERT(connection.antiparallel(), "left connection must be antiparallel, i.e., going from top to bottom");
        connections[connection.source()].push_back(connection);
        connections[connection.target()].push_back(connection);
      }
    }
    for (const auto& right : {halfEdge.second.topRight, halfEdge.second.bottomRight}) {
      for (const auto& data : right) {
        const auto connection = ImplementationOf<Connection>::make(decomposition, data);
        LIBINTERVALXT_ASSERT(connection.parallel(), "right connection must be parallel, i.e., going from bottom to top");
        connections[connection.source()].push_back(connection);
        connections[connection.target()].push_back(connection);
      }
    }
  }

  for (const auto& [separatrix, atSeparatrix] : connections) {
    LIBINTERVALXT_ASSERT(atSeparatrix.size() <= 2, "Only a single connection may begin at separatrix " << separatrix);
    if (atSeparatrix.size() == 2) {
      LIBINTERVALXT_ASSERT(atSeparatrix[0] == -atSeparatrix[1], "Connections at separatrix " << separatrix << " do not match; found " << atSeparatrix[0] << " and " << atSeparatrix[1]);
    }
  }
}

ImplementationOf<DynamicalDecomposition>& ImplementationOf<DynamicalDecomposition>::self(DynamicalDecomposition& self) {
  return *self.self;
}

const ImplementationOf<DynamicalDecomposition>& ImplementationOf<DynamicalDecomposition>::self(const DynamicalDecomposition& self) {
  return *self.self;
}

std::ostream& operator<<(std::ostream& os, const DynamicalDecomposition& self) {
  return os << fmt::format("DynamicalDecomposition({})", fmt::join(self.components(), ", "));
}

}  // namespace intervalxt
