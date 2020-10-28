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

#include "../intervalxt/component.hpp"

#include <fmt/format.h>

#include <boost/logic/tribool.hpp>
#include <ostream>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../intervalxt/decomposition_step.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/induction_step.hpp"
#include "../intervalxt/label.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/component.impl.hpp"
#include "impl/connection.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/half_edge.impl.hpp"
#include "impl/implementation_of_decomposition.hpp"
#include "impl/separatrix.impl.hpp"
#include "util/assert.ipp"

namespace intervalxt {

std::vector<Side> Component::perimeter() const {
  auto perimeter = rx::chain(bottom(), right(), top(), left()) | rx::to_vector();

  ASSERT(std::unordered_set<Side>(begin(perimeter), end(perimeter)).size() == perimeter.size(), "Perimeter must not contain duplicates.");

  return perimeter;
}

std::vector<Side> Component::left() const {
  const auto& iet = self->component->iet;
  const bool swapped = iet.swapped();
  return rx::chain(
             self->connections(*begin(swapped ? iet.top() : iet.bottom())).bottomLeft,
             self->connections(*begin(swapped ? iet.bottom() : iet.top())).topLeft) |
         rx::transform([&](const auto& connection) { return Side(ImplementationOf<Connection>::make(self->decomposition, connection)); }) | rx::reverse() | rx::to_vector();
}

std::vector<Side> Component::right() const {
  const auto& iet = self->component->iet;
  const bool swapped = iet.swapped();
  return rx::chain(
             self->connections(*rbegin(swapped ? iet.top() : iet.bottom())).bottomRight,
             self->connections(*rbegin(swapped ? iet.bottom() : iet.top())).topRight) |
         rx::transform([&](const auto& connection) { return Side(ImplementationOf<Connection>::make(self->decomposition, connection)); }) | rx::to_vector();
}

std::vector<Side> Component::bottom() const {
  return ImplementationOf<Component>::horizontal(*this, false);
}

std::vector<Side> Component::top() const {
  return ImplementationOf<Component>::horizontal(*this, true);
}

boost::logic::tribool Component::cylinder() const {
  return self->component->cylinder;
}

boost::logic::tribool Component::withoutPeriodicTrajectory() const {
  return self->component->withoutPeriodicTrajectory;
}

boost::logic::tribool Component::keane() const {
  return self->component->keane;
}

std::vector<HalfEdge> Component::topContour() const {
  return (self->component->iet.swapped() ? self->component->iet.bottom() : self->component->iet.top()) | rx::transform([&](const auto& label) { return topContour(label); }) | rx::to_vector();
}

std::vector<HalfEdge> Component::bottomContour() const {
  return (self->component->iet.swapped() ? self->component->iet.top() : self->component->iet.bottom()) | rx::transform([&](const auto& label) { return bottomContour(label); }) | rx::to_vector();
}

HalfEdge Component::topContour(Label label) const {
  return ImplementationOf<HalfEdge>::make(self->decomposition, *this, label, ImplementationOf<HalfEdge>::Contour::TOP);
}

HalfEdge Component::bottomContour(Label label) const {
  return ImplementationOf<HalfEdge>::make(self->decomposition, *this, label, ImplementationOf<HalfEdge>::Contour::BOTTOM);
}

bool Component::operator==(const Component& rhs) const {
  return &self->impl() == &rhs.self->impl() && self->component == rhs.self->component;
}

std::pair<std::list<Connection>, std::list<Connection>> Component::inject(const HalfEdge& at, const std::vector<std::pair<Label, Label>>& left_, const std::vector<std::pair<Label, Label>>& right_) {
  using Orientation = DecompositionState::Separatrix::Orientation;

  ImplementationOf<DynamicalDecomposition>::check(self->decomposition);

  const bool top = at.top();

  auto& topLeftConnections = top ? self->connections(at).topLeft : self->connections(at).bottomLeft;
  auto& topRightConnections = top ? self->connections(at).topRight : self->connections(at).bottomRight;

  std::list<Connection> leftInjected, rightInjected;

  // Inject left connections.
  {
    auto left = left_;

    if (top) std::reverse(begin(left), end(left));

    ASSERT(left.empty() || topLeftConnections.empty(), "cannot inject into a component with existing connections");

    for (const auto& [source, target] : left) {
      topLeftConnections.push_back({
          {source, Orientation::ANTIPARALLEL},
          {target, Orientation::PARALLEL},
      });
      leftInjected.push_back(ImplementationOf<Connection>::make(self->decomposition, topLeftConnections.back()));
    }

    if (top) std::reverse(begin(leftInjected), end(leftInjected));
  }

  // Inject right connections.
  {
    auto right = right_;

    if (top) std::reverse(begin(right), end(right));

    ASSERT(right.empty() || topRightConnections.empty(), "cannot inject into a component with existing connections");

    for (const auto& [source, target] : right) {
      topRightConnections.push_back({{source, Orientation::PARALLEL},
          {target, Orientation::ANTIPARALLEL}});
      rightInjected.push_back(ImplementationOf<Connection>::make(self->decomposition, topRightConnections.back()));
    }

    if (top) std::reverse(begin(rightInjected), end(rightInjected));
  }

  ImplementationOf<DynamicalDecomposition>::check(self->decomposition);

  return {leftInjected, rightInjected};
}

DecompositionStep Component::decompositionStep(int limit) {
  ASSERT(!cylinder() || !keane(), "Component " << *this << " is already fully decomposed.");

  auto& component = *self->component;

  const std::optional<int> boshernitzanCost = ImplementationOf<Component>::boshernitzanCost(component.iet);

  if (limit == -1) {
    limit = 0;
    while (true) {
      auto step = decompositionStep(limit);

      if (step.result != DecompositionStep::Result::LIMIT_REACHED)
        return step;

      limit = boshernitzanCost.value_or(2 * (limit + 1));
    }
  }

  InductionStep step;

  do {
    int zorichInductionSteps;
    if (boshernitzanCost) {
      if (limit < 2 * *boshernitzanCost) {
        zorichInductionSteps = limit;
      } else {
        zorichInductionSteps = *boshernitzanCost;
      }
    } else {
      zorichInductionSteps = limit;
    }

    limit -= zorichInductionSteps;

    step = component.iet.induce(zorichInductionSteps);
  } while (step.result == InductionStep::Result::LIMIT_REACHED && limit != 0);

  switch (step.result) {
    case InductionStep::Result::LIMIT_REACHED:
      return {DecompositionStep::Result::LIMIT_REACHED};
    case InductionStep::Result::CYLINDER:
      component.cylinder = true;
      component.withoutPeriodicTrajectory = false;
      component.keane = false;
      return DecompositionStep{
          DecompositionStep::Result::CYLINDER,
      };
    case InductionStep::Result::SEPARATING_CONNECTION: {
      auto [b, t] = *step.connection;

      auto equivalent = self->walkClockwise(-ImplementationOf<HalfEdge>::make(self->decomposition, *this, *rbegin(bottomContour()), ImplementationOf<HalfEdge>::Contour::TOP), ImplementationOf<HalfEdge>::make(self->decomposition, *this, *rbegin(topContour()), ImplementationOf<HalfEdge>::Contour::TOP));

      auto connection = DecompositionState::Connection{
          ImplementationOf<Separatrix>::makeAtBottom(self->decomposition, b),
          ImplementationOf<Separatrix>::makeAtTop(self->decomposition, t)};
      auto right = ImplementationOf<DynamicalDecomposition>::insertComponent(self->decomposition, *this, connection, std::move(*step.additionalIntervalExchangeTransformation));
      return {
          DecompositionStep::Result::SEPARATING_CONNECTION,
          ImplementationOf<Connection>::make(self->decomposition, connection),
          equivalent,
          right};
    }
    case InductionStep::Result::NON_SEPARATING_CONNECTION: {
      // We found a non-separating connection b ⚯ t.
      auto [b, t] = *step.connection;
      ASSERT(b != t, "Mistook cylinder for a non-separating connection");

      // t is not valid anymore but we can still use cross() on it.
      auto equivalent = ImplementationOf<HalfEdge>::make(self->decomposition, *this, t, ImplementationOf<HalfEdge>::Contour::TOP).cross();
      equivalent.splice(end(equivalent), (-ImplementationOf<HalfEdge>::make(self->decomposition, *this, b, ImplementationOf<HalfEdge>::Contour::TOP)).cross());
      std::reverse(begin(equivalent), end(equivalent));
      for (auto& side : equivalent) {
        if (auto connection = std::get_if<intervalxt::Connection>(&side)) {
          side = -*connection;
        } else {
          // This should probably change with #68.
          ;
        }
      }

      auto connection = DecompositionState::Connection{
          ImplementationOf<Separatrix>::makeAtBottom(self->decomposition, b),
          ImplementationOf<Separatrix>::makeAtTop(self->decomposition, t)};

      ASSERT(*begin(component.iet.top()) != t, "Label t has been eliminated already.");

      // Register the new connection right of b.
      self->connections(b).bottomRight.push_back(connection);
      self->connections(*begin(component.iet.top())).topLeft.push_front({
          connection.target,
          connection.source,
      });

      // The label t has been eliminated by moving the initial section to
      // where t is in the bottom contour. Before we perform this move of
      // connections, make sure no connections are associated to t anymore.
      self->connections(b).bottomLeft.splice(end(self->connections(b).bottomLeft), self->connections(t).topLeft);
      self->connections(b).bottomRight.splice(end(self->connections(b).bottomRight), self->connections(t).topRight);

      // Now we move the initial section under t.
      self->connections(b).bottomLeft.splice(end(self->connections(b).bottomLeft), self->connections(t).bottomLeft);
      self->connections(b).bottomRight.splice(end(self->connections(b).bottomRight), self->connections(t).bottomRight);

      ASSERT(not left().empty(), "we just added a left boundary so it cannot be empty");

      return {
          DecompositionStep::Result::NON_SEPARATING_CONNECTION,
          ImplementationOf<Connection>::make(self->decomposition, connection),
          equivalent};
    }
    case InductionStep::Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN:
      component.withoutPeriodicTrajectory = true;
      component.cylinder = false;
      return {DecompositionStep::Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN};
    case InductionStep::Result::WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR:
      component.withoutPeriodicTrajectory = true;
      component.cylinder = false;
      return {DecompositionStep::Result::WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR};
    default:
      throw std::logic_error("not implemented: unknown enum value");
  }
}

ImplementationOf<Component>::ImplementationOf(const DynamicalDecomposition& decomposition, DecompositionState::Component* component) :
  ImplementationOfDecomposition(decomposition),
  component(component) {}

std::vector<Side> ImplementationOf<Component>::horizontal(const Component& component, bool top) {
  std::vector<Side> contour;

  const auto add = [&](const auto& connection) {
    assert(not contour.empty());
    contour.push_back(ImplementationOf<Connection>::make(component.self->decomposition, connection));
  };

  auto halfEdges = top ? component.topContour() : component.bottomContour();

  for (auto edge = begin(halfEdges); edge != end(halfEdges); edge++) {
    if (edge != begin(halfEdges)) {
      if (top)
        component.self->connections(*edge).topLeft | rx::for_each(add);
      else
        component.self->connections(*edge).bottomLeft | rx::reverse() | rx::for_each(add);
    }

    contour.push_back(*edge);

    if (edge != --end(halfEdges)) {
      if (top)
        component.self->connections(*edge).topRight | rx::reverse() | rx::for_each(add);
      else
        component.self->connections(*edge).bottomRight | rx::for_each(add);
    }
  }

  if (top) std::reverse(contour.begin(), contour.end());

  return contour;
}

Component ImplementationOf<Component>::make(const DynamicalDecomposition& decomposition, DecompositionState::Component* component) {
  return Component(PrivateConstructor{}, decomposition, component);
}

void ImplementationOf<Component>::registerSeparating(Component& left, DecompositionState::Connection separating, Component& right) {
  auto& decomposition = left.self->decomposition;

  const auto connection = ImplementationOf<Connection>::make(decomposition, separating);
  ASSERT(connection.parallel(), "separating connection must by convention be from bottom to top");
  ASSERT(left != right, "separating connection must separate distinct components");

  left.self->connections(*rbegin(left.self->component->iet.bottom())).bottomRight.push_back(separating);
  left.self->connections(*begin(right.self->component->iet.bottom())).bottomLeft.push_back({separating.target, separating.source});

  ASSERT(not left.right().empty(), "we just added a right boundary so it cannot be empty");
  ASSERT(not right.left().empty(), "we just added a left boundary so it cannot be empty");
}

std::optional<int> ImplementationOf<Component>::boshernitzanCost(const IntervalExchangeTransformation& iet) {
  {
    auto saf = iet.safInvariant();
    if (std::none_of(begin(saf), end(saf), [](const auto& v) { return v; })) {
      // When SAF = 0, the Boshernitzan criterion is not useful.
      // https://github.com/flatsurf/intervalxt/issues/86
      return {};
    }
  }

  // Not implemented: see #60.
  return 64;
}

// It would be easier to always walk counterclockwise as that is compatible with the order in cross(), see #69.
std::list<Side> ImplementationOf<Component>::walkClockwise(HalfEdge from, HalfEdge to) const {
  ASSERT(from.component() == to.component(), "Cannot walk between components");

  const auto& component = from.component();

  std::list<Side> connections;

  if (from.bottom() && to.top()) {
    connections.splice(end(connections), walkClockwise(from, *begin(component.bottomContour())));
    connections.splice(end(connections), walkClockwise(*begin(component.topContour()), to));
  } else if (from.top() && to.bottom()) {
    connections.splice(end(connections), walkClockwise(from, *rbegin(component.topContour())));
    connections.splice(end(connections), walkClockwise(*rbegin(component.bottomContour()), to));
  } else if (from.top() && to.top()) {
    while (true) {
      auto cross = from.cross();

      std::reverse(begin(cross), end(cross));

      for (auto& side : cross)
        if (auto connection = std::get_if<intervalxt::Connection>(&side)) {
          side = -*connection;
        } else {
          // This should probably change with #68.
          ;
        }

      connections.splice(end(connections), cross);

      if (from == to) {
        break;
      } else if (!from.next()) {
        connections.splice(end(connections), walkClockwise(*rbegin(component.bottomContour()), to));
        break;
      } else {
        from = *from.next();
      }
    }
  } else if (from.bottom() && to.bottom()) {
    while (true) {
      auto cross = from.cross();
      std::reverse(begin(cross), end(cross));

      for (auto& side : cross)
        if (auto connection = std::get_if<intervalxt::Connection>(&side)) {
          side = -*connection;
        } else {
          // This should probably change with #68.
          ;
        }

      connections.splice(end(connections), cross);

      if (from == to) {
        break;
      } else if (!from.previous()) {
        connections.splice(end(connections), walkClockwise(*begin(component.topContour()), to));
        break;
      } else {
        from = *from.previous();
      }
    }
  }

  return connections;
}

std::optional<HalfEdge> ImplementationOf<Component>::next(const Component& self, const HalfEdge& edge) {
  auto contour = edge.top() ? self.self->component->iet.top() : self.self->component->iet.bottom();
  auto pos = std::find(begin(contour), end(contour), static_cast<Label>(edge));
  ASSERT(pos != end(contour), "half edge " << edge << " not in component " << self)
  if (++pos == end(contour)) return {};
  HalfEdge next = ImplementationOf<HalfEdge>::make(self.self->decomposition, self, *pos, ImplementationOf<HalfEdge>::Contour::TOP);
  return edge.top() ? next : -next;
}

std::optional<HalfEdge> ImplementationOf<Component>::previous(const Component& self, const HalfEdge& edge) {
  auto contour = edge.top() ? self.self->component->iet.top() : self.self->component->iet.bottom();
  auto pos = std::find(begin(contour), end(contour), static_cast<Label>(edge));
  ASSERT(pos != end(contour), "half edge " << edge << " not in component " << self)
  if (pos == begin(contour)) return {};
  HalfEdge previous = ImplementationOf<HalfEdge>::make(self.self->decomposition, self, *--pos, ImplementationOf<HalfEdge>::Contour::TOP);
  return edge.top() ? previous : -previous;
}

bool Component::decompose(std::function<bool(const Component&)> target, int limit) {
  bool limitReached = false;
  while (!target(*this)) {
    auto step = decompositionStep(limit);
    if (step.result == DecompositionStep::Result::LIMIT_REACHED)
      return false;
    if (step.additionalComponent) {
      limitReached = limitReached || !step.additionalComponent->decompose(target, limit);
    }
  }
  return not limitReached;
}

IntervalExchangeTransformation Component::iet() const {
  auto& iet = self->component->iet;
  return IntervalExchangeTransformation(std::make_shared<Lengths>(self->component->iet.lengths()->forget()), iet.top(), iet.bottom());
}

std::ostream& operator<<(std::ostream& os, const Component& self) {
  return os << fmt::format("{}", fmt::join(self.perimeter(), " "));
}

std::ostream& operator<<(std::ostream& os, const Side& self) {
  if (auto edge = std::get_if<HalfEdge>(&self))
    return os << *edge;
  else if (auto connection = std::get_if<Connection>(&self))
    return os << *connection;
  else
    throw std::logic_error("invalid connection");
}

}  // namespace intervalxt
