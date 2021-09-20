/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Vincent Delecroix
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBINTERVALXT_CONTEXT_HPP
#define LIBINTERVALXT_CONTEXT_HPP

#include <boost/logic/tribool.hpp>
#include <deque>
#include <list>
#include <unordered_map>

#include "../../intervalxt/interval_exchange_transformation.hpp"

namespace intervalxt {

struct DecompositionState {
  struct Component {
    IntervalExchangeTransformation iet;

    boost::logic::tribool cylinder = boost::logic::indeterminate;
    boost::logic::tribool withoutPeriodicTrajectory = boost::logic::indeterminate;
    boost::logic::tribool keane = boost::logic::indeterminate;
  };

  struct Separatrix {
    enum class Orientation {
      PARALLEL = -1,
      ANTIPARALLEL = 1,
    };

    Label label;
    Orientation orientation;
  };

  struct Connection {
    Separatrix source;
    Separatrix target;
  };

  struct Connections {
    std::list<Connection> topLeft;
    std::list<Connection> topRight;
    std::list<Connection> bottomLeft;
    std::list<Connection> bottomRight;
  };

  std::deque<Component> components;

  std::unordered_map<Label, Connections> connections;
};

}  // namespace intervalxt

#endif
