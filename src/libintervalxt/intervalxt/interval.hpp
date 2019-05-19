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

#ifndef LIBINTERVALXT_INTERVAL_HPP
#define LIBINTERVALXT_INTERVAL_HPP

#include "intervalxt/intervalxt.hpp"
#include "intervalxt/forward.hpp"

namespace intervalxt {

// An Interval models either a subinterval of the bottom or the top.
// It keeps pointers to the subinterval on the left, on the right
// and its twin. It is implemented as a doubled chained list so that Rauzy
// induction can be efficiently done
template <typename Tlen, typename Tmat>
class Interval {
 public:
  Interval *prev;   // interval on the left
  Interval *next;   // interval on the right
  Interval *twin;   // the twin interval
  Label<Tlen, Tmat> *lab;  // the label
};

}

#endif

