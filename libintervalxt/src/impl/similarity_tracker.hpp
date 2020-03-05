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

#ifndef LIBINTERVALXT_IMPL_SIMILARITY_TRACKER_HPP
#define LIBINTERVALXT_IMPL_SIMILARITY_TRACKER_HPP

#include <unordered_set>

#include "../../intervalxt/interval_exchange_transformation.hpp"

namespace intervalxt {

// Tracks whether Zorich Inductions for an IET are caught in a loop.
class SimilarityTracker {
 public:
  bool loop(const IntervalExchangeTransformation&);

  void reset(const IntervalExchangeTransformation&);

 private:
  size_t periodBound = 1;
  size_t ttl = 0;

  std::vector<Label> top;
  std::vector<size_t> bottom;
  Lengths lengths;
};

}

#endif
