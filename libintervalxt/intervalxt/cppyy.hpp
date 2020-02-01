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

#ifndef LIBINTERVALXT_CPPYY_HPP
#define LIBINTERVALXT_CPPYY_HPP

#include <iosfwd>
#include <memory>
#include <vector>

#include "forward.hpp"
#include "interval_exchange_transformation.hpp"
#include "label.hpp"
#include "lengths.hpp"

#include "sample/lengths.hpp"

namespace intervalxt {

template <typename L>
auto makeIET(const L &lengths, std::vector<int> permutation) {
  Lengths erasedLengths = lengths;
  auto top = lengths.labels();
  std::vector<Label> bottom;
  for (int p : permutation)
    bottom.push_back(top[p]);
  return IntervalExchangeTransformation(std::make_shared<Lengths>(erasedLengths), top, bottom);
}

std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation &);

}  // namespace intervalxt

#endif
