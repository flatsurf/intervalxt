/* ********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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
 * *******************************************************************/

#ifndef LIBINTERVALXT_LABEL_HPP
#define LIBINTERVALXT_LABEL_HPP

#include <functional>

#include <boost/operators.hpp>

#include "serializable.hpp"

namespace intervalxt {

// A label identifying a pair of intervals in an interval exchange
// transformation. The label itself carries no information about the length of
// the interval, the Lengths object knows about this and other metadata.
// Note that this class has on purpose no printing operator<< on purpose; the
// label itself is just some random id, only the Lengths object knows a
// meaningful name for this id which can be determined by calling
// Lengths::render().
class Label : public boost::equality_comparable<Label>,
              public Serializable<Label> {
 public:
  Label();
  explicit Label(size_t id);

  bool operator==(const Label&) const noexcept;

 private:
  size_t id;

  friend Serialization<Label>;
  friend std::hash<Label>;
};

}  // namespace intervalxt

namespace std {

template <>
struct hash<intervalxt::Label> { size_t operator()(const intervalxt::Label&) const noexcept; };

}  // namespace std

#endif
