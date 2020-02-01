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

#ifndef LIBINTERVALXT_SEPARATRIX_HPP
#define LIBINTERVALXT_SEPARATRIX_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace intervalxt {

class Separatrix : boost::equality_comparable<Separatrix> {
  // Separatrices can not be created directly (other than copying & moving them.)
  // They are created in the process of a DynamicalDecomposition.
  Separatrix();

 public:
  // Whether the separatrix goes from bottom to top.
  bool parallel() const noexcept;
  // Whether the separatrix goes from top to bottom.
  bool antiparallel() const noexcept;

  bool operator==(const Separatrix&) const;

  friend std::ostream& operator<<(std::ostream&, const Separatrix&);

 private:
  using Implementation = ::intervalxt::Implementation<Separatrix>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;
  friend std::hash<Separatrix>;
};

}  // namespace intervalxt

namespace std {

template <>
struct hash<intervalxt::Separatrix> { size_t operator()(const intervalxt::Separatrix&) const noexcept; };

}  // namespace std

#endif
