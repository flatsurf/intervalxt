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

#include "../intervalxt/decomposition_step.hpp"

#include <ostream>

namespace intervalxt {

using std::ostream;

ostream& operator<<(ostream& os, const DecompositionStep& self) {
  using Result = DecompositionStep::Result;

  switch (self.result) {
    case Result::LIMIT_REACHED:
      return os << "LIMIT_REACHED";
    case Result::CYLINDER:
      return os << "CYLINDER";
    case Result::SEPARATING_CONNECTION:
      return os << "SEPARATING_CONNECTION(" << *self.connection << ", " << *self.additionalComponent << ")";
    case Result::NON_SEPARATING_CONNECTION:
      return os << "NON_SEPARATING_CONNECTION(" << *self.connection << ")";
    case Result::KEANE:
      return os << "KEANE";
    case Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN:
      return os << "WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN";
    case Result::WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR:
      return os << "WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR";
    default:
      throw std::logic_error("invalid enum value");
  }
}

}  // namespace intervalxt
