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

#ifndef LIBINTERVALXT_ERASED_CEREAL_HPP
#define LIBINTERVALXT_ERASED_CEREAL_HPP

#include "serializable.hpp"

#include "detail/cereal.hpp"
#include "detail/unparen.hpp"

namespace intervalxt::erased {

// Register our derived Serializable type with cereal's polymorphism machinery.
// For this to work, users need to have <cereal/types/polymorphic.hpp> included.
// We do not include it here explicitly since we do not want to make
// assumptions on how cereal was included.
// Further note, that we cannot do CEREAL_REGISTER_POLYMORPHIC_RELATION() since
// that cannot handle type names containing commas.
#define LIBINTERVALXT_ERASED_REGISTER(SERIALIZATION, UNERASED) \
  CEREAL_REGISTER_TYPE(::intervalxt::erased::detail::Serializable<typename ::intervalxt::erased::detail::unparen<void UNERASED>::type, ::intervalxt::erased::Serializable<typename ::intervalxt::erased::detail::unparen<void SERIALIZATION>::type>>)

}  // namespace intervalxt::erased

#endif
