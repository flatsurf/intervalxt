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

#ifndef LIBINTERVALXT_SEPARATRIX_IMPL_HPP
#define LIBINTERVALXT_SEPARATRIX_IMPL_HPP

#include <memory>

#include "../../intervalxt/separatrix.hpp"
#include "implementation_of_decomposition.hpp"

namespace intervalxt {

template <>
class ImplementationOf<Separatrix> : public ImplementationOfDecomposition {
 public:
  ImplementationOf(const DynamicalDecomposition&, DecompositionState::Separatrix);
  static Separatrix make(const DynamicalDecomposition&, DecompositionState::Separatrix separatrix);

  // Return the top separatrix that connections created right of the label would report now.
  static DecompositionState::Separatrix makeAtTop(const DynamicalDecomposition&, Label);
  // Return the bottom separatrix that connections created right of the label would report now.
  static DecompositionState::Separatrix makeAtBottom(const DynamicalDecomposition&, Label);

  DecompositionState::Separatrix separatrix;
};

template <typename... Args>
Separatrix::Separatrix(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<Separatrix>>(std::forward<Args>(args)...)) {}

}  // namespace intervalxt

#endif
