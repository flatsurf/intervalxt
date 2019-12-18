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

#ifndef LIBINTERVALXT_INDUCTION_STEP_HPP
#define LIBINTERVALXT_INDUCTION_STEP_HPP

#include "forward.hpp"

namespace intervalxt {

// The result of a call to IntervalExchangeTransformation::induce()
template <typename Length>
struct InductionStep {
  enum class Result {
    LIMIT_REACHED,
    CYLINDER,
    SEPARATING_CONNECTION,
    NON_SEPARATING_CONNECTION,
    WITHOUT_PERIODIC_TRAJECTORY,
  };

  Result result;
  std::optional<std::pair<Label<Length>, Label<Length>>> connection = {};
  std::optional<IntervalExchangeTransformation<Length>> additionalIntervalExchangeTransformation = {};
  std::optional<Label<Length>> cylinder = {};
};

template <typename Length>
std::ostream& operator<<(std::ostream&, const InductionStep<Length>&);

}  // namespace intervalxt

#include "detail/dynamical_decomposition.ipp"

#endif
