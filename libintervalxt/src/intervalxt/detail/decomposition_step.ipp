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

#ifndef LIBINTERVALXT_DETAIL_DECOMPOSITION_STEP_IPP
#define LIBINTERVALXT_DETAIL_DECOMPOSITION_STEP_IPP

#include <iostream>
#include <cassert>

#include "../decomposition_step.hpp"

namespace intervalxt {
template <typename Length>
std::ostream& operator<<(std::ostream& os, const DecompositionStep<Length>& self) {
  using Result = typename DecompositionStep<Length>::Result;

  switch(self.result) {
    case Result::LIMIT_REACHED:
      return os << "LIMIT_REACHED";
    case Result::CYLINDER:
      if (self.additionalComponent) {
        return os << "CYLINDER on " << *self.additionalComponent;
      } else {
        return os << "CYLINDER";
      }
    case Result::SEPARATING_CONNECTION:
      return os << "SEPARATING_CONNECTION after " << *self.connection << " giving " << *self.additionalComponent;
    case Result::NON_SEPARATING_CONNECTION:
      return os << "NON_SEPARATING_CONNECTION after " << *self.connection;
    case Result::WITHOUT_PERIODIC_TRAJECTORY:
      return os << "WITHOUT_PERIODIC_TRAJECTORY";
    case Result::KEANE:
      return os << "KEANE";
    default:
      assert(false && "impossible decomposition step state");
  }
}
}

#endif
