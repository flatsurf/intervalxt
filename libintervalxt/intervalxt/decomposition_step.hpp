/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019      Vincent Delecroix
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

#ifndef LIBINTERVALXT_DECOMPOSITION_STEP_HPP
#define LIBINTERVALXT_DECOMPOSITION_STEP_HPP

#include "component.hpp"
#include "connection.hpp"
#include "half_edge.hpp"

namespace intervalxt {

// The result of a call to Component::decompositionStep.
struct LIBINTERVALXT_API DecompositionStep {
  enum class Result {
    LIMIT_REACHED,
    CYLINDER,
    SEPARATING_CONNECTION,
    NON_SEPARATING_CONNECTION,
    WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN,
    WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR,
    KEANE,
  };

  Result result;
  std::optional<Connection> connection = {};
  std::optional<std::list<Side>> equivalent = {};
  std::optional<Component> additionalComponent = {};
};

LIBINTERVALXT_API std::ostream& operator<<(std::ostream&, const DecompositionStep&);

}  // namespace intervalxt

#endif
