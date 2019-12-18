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

#ifndef LIBINTERVALXT_DETAIL_CONNECTION_IPP
#define LIBINTERVALXT_DETAIL_CONNECTION_IPP

#include <iostream>

#include "../dynamical_decomposition.hpp"

namespace intervalxt {

template <typename Length>
Connection<Length>::Connection() {
  // we assume the caller to be aware that the impl needs to be initialized.
}

template <typename Length>
Connection<Length> Connection<Length>::Implementation::make(std::shared_ptr<const DecompositionState> state, ConnectionState* const connection) {
  assert(connection->twin != nullptr && "cannot create connection from unmatched");
  Connection<Length> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, connection);
  return ret;
}

template <typename Length>
MaybeConnection<Length> Connection<Length>::source() const noexcept {
  return MaybeConnection<Length>::Implementation::make(impl->state, impl->connection);
}

template <typename Length>
MaybeConnection<Length> Connection<Length>::nextInBoundary() const noexcept {
  return this->impl->state->makeMaybe(this->impl->connection->twin->previousAtSingularity);
}

template <typename Length>
Connection<Length> Connection<Length>::operator-() const noexcept {
  return Implementation::make(impl->state, impl->connection->twin);
}

template <typename Length>
bool Connection<Length>::operator==(const Connection<Length>& rhs) const noexcept {
  return impl->connection == rhs.impl->connection;
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const Connection<Length>& self) {
  return os << self.impl->connection->before << " 🔗 " << self.impl->connection->twin->before;
}

}  // namespace intervalxt

#endif
