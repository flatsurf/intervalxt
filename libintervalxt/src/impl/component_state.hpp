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

#ifndef LIBINTERVALXT_COMPONENT_STATE_HPP
#define LIBINTERVALXT_COMPONENT_STATE_HPP

#include <vector>

#include <boost/logic/tribool.hpp>

#include "forward.hpp"
#include "../../intervalxt/interval_exchange_transformation.hpp"

namespace intervalxt {

using std::vector;

struct ComponentState {
  ComponentState(IntervalExchangeTransformation&&);
  
  // TODO: I might be leaking memory here. Does this hold a reference to
  // DecompositionState through the LengthsWithConnections? :(
  // If so, then this must be stored in the DecompositionState itself.
  IntervalExchangeTransformation iet;

  boost::logic::tribool cylinder = boost::logic::indeterminate;
  boost::logic::tribool withoutPeriodicTrajectory = boost::logic::indeterminate;
  boost::logic::tribool keane = boost::logic::indeterminate;
};

}

#endif


