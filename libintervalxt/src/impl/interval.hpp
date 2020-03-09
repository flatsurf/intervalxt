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

#include <boost/operators.hpp>
#include <list>

#include "../../intervalxt/label.hpp"

namespace intervalxt {

struct Interval : boost::equality_comparable<Interval>,
                  boost::equality_comparable<Interval, Label> {
  explicit Interval(const Label label) :
    label(label) {}

  inline operator Label() const noexcept { return label; }
  inline bool operator==(const Interval& rhs) const noexcept { return label == rhs.label; }
  inline bool operator==(const Label rhs) const noexcept { return label == rhs; }

  Label label;
  typename std::list<Interval>::iterator twin;
};

}  // namespace intervalxt

#endif
