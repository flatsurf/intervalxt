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
std::vector<std::list<Connection<Length>>> Component<Length>::left() const {
  std::vector<std::list<Connection<Length>>> boundaries;

  for (auto& label : this->impl->component->iet.top()) {
    auto boundary = follow(impl->state->makeTop(label));
    if (boundary.size()) boundaries.push_back(boundary);
  }

  return boundaries;
}

template <typename Length>
std::vector<std::list<Connection<Length>>> Component<Length>::right() const {
  std::vector<std::list<Connection<Length>>> boundaries;

  for (auto& label : this->impl->component->iet.bottom()) {
    auto boundary = follow(impl->state->makeBottom(label));
    if (boundary.size()) boundaries.push_back(boundary);
  }

  return boundaries;
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const Component<Length>& self) {
  if (self.impl->component->cylinder) {
    return os << "Cylinder(" << *self.impl->component->cylinder << ")";
  } else {
    return os << self.impl->component->iet;
  }
}

}  // namespace intervalxt
#endif
