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

#ifndef LIBINTERVALXT_IMPLEMENTATION_WITH_CONTEXT
#define LIBINTERVALXT_IMPLEMENTATION_WITH_CONTEXT

#include "../../intervalxt/dynamical_decomposition.hpp"
#include "decomposition_state.hpp"

namespace intervalxt {

class ImplementationOfDecomposition {
 public:
  ImplementationOfDecomposition(const DynamicalDecomposition& decomposition);

  std::deque<DecompositionState::Component>& components();
  const std::deque<DecompositionState::Component>& components() const;

  DecompositionState::Connections& connections(const Label&);
  const DecompositionState::Connections& connections(const Label&) const;

  ImplementationOf<DynamicalDecomposition>& impl();
  const ImplementationOf<DynamicalDecomposition>& impl() const;

  DecompositionState& state();
  const DecompositionState& state() const;

  std::string render(Label) const;

  DynamicalDecomposition decomposition;
};

}  // namespace intervalxt

#endif
