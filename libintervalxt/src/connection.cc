/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include "../intervalxt/connection.hpp"

#include <ostream>

#include "impl/connection.impl.hpp"
#include "impl/decomposition_state.hpp"
#include "impl/implementation_of_decomposition.hpp"
#include "impl/separatrix.impl.hpp"

namespace intervalxt {

Separatrix Connection::source() const {
  return ImplementationOf<Separatrix>::make(self->decomposition, self->connection.source);
}

Separatrix Connection::target() const {
  return ImplementationOf<Separatrix>::make(self->decomposition, self->connection.target);
}

bool Connection::operator==(const Connection& rhs) const {
  // Note that we identify separatrices on different decompositions. This seems
  // wrong. However, it is currently, necessary because Lenghts are not shared
  // in libflatsurf between IETs, see #72.
  return ImplementationOf<Separatrix>::make(self->decomposition, self->connection.source) == ImplementationOf<Separatrix>::make(rhs.self->decomposition, rhs.self->connection.source) && ImplementationOf<Separatrix>::make(self->decomposition, self->connection.target) == ImplementationOf<Separatrix>::make(rhs.self->decomposition, rhs.self->connection.target);
}

bool Connection::parallel() const {
  return ImplementationOf<Separatrix>::make(self->decomposition, self->connection.source).parallel();
}

bool Connection::antiparallel() const {
  return not parallel();
}

Connection Connection::operator-() const {
  return ImplementationOf<Connection>::make(self->decomposition, {self->connection.target, self->connection.source});
}

ImplementationOf<Connection>::ImplementationOf(const DynamicalDecomposition& decomposition, DecompositionState::Connection connection) :
  ImplementationOfDecomposition(decomposition),
  connection(std::move(connection)) {}

Connection ImplementationOf<Connection>::make(const DynamicalDecomposition& decomposition, DecompositionState::Connection connection) {
  return Connection(PrivateConstructor{}, decomposition, std::move(connection));
}

std::ostream& operator<<(std::ostream& os, const Connection& self) {
  return os << "[" << self.source() << " ⚯ " << self.target() << "]";
}

}  // namespace intervalxt

namespace std {

using namespace intervalxt;

size_t hash<Connection>::operator()(const Connection& self) const {
  // We should use hash_combine here, see https://github.com/flatsurf/intervalxt/issues/67
  return hash<Separatrix>()(self.source()) + (hash<Separatrix>()(self.target()) << 32);
}

}  // namespace std
