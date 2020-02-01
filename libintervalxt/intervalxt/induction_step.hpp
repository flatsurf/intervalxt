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

#include <optional>

#include "forward.hpp"
#include "interval_exchange_transformation.hpp"
#include "label.hpp"

namespace intervalxt {

// The result of a call to IntervalExchangeTransformation::induce()
struct InductionStep {
  enum class Result {
    LIMIT_REACHED,
    CYLINDER,
    SEPARATING_CONNECTION,
    NON_SEPARATING_CONNECTION,
    WITHOUT_PERIODIC_TRAJECTORY,
  };

  Result result;
  std::optional<std::pair<Label, Label>> connection = {};
  std::optional<IntervalExchangeTransformation> additionalIntervalExchangeTransformation = {};
};

std::ostream& operator<<(std::ostream&, const InductionStep&);

}  // namespace intervalxt

#endif
