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
#include <unordered_set>
#include <variant>
#include <vector>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/logic/tribool.hpp>

#include "../intervalxt/component.hpp"
#include "../intervalxt/decomposition_step.hpp"
#include "../intervalxt/induction_step.hpp"
#include "../intervalxt/label.hpp"

#include "impl/component.impl.hpp"
#include "impl/connection.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/dynamical_decomposition.impl.hpp"
#include "impl/half_edge.impl.hpp"
#include "impl/separatrix.impl.hpp"

#include "util/assert.ipp"

namespace intervalxt {

using std::begin;
using std::end;
using std::string;
using std::vector;

using boost::lexical_cast;

using InductionResult = InductionStep::Result;
using DecompositionResult = DecompositionStep::Result;
using Contour = Implementation<HalfEdge>::Contour;

Component::Component() :  // We assume that the caller takes care of initializing impl.
                         impl(nullptr) {}

boost::logic::tribool Component::cylinder() const noexcept {
  return impl->state.cylinder;
}

boost::logic::tribool Component::withoutPeriodicTrajectory() const noexcept {
  return impl->state.withoutPeriodicTrajectory;
}

boost::logic::tribool Component::keane() const noexcept {
  return impl->state.keane;
}

vector<Side> Component::perimeter() const {
  auto perimeter = rx::chain(bottom(), right(), top(), left()) | rx::to_vector();

  ASSERT(std::unordered_set<Side>(begin(perimeter), end(perimeter)).size() == perimeter.size(), "Perimeter must not contain duplicates.");

  return perimeter;
}

vector<HalfEdge> Component::topContour() const {
  return (impl->state.iet.swapped() ? impl->state.iet.bottom() : impl->state.iet.top()) | rx::transform([&](const auto& label) { return HalfEdge(*this, label); }) | rx::to_vector();
}

vector<HalfEdge> Component::bottomContour() const {
  return (impl->state.iet.swapped() ? impl->state.iet.top() : impl->state.iet.bottom()) | rx::transform([&](const auto& label) { return -HalfEdge(*this, label); }) | rx::to_vector();
}

vector<Side> Component::left() const {
  return rx::chain(
             impl->decomposition->bottom.at(*begin(impl->state.iet.swapped() ? impl->state.iet.top() : impl->state.iet.bottom())).left,
             impl->decomposition->top.at(*begin(impl->state.iet.swapped() ? impl->state.iet.bottom() : impl->state.iet.top())).left) |
         rx::transform([](const auto& connection) { return Side(connection); }) | rx::reverse() | rx::to_vector();
}

vector<Side> Component::right() const {
  return rx::chain(
             impl->decomposition->bottom.at(*rbegin(impl->state.iet.swapped() ? impl->state.iet.top() : impl->state.iet.bottom())).right,
             impl->decomposition->top.at(*rbegin(impl->state.iet.swapped() ? impl->state.iet.bottom() : impl->state.iet.top())).right) |
         rx::transform([](const auto& connection) { return Side(connection); }) | rx::to_vector();
}

vector<Side> Component::bottom() const {
  return Implementation::horizontal(*this, false);
}

vector<Side> Component::top() const {
  return Implementation::horizontal(*this, true);
}

bool Component::operator==(const Component& rhs) const {
  return impl->decomposition == rhs.impl->decomposition &&
         &impl->state == &rhs.impl->state;
}

DecompositionStep Component::decompositionStep(int limit) {
  ASSERT(!cylinder() || !keane(), "Component " << *this << " is already fully decomposed.");

  auto& self = impl->state;

  int boshernitzanCost = Implementation::boshernitzanCost(self.iet);

  InductionStep step;

  do {
    int zorichInductionSteps;
    if (limit == -1) {
      zorichInductionSteps = boshernitzanCost;
    } else if (limit < 2 * boshernitzanCost) {
      zorichInductionSteps = limit;
    } else {
      zorichInductionSteps = boshernitzanCost;
    }

    step = self.iet.induce(zorichInductionSteps);
  } while (step.result == InductionResult::LIMIT_REACHED && limit != 0);

  switch (step.result) {
    case InductionResult::LIMIT_REACHED:
      return {DecompositionResult::LIMIT_REACHED};
    case InductionResult::CYLINDER:
      self.cylinder = true;
      self.withoutPeriodicTrajectory = false;
      self.keane = false;
      return DecompositionStep{
          DecompositionResult::CYLINDER,
      };
    case InductionResult::SEPARATING_CONNECTION: {
      auto [b, t] = *step.connection;

      auto equivalent = impl->walkClockwise(-HalfEdge(*this, *rbegin(bottomContour())), HalfEdge(*this, *rbegin(topContour())));

      auto connection = ::intervalxt::Implementation<Connection>::make(
          impl->decomposition,
          ::intervalxt::Implementation<Separatrix>::atBottom(impl->decomposition, b),
          ::intervalxt::Implementation<Separatrix>::atTop(impl->decomposition, t));
      auto right = ::intervalxt::Implementation<DynamicalDecomposition>::createComponent(impl->decomposition, *this, connection, std::move(*step.additionalIntervalExchangeTransformation));
      return {
          DecompositionResult::SEPARATING_CONNECTION,
          connection,
          equivalent,
          right};
    }
    case InductionResult::NON_SEPARATING_CONNECTION: {
      auto& bottom = impl->decomposition->bottom;
      auto& top = impl->decomposition->top;

      // We found a non-separating connection b ⚯ t.
      auto [b, t] = *step.connection;
      ASSERT(b != t, "Mistook cylinder for a non-separating connection");

      // t is not valid anymore but we can still use cross() on it.
      auto equivalent = HalfEdge(*this, t).cross();
      equivalent.splice(end(equivalent), (-HalfEdge(*this, b)).cross());
      std::reverse(begin(equivalent), end(equivalent));
      for (auto& side : equivalent) {
        if (auto connection = std::get_if<intervalxt::Connection>(&side)) {
          side = -*connection;
        } else {
          // This should probably change with #68.
          ;
        }
      }

      auto connection = ::intervalxt::Implementation<Connection>::make(
          impl->decomposition,
          ::intervalxt::Implementation<Separatrix>::atBottom(impl->decomposition, b),
          ::intervalxt::Implementation<Separatrix>::atTop(impl->decomposition, t));

      // Register the new connection right of b.
      bottom.at(b).right.push_back(connection);
      top.at(*begin(impl->state.iet.top())).left.push_front(-connection);

      // The label t has been eliminated by moving the initial section to
      // where t is in the bottom contour. Before we perform this move of
      // connections, make sure no connections are associated to t anymore.
      bottom.at(b).left.splice(end(bottom.at(b).left), top.at(t).left);
      bottom.at(b).right.splice(end(bottom.at(b).right), top.at(t).right);

      // Now we move the initial section under t.
      bottom.at(b).left.splice(end(bottom.at(b).left), bottom.at(t).left);
      bottom.at(b).right.splice(end(bottom.at(b).right), bottom.at(t).right);

      ASSERT(not left().empty(), "we just added a left boundary so it cannot be empty");

      return {
          DecompositionResult::NON_SEPARATING_CONNECTION,
          connection,
          equivalent};
    }
    case InductionResult::WITHOUT_PERIODIC_TRAJECTORY:
      self.withoutPeriodicTrajectory = true;
      self.cylinder = false;
      return {DecompositionResult::WITHOUT_PERIODIC_TRAJECTORY};
    default:
      throw std::logic_error("not implemented: unknown enum value");
  }
}

bool Component::decompose(std::function<bool(const Component&)> target, int limit) {
  bool limitReached = false;
  while (!target(*this)) {
    auto step = decompositionStep(limit);
    if (step.result == DecompositionResult::LIMIT_REACHED)
      return false;
    if (step.additionalComponent) {
      limitReached = limitReached || !step.additionalComponent->decompose(target, limit);
    }
  }
  return not limitReached;
}

std::pair<std::list<Connection>, std::list<Connection>> Component::inject(const HalfEdge& at, const vector<std::pair<Label, Label>>& left_, const vector<std::pair<Label, Label>>& right_) {
  using Orientation = ::intervalxt::Implementation<Separatrix>::Orientation;

  auto& state = impl->decomposition;

  state->check();

  const bool top = at.top();

  auto& connections = top ? state->top.at(at) : state->bottom.at(at);

  std::list<Connection> leftInjected, rightInjected;

  // Inject left connections.
  {
    auto left = left_;

    if (top) std::reverse(begin(left), end(left));

    ASSERT(left.empty() || connections.left.empty(), "cannot inject into a component with existing connections");

    for (const auto& [source, target] : left) {
      auto connection = ::intervalxt::Implementation<Connection>::make(state,
                                                                       ::intervalxt::Implementation<Separatrix>::make(state, source, Orientation::ANTIPARALLEL),
                                                                       ::intervalxt::Implementation<Separatrix>::make(state, target, Orientation::PARALLEL));
      connections.left.push_back(connection);
      leftInjected.push_back(connection);
    }

    if (top) std::reverse(begin(leftInjected), end(leftInjected));
  }

  // Inject right connections.
  {
    auto right = right_;

    if (top) std::reverse(begin(right), end(right));

    ASSERT(right.empty() || connections.right.empty(), "cannot inject into a component with existing connections");

    for (const auto& [source, target] : right) {
      auto connection = ::intervalxt::Implementation<Connection>::make(state,
                                                                       ::intervalxt::Implementation<Separatrix>::make(state, source, Orientation::PARALLEL),
                                                                       ::intervalxt::Implementation<Separatrix>::make(state, target, Orientation::ANTIPARALLEL));
      connections.right.push_back(connection);
      rightInjected.push_back(connection);
    }

    if (top) std::reverse(begin(rightInjected), end(rightInjected));
  }

  state->check();

  return {leftInjected, rightInjected};
}

Implementation<Component>::Implementation(std::shared_ptr<DecompositionState> decomposition, ComponentState* state) : decomposition(decomposition),
                                                                                                                      state(*state) {}

Component Implementation<Component>::make(std::shared_ptr<DecompositionState> decomposition, ComponentState* state) {
  Component component;
  component.impl = spimpl::make_impl<Implementation>(decomposition, state);
  return component;
}

vector<Side> Implementation<Component>::horizontal(const Component& component, bool top) {
  vector<Side> contour;

  const auto add = [&](const Connection& connection) {
    assert(not contour.empty());
    if (auto last = std::get_if<Connection>(&*rbegin(contour))) {
      if (*last == -connection) {
        contour.pop_back();
        return;
      }
    }
    contour.push_back(connection);
  };

  auto halfEdges = top ? component.topContour() : component.bottomContour();
  const auto& collapsed = top ? component.impl->decomposition->top : component.impl->decomposition->bottom;

  for (auto edge = begin(halfEdges); edge != end(halfEdges); edge++) {
    if (edge != begin(halfEdges))
      collapsed.at(*edge).left | rx::reverse() | rx::for_each(add);

    contour.push_back(*edge);

    if (edge != --end(halfEdges))
      collapsed.at(*edge).right | rx::for_each(add);
  }

  if (top) std::reverse(contour.begin(), contour.end());

  return contour;
}

int Implementation<Component>::boshernitzanCost(const IntervalExchangeTransformation&) {
  // Not implemented: see #60.
  return 1;
}

std::shared_ptr<DecompositionState> Implementation<Component>::parent(const Component& self) {
  return self.impl->decomposition;
}

// It would be easier to always walk counterclockwise as that is compatible with the order in cross(), see #69.
std::list<Side> Implementation<Component>::walkClockwise(HalfEdge from, HalfEdge to) const {
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

std::optional<HalfEdge> Implementation<Component>::next(const Component& self, const HalfEdge& edge) {
  auto contour = edge.top() ? self.impl->state.iet.top() : self.impl->state.iet.bottom();
  auto pos = std::find(begin(contour), end(contour), static_cast<Label>(edge));
  ASSERT(pos != end(contour), "half edge " << edge << " not in component " << self)
  if (++pos == end(contour)) return {};
  HalfEdge next = HalfEdge(self, *pos);
  return edge.top() ? next : -next;
}

std::optional<HalfEdge> Implementation<Component>::previous(const Component& self, const HalfEdge& edge) {
  auto contour = edge.top() ? self.impl->state.iet.top() : self.impl->state.iet.bottom();
  auto pos = std::find(begin(contour), end(contour), static_cast<Label>(edge));
  ASSERT(pos != end(contour), "half edge " << edge << " not in component " << self)
  if (pos == begin(contour)) return {};
  HalfEdge previous = HalfEdge(self, *--pos);
  return edge.top() ? previous : -previous;
}

void Implementation<Component>::registerSeparating(Component& left, const Connection& connection, Component& right) {
  ASSERT(connection.parallel(), "separating connection must by convention be from bottom to top");
  ASSERT(left != right, "separating connection must separate distinct components");
  auto& decomposition = *left.impl->decomposition;

  decomposition.bottom.at(*rbegin(left.impl->state.iet.bottom())).right.push_back(connection);
  decomposition.bottom.at(*begin(right.impl->state.iet.bottom())).left.push_back(-connection);

  ASSERT(not left.right().empty(), "we just added a right boundary so it cannot be empty");
  ASSERT(not right.left().empty(), "we just added a left boundary so it cannot be empty");
}

std::ostream& operator<<(std::ostream& os, const Component& self) {
  return os << boost::algorithm::join(
             self.perimeter() | rx::transform([](const auto& side) { return lexical_cast<string>(side); }) | rx::to_vector(), " ");
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
