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

#ifndef LIBINTERVALXT_SEPARATRIX_IMPL_HPP
#define LIBINTERVALXT_SEPARATRIX_IMPL_HPP

#include <memory>

#include "../../intervalxt/label.hpp"

#include "../../intervalxt/separatrix.hpp"
#include "forward.hpp"

namespace intervalxt {

template <>
class Implementation<Separatrix> {
 public:
  enum class Orientation {
    PARALLEL = -1,
    ANTIPARALLEL = 1,
  };

  Implementation(std::shared_ptr<DecompositionState>, Label, Orientation);

  static Separatrix make(std::shared_ptr<DecompositionState>, Label, Orientation);

  // Return the top separatrix that connections created right of the label would report now.
  static Separatrix atTop(std::shared_ptr<DecompositionState>, Label);
  // Return the bottom separatrix that connections created right of the label would report now.
  static Separatrix atBottom(std::shared_ptr<DecompositionState>, Label);

  const std::shared_ptr<DecompositionState> decomposition;
  Label label;
  Orientation orientation;
};

}  // namespace intervalxt

#endif
