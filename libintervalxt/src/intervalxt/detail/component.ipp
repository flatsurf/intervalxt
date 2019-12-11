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

#ifndef LIBINTERVALXT_DETAIL_COMPONENT_IPP
#define LIBINTERVALXT_DETAIL_COMPONENT_IPP

#include <boost/logic/tribool.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "../dynamical_decomposition.hpp"

namespace intervalxt {
namespace {
template <typename Length>
std::list<Connection<Length>> follow(const MaybeConnection<Length>& source) {
  std::list<Connection<Length>> boundary;

  MaybeConnection<Length> current = source;
  do {
    auto connection = current.connection();
    if (!connection) break;
    boundary.push_back(*connection);
    current = connection->nextInBoundary();
  } while (current != source);

  return boundary;
}
}  // namespace

template <typename Length>
Component<Length>::Component() {
  // we assume the caller to be aware that the impl needs to be initialized.
}

template <typename Length>
boost::tribool Component<Length>::cylinder() const noexcept {
  if (impl->component->cylinder)
    return true;
  if (impl->component->withoutPeriodicTrajectory)
    return false;
  return boost::logic::indeterminate;
}

template <typename Length>
boost::tribool Component<Length>::withoutPeriodicTrajectory() const noexcept { return impl->component->withoutPeriodicTrajectory; }

template <typename Length>
boost::tribool Component<Length>::keane() const noexcept { return impl->component->keane; }

template <typename Length>
DecompositionStep<Length> Component<Length>::decompositionStep(int limit) {
  return impl->state->decompositionStep(impl->component, limit);
}

template <typename Length>
Component<Length> Component<Length>::Implementation::make(std::shared_ptr<DecompositionState> state, ComponentState* const component) {
  Component<Length> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, component);
  return ret;
}

template <typename Length>
std::vector<Label<Length>> Component<Length>::top() const {
  std::vector<Label<Length>> top;
  
  if (this->cylinder()) {
    top.push_back(*this->impl->component->cylinder);
  } else {
    for (auto& label : this->impl->component->iet.top())
      top.push_back(label);
  }

  std::reverse(top.begin(), top.end());

  assert(top.size() && "top contour cannot be empty");

  return top;
}

template <typename Length>
std::vector<Label<Length>> Component<Length>::bottom() const {
  std::vector<Label<Length>> bottom;
  
  if (this->cylinder()) {
    bottom.push_back(*this->impl->component->cylinder);
  } else {
    for (auto& label : this->impl->component->iet.bottom())
      bottom.push_back(label);
  }

  assert(bottom.size() && "bottom contour cannot be empty");

  return bottom;
}

template <typename Length>
std::vector<std::list<Connection<Length>>> Component<Length>::left() const {
  std::vector<std::list<Connection<Length>>> boundaries;

  if (this->cylinder()) {
    auto boundary = follow(impl->state->makeTop(*impl->component->cylinder));
    if (boundary.size()) boundaries.push_back(boundary);
  } else {
    for (auto& label : this->impl->component->iet.top()) {
      auto boundary = follow(impl->state->makeTop(label));
      if (boundary.size()) boundaries.push_back(boundary);
    }
  }

  return boundaries;
}

template <typename Length>
std::vector<std::list<Connection<Length>>> Component<Length>::right() const {
  std::vector<std::list<Connection<Length>>> boundaries;

  if (this->cylinder()) {
    auto boundary = follow(impl->state->makeBottom(*impl->component->cylinder));
    if (boundary.size()) boundaries.push_back(boundary);
  } else {
    for (auto& label : this->impl->component->iet.bottom()) {
      auto boundary = follow(impl->state->makeBottom(label));
      if (boundary.size()) boundaries.push_back(boundary);
    }
  }

  return boundaries;
}

template <typename Length>
bool Component<Length>::decompose(std::function<bool(const Component<Length>&)> target, int limit) {
  bool limitNotReached = true;
  while (!target(*this)) {
    auto step = decompositionStep(limit);
    if (step.result == DecompositionStep<Length>::Result::LIMIT_REACHED)
      return false;
    if (step.additionalComponent) {
      limitNotReached = limitNotReached && step.additionalComponent->decompose(target, limit);
    }
  }
  return limitNotReached;
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const Component<Length>& self) {
  const auto format = [](const auto& boundaries) {
    return "["
      + boost::algorithm::join(boundaries | boost::adaptors::transformed([](const auto& boundary) {
        return "[" + boost::algorithm::join(boundary | boost::adaptors::transformed([](const auto& connection) {
          return boost::lexical_cast<std::string>(connection);
        }), ", ") + "]";
      }), ", ")
      + "]";
  };

  if (self.impl->component->cylinder) {
    return os << "Cylinder(" << *self.impl->component->cylinder << ", left=" << format(self.left()) << ", right=" << format(self.right()) << ")";
  } else {
    return os << self.impl->component->iet << " with left=" << format(self.left()) << ", right=" << format(self.right());
  }
}

}  // namespace intervalxt
#endif
