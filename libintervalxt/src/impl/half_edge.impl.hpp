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

#ifndef LIBINTERVALXT_HALF_EDGE_IMPL_HPP
#define LIBINTERVALXT_HALF_EDGE_IMPL_HPP

#include <memory>

#include "../../intervalxt/component.hpp"
#include "../../intervalxt/half_edge.hpp"
#include "../../intervalxt/label.hpp"
#include "forward.hpp"

namespace intervalxt {

template <>
class Implementation<HalfEdge> {
 public:
  enum class Contour {
    TOP = -1,
    BOTTOM = 1,
  };

  Implementation(std::shared_ptr<DecompositionState>, const Component&, Label, Contour);

  std::shared_ptr<DecompositionState> decomposition;
  Component component;
  Label label;
  Contour contour;
};

}  // namespace intervalxt

#endif
