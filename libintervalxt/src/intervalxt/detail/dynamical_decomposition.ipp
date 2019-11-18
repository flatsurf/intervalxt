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
 *t
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBINTERVALXT_DETAIL_DYNAMICAL_DECOMPOSITION_IPP
#define LIBINTERVALXT_DETAIL_DYNAMICAL_DECOMPOSITION_IPP

#include <boost/logic/tribool.hpp>
#include <cassert>
#include <map>

#include "../component.hpp"
#include "../connection.hpp"
#include "../dynamical_decomposition.hpp"
#include "../interval_exchange_transformation.hpp"
#include "../maybe_connection.hpp"

// Consult the rant on top of interval_exchange_transformation.ipp for why we
// have to put the implementation into this header file.
// However, it would be conceivable to put this implementation into a regular
// .cc file by hiding the actual IET away in a small virtual class or by hiding
// it behind a small number of lambda callbacks.

// Note that Component, DynamicalDecomposition, and Connection are just
// pointers into a more heavy global state, the DecompositionState class defined
// below.
// All of these keep the DecompositionState alive by holding a shared_ptr to it.

namespace intervalxt {
template <typename Length>
class DynamicalDecomposition<Length>::Implementation {
 public:
  explicit Implementation(IntervalExchangeTransformation<Length>&& iet) : state(new DecompositionState(std::move(iet))) {}

  class DecompositionState;

 private:
  friend class DynamicalDecomposition<Length>;
  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const DynamicalDecomposition<T>&);

  std::shared_ptr<DecompositionState> state;
};

template <typename Length>
class Component<Length>::Implementation {
 public:
  using DecompositionState = typename DynamicalDecomposition<Length>::Implementation::DecompositionState;

  struct ComponentState;

  Implementation(std::shared_ptr<DecompositionState> state, ComponentState* component) : state(state), component(component) {}

  static Component<Length> make(std::shared_ptr<DecompositionState> state, ComponentState* component);

 private:
  friend class Component<Length>;
  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const Component<T>&);

  std::shared_ptr<DecompositionState> state;
  ComponentState* const component;
};

template <typename Length>
class Connection<Length>::Implementation {
 public:
  using DecompositionState = typename DynamicalDecomposition<Length>::Implementation::DecompositionState;

  struct ConnectionState;

  Implementation(std::shared_ptr<const DecompositionState> state, ConnectionState* const connection) : state(state), connection(connection) {}

  static Connection<Length> make(std::shared_ptr<const DecompositionState> state, ConnectionState* const component);

 private:
  friend class Connection<Length>;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const Connection<T>&);

  std::shared_ptr<const DecompositionState> state;
  ConnectionState* const connection;
};

template <typename Length>
class MaybeConnection<Length>::Implementation {
 public:
  using DecompositionState = typename DynamicalDecomposition<Length>::Implementation::DecompositionState;
  using ConnectionState = typename Connection<Length>::Implementation::ConnectionState;

  Implementation(std::shared_ptr<const DecompositionState> state, ConnectionState* connection) : state(state), connection(connection) {}

  static MaybeConnection<Length> make(std::shared_ptr<const DecompositionState> state, ConnectionState* connection);

 private:
  friend class MaybeConnection<Length>;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const MaybeConnection<T>&);

  std::shared_ptr<const DecompositionState> state;
  ConnectionState* const connection;
};

template <typename Length>
struct Component<Length>::Implementation::ComponentState {
  using ConnectionState = typename Connection<Length>::Implementation::ConnectionState;

  IntervalExchangeTransformation<Length> iet;
  std::optional<Label<Length>> cylinder = {};
  boost::logic::tribool withoutPeriodicTrajectory = boost::logic::indeterminate;
  boost::logic::tribool keane = boost::logic::indeterminate;
};

