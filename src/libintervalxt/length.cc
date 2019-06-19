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

#include <cassert>
#include <variant>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

#include "intervalxt/length.hpp"

namespace intervalxt {
template <typename T>
Length<T>::Length() : Length(0) {}

template <typename T>
Length<T>::Length(const T& value) : value(value) {
  assert(value >= 0 && "a length cannot be negative");
}

template <typename T>
Length<T>& Length<T>::operator+=(const Length<T>& rhs) noexcept {
  value += rhs.value;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T>
Length<T>& Length<T>::operator-=(const Length<T>& rhs) noexcept {
  value -= rhs.value;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T>
Length<T>& Length<T>::operator*=(const mpz_class& rhs) noexcept {
  if constexpr (std::is_integral_v<T>) {
    assert(rhs.fits_sint_p());
    value *= boost::numeric_cast<T>(rhs.get_si());
  } else {
    value *= rhs;
  }
  return *this;
}

template <typename T>
mpz_class Length<T>::operator/(const Length<T>& rhs) {
  assert(static_cast<bool>(rhs) && "cannot divide by zero vector");
  if constexpr (std::is_integral_v<T>) {
    auto ret = value / rhs.value;
    if constexpr (std::is_same_v<T, long long>) {
      return mpz_class(boost::lexical_cast<std::string>(ret));
    } else {
      return ret;
    }
  } else {
    throw std::logic_error("not implemented: floor division for this type");
  }
}

template <typename T>
Length<T>::operator bool() const noexcept {
  return static_cast<bool>(value);
}

template <typename T>
bool Length<T>::operator<(const Length<T>& rhs) const noexcept {
  return value < rhs.value;
}

template <typename T>
bool Length<T>::operator==(const Length<T>& rhs) const noexcept {
  return value == rhs.value;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Length<T>& self) {
  return os << self.value;
}
}  // namespace intervalxt

// Create instantiations of the template for the linker.

namespace intervalxt {
template class Length<int>;
template class Length<long long>;
template std::ostream& operator<<(std::ostream&, const Length<int>&);
}  // namespace intervalxt
