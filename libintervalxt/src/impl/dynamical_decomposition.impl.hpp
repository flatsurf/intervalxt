/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
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
 *********************************************************************/

#ifndef LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_IMPL_HPP
#define LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_IMPL_HPP

#include <deque>
#include <memory>

#include "../../intervalxt/dynamical_decomposition.hpp"
#include "decomposition_state.hpp"

namespace intervalxt {

template <>
class ImplementationOf<DynamicalDecomposition> {
 public:
  ImplementationOf();
  ImplementationOf(const ImplementationOf&) = delete;
  ImplementationOf(ImplementationOf&&) = delete;

  ImplementationOf& operator=(const ImplementationOf&) = delete;
  ImplementationOf& operator=(ImplementationOf&&) = delete;

  static Component insertComponent(DynamicalDecomposition&, Component& left, DecompositionState::Connection, IntervalExchangeTransformation&& right);
  static Component insertComponent(DynamicalDecomposition&, IntervalExchangeTransformation&&);

  static void check(const DynamicalDecomposition&);

  static ImplementationOf& self(DynamicalDecomposition&);
  static const ImplementationOf& self(const DynamicalDecomposition&);

  DecompositionState decomposition;
};

}  // namespace intervalxt

#endif