// ConnectionState keeps track of saddle connections detected during the
// process of the dynamical decomposition. Initially, all connections are
// unmatched, i.e., at every inner endpoint of an interval, there is an
// unmatched connection, identified by the labels that were initially next to
// it.
template <typename Length>
struct Connection<Length>::Implementation::ConnectionState {
  // The label initially to the left of this connection if a bottom
  // connection, otherwise the one to the right.
  const Label<Length> before;
  // The label initially to the left of this connection if a top
  // connection, otherwise the one to the right.
  const Label<Length> after;

  // The next connection in clockwise order at the singularity at the source of this connection.
  ConnectionState* previousAtSingularity;
  // The next connection in counter-clockwise order at the singularity at the source of this connection.
  ConnectionState* nextAtSingularity;

  // When this is part of a saddle connection, the target of that saddle
  // connection; null while this has not been determined yet.
  ConnectionState* twin = nullptr;
};

template <typename Length>
class DynamicalDecomposition<Length>::Implementation::DecompositionState : public std::enable_shared_from_this<DynamicalDecomposition<Length>::Implementation::DecompositionState> {
 public:
  using ComponentState = typename Component<Length>::Implementation::ComponentState;
  using ConnectionState = typename Connection<Length>::Implementation::ConnectionState;

  Component<Length> make(ComponentState* component) { return Component<Length>::Implementation::make(this->shared_from_this(), component); }

  Connection<Length> make(ConnectionState* const connection) const { return Connection<Length>::Implementation::make(this->shared_from_this(), connection); }

  MaybeConnection<Length> makeMaybe(ConnectionState* const connection) const { return MaybeConnection<Length>::Implementation::make(this->shared_from_this(), connection); }

  MaybeConnection<Length> makeTop(const Label<Length>& before) const { return makeMaybe(top.at(before)); }

  MaybeConnection<Length> makeBottom(const Label<Length>& before) const { return makeMaybe(bottom.at(before)); }

  DecompositionStep<Length> decompositionStep(ComponentState* component, int limit) {
    using InductionResult = typename InductionStep<Length>::Result;
    using DecompositionResult = typename DecompositionStep<Length>::Result;

    if (component->cylinder || component->keane) {
      assert(false && "component is already fully decomposed");
    }

    int boshernitzanCost = estimateBoshernitzanCost(component->iet);

    InductionStep<Length> step;

    do {
      int zorichInductionSteps;
      if (limit == -1) {
        zorichInductionSteps = boshernitzanCost;
      } else if (limit < 2 * boshernitzanCost) {
        zorichInductionSteps = limit;
      } else {
        zorichInductionSteps = boshernitzanCost;
      }

      step = component->iet.induce(zorichInductionSteps);
    } while (step.result == InductionResult::LIMIT_REACHED && limit != 0);

    switch (step.result) {
      case InductionResult::LIMIT_REACHED:
        return {DecompositionResult::LIMIT_REACHED};
      case InductionResult::CYLINDER: {
        bool isAdditionalComponent = component->iet.size();
        ComponentState* cylinder = component;
        if (isAdditionalComponent) {
          components.emplace_back(ComponentState{IntervalExchangeTransformation<Length>()});
          cylinder = &*components.rbegin();
        }

        cylinder->cylinder = step.cylinder;
        cylinder->withoutPeriodicTrajectory = false;
        cylinder->keane = false;

        return DecompositionStep<Length>{
            DecompositionResult::CYLINDER,
            {},
            isAdditionalComponent ? std::optional<Component<Length>>(make(cylinder)) : std::optional<Component<Length>>{},
        };
      }
      case InductionResult::SEPARATING_CONNECTION:
        components.emplace_back(ComponentState{std::move(step.additionalIntervalExchangeTransformation).value()});
        return {
            DecompositionResult::SEPARATING_CONNECTION,
            make(recordConnection(step.connection->first, step.connection->second)),
            make(&*components.rbegin())};
      case InductionResult::NON_SEPARATING_CONNECTION:
        return {
            DecompositionResult::NON_SEPARATING_CONNECTION,
            make(recordConnection(step.connection->first, step.connection->second))};
      case InductionResult::WITHOUT_PERIODIC_TRAJECTORY:
        component->withoutPeriodicTrajectory = true;
        return {
            DecompositionResult::WITHOUT_PERIODIC_TRAJECTORY};
      default:
        throw std::logic_error("invalid enum value");
    }
  }

