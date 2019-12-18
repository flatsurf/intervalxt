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

#include <vector>

#include "../../intervalxt/length.hpp"

namespace intervalxt {
template <typename T>
std::vector<mpq_class> LengthArithmetic<T>::coefficients(const T& value) {
  return {value};
}

template <typename T>
std::conditional_t<std::is_integral_v<T>, T, mpz_class> LengthArithmetic<T>::floorDivision(const T& divident, const T& divisor) {
  return (divident.get_num() * divisor.get_den()) / (divident.get_den() * divisor.get_num());
}

// Instantiate the above implementation, so that the linker can find it.
template struct LengthArithmetic<mpq_class>;
}
