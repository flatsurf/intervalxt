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

#include "../intervalxt/label.hpp"

#include <stdlib.h>

namespace intervalxt {

Label::Label() noexcept :
  id(static_cast<size_t>(random())) {}

Label::Label(size_t id) :
  id(id) {}

bool Label::operator==(const Label& rhs) const { return id == rhs.id; }

}  // namespace intervalxt

namespace std {

using namespace intervalxt;

size_t hash<Label>::operator()(const Label& self) const {
  return self.id;
}

}  // namespace std