 private:
  explicit DecompositionState(IntervalExchangeTransformation<Length>&& iet) : components({}), top(), bottom() {
    auto topLabels = iet.top();
    auto bottomLabels = iet.bottom();

    for (size_t i = 1; i < topLabels.size(); i++) {
      connections.push_front({topLabels[i], topLabels[i - 1], nullptr, nullptr});
      top[topLabels[i]] = &*connections.begin();
    }
    top[topLabels[0]] = top[bottomLabels[0]];

    for (size_t i = 0; i < bottomLabels.size() - 1; i++) {
      connections.push_front({bottomLabels[i], bottomLabels[i + 1], nullptr, nullptr});
      bottom[bottomLabels[i]] = &*connections.begin();
    }
    bottom[bottomLabels[bottomLabels.size() - 1]] = bottom[topLabels[topLabels.size() - 1]];

    for (size_t i = 1; i < topLabels.size(); i++) {
      top[topLabels[i]]->previousAtSingularity = bottom[topLabels[i - 1]];
      bottom[topLabels[i - 1]]->nextAtSingularity = top[topLabels[i]];
    }
    for (size_t i = 0; i < bottomLabels.size() - 1; i++) {
      bottom[bottomLabels[i]]->previousAtSingularity = top[bottomLabels[i + 1]];
      top[bottomLabels[i + 1]]->nextAtSingularity = bottom[bottomLabels[i]];
    }

    components.emplace_back(ComponentState{std::move(iet)});
  }

  friend class DynamicalDecomposition<Length>;

  ConnectionState* recordConnection(const Label<Length>& afterTop, const Label<Length>& beforeBottom) {
    ConnectionState* bottom = this->bottom[beforeBottom];
    while (bottom->twin != nullptr)
      bottom = this->bottom[bottom->twin->after];

    ConnectionState* top = this->bottom[afterTop]->nextAtSingularity;
    assert(top->twin == nullptr && "repeated connections can only happen with non-separating connections, but these are always stacked from below");

    bottom->twin = top;
    top->twin = bottom;

    return bottom;
  }

  int estimateBoshernitzanCost(const IntervalExchangeTransformation<Length>&) {
    // see https://github.com/flatsurf/intervalxt/issues/60
    return 1;
  }

  std::list<ComponentState> components;
  std::list<ConnectionState> connections;

  // The top connection initially left of a label; note that this contains one
  // connection state twice (for the left-most interval.)
  std::map<Label<Length>, ConnectionState*> top;
  // The bottom connection initially right of a label; note that this contains
  // one connection state twice (for the right-most interval.)
  std::map<Label<Length>, ConnectionState*> bottom;
};

template <typename Length>
std::vector<Component<Length>> DynamicalDecomposition<Length>::components() const noexcept {
  std::vector<Component<Length>> ret;
  for (auto& component : impl->state->components)
    ret.emplace_back(impl->state->make(&component));
  return ret;
}

template <typename Length>
void DynamicalDecomposition<Length>::decompose(std::function<bool(const Component<Length>&)> target, int limit) {
  for (auto& component : impl->state->components)
    impl->state->make(&component).decompose(target, limit);
}

template <typename Length>
DynamicalDecomposition<Length>::DynamicalDecomposition(const IntervalExchangeTransformation<Length>& iet) : impl(spimpl::make_impl<Implementation>(IntervalExchangeTransformation<Length>(iet.top(), iet.bottom()))) {}

}  // namespace intervalxt

#include "./component.ipp"
#include "./connection.ipp"
#include "./maybe_connection.ipp"

#endif
