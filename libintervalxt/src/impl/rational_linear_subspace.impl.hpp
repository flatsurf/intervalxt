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

#ifndef LIBINTERVALXT_RATIONAL_LINEAR_SUBSPACE_IMPL_HPP
#define LIBINTERVALXT_RATIONAL_LINEAR_SUBSPACE_IMPL_HPP

#include <ppl.hh>

#include "rational_linear_subspace.hpp"

using Parma_Polyhedra_Library::NNC_Polyhedron;

namespace intervalxt {

template<>
class Implementation<RationalLinearSubspace> {
 public:
  Implementation();

  Implementation(const std::vector<std::vector<mpq_class>>& vectors, bool equations);

  NNC_Polyhedron subspace;
  NNC_Polyhedron positive;
  NNC_Polyhedron nonNegative;
};

}

#endif
