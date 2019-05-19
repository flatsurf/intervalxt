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

#ifndef LIBINTERVALXT_LABEL_HPP
#define LIBINTERVALXT_LABEL_HPP

#include <vector>

#include "intervalxt/intervalxt.hpp"
#include "intervalxt/forward.hpp"

#include "intervalxt/interval.hpp"

namespace intervalxt {

// A Label is the data attached to a pair of matched intervals on the top
// and bottom
template <typename Tlen, typename Tmat>
class Label {
 public:
  Tlen length;          // length of the subinterval (real part, >= 0)
  std::vector<Tmat> v;  // Kontsevich-Zorich cocycle (= coordinate of core curves)
  size_t index;         // index in the iet (a number in {0, 1, ..., nb label - 1}

  Interval<Tlen, Tmat> i1, i2;  // top and bot subintervals

  Label();
};

}

#endif


