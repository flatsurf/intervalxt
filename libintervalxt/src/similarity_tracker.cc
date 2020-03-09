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

#include "impl/similarity_tracker.hpp"

namespace intervalxt {

bool SimilarityTracker::loop(const IntervalExchangeTransformation& iet) {
  if (ttl-- <= 0) {
    reset(iet);
    return false;
  }

  if (iet.size() != top.size())
    return false;

  auto top = iet.top();
  auto bottom = iet.bottom();

  for (size_t i = 0; i < iet.size(); i++)
    if (bottom[i] != top[this->bottom[i]])
      return false;

  for (size_t i = 1; i < iet.size(); i++) {
    const Label& a = this->top[i - 1];
    const Label& b = this->top[i];

    const Label& othera = this->top[i - 1];
    const Label& otherb = this->top[i];

    if (!lengths.similar(a, b, *iet.lengths(), othera, otherb))
      return false;
  }

  return true;
}

void SimilarityTracker::reset(const IntervalExchangeTransformation& iet) {
  top = iet.top();
  lengths = iet.lengths()->forget().only(std::unordered_set<Label>(begin(top), end(top)));

  bottom.clear();
  for (const auto& b : iet.bottom()) {
    for (size_t pos = 0; pos < top.size(); pos++) {
      if (top[pos] == b) {
        bottom.push_back(pos);
      }
    }
  }

  ttl = periodBound;
  periodBound *= 2;
}

}  // namespace intervalxt
