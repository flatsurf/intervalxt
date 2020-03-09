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

#ifndef LIBINTERVALXT_DETAIL_LENGTH_IPP
#define LIBINTERVALXT_DETAIL_LENGTH_IPP

#include <cassert>
#include <ostream>
#include <variant>
#include <vector>

#include <boost/numeric/conversion/cast.hpp>

#include "../external/gmpxxll/mpz_class.hpp"
#include "../length.hpp"

namespace intervalxt {
template <typename T>
Length<T>::Length() :
  Length(T()) {}

template <typename T>
Length<T>::Length(const T& value) :
  value(value) {
  assert(value >= 0 && "a length cannot be negative");
}

template <typename T>
template <typename S, typename std::enable_if_t<std::is_convertible_v<T, S>, bool>>
Length<T>::operator Length<S>() const {
  return Length<S>(value);
}

template <typename T>
template <typename S, typename std::enable_if_t<std::is_constructible_v<S, T>, bool>>
Length<T>::operator Length<S>() const {
  return Length<S>(S(value));
}

template <typename T>
template <typename S, typename std::enable_if_t<!std::is_constructible_v<S, T> && std::is_integral_v<T> && std::is_constructible_v<S, mpz_class>, bool>>
Length<T>::operator Length<S>() const {
  return Length<S>(S(gmpxxll::mpz_class(value)));
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
Length<T>& Length<T>::operator*=(const typename LengthArithmetic<T>::QuotientFloorDivision& rhs) noexcept {
  value *= rhs;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T>
typename LengthArithmetic<T>::QuotientFloorDivision Length<T>::operator/(const Length<T>& rhs) {
  return LengthArithmetic<T>::floorDivision(value, rhs.value);
}

template <typename T>
std::vector<mpq_class> Length<T>::coefficients() const {
  return LengthArithmetic<T>::coefficients(value);
}

template <typename T>
Length<T>::operator bool() const noexcept {
  return static_cast<bool>(value);
}

template <typename T>
bool Length<T>::operator==(const T& rhs) const noexcept {
  return value == rhs;
}

template <typename T>
bool Length<T>::operator<(const T& rhs) const noexcept {
  return value < rhs;
}

template <typename T>
bool Length<T>::operator>(const T& rhs) const noexcept {
  return value > rhs;
}

template <typename T>
bool Length<T>::operator==(const std::pair<T, T>& rhs) const noexcept {
  return value * value == rhs.first * rhs.first + rhs.second * rhs.second;
}

template <typename T>
bool Length<T>::operator<(const std::pair<T, T>& rhs) const noexcept {
  return value * value < rhs.first * rhs.first + rhs.second * rhs.second;
}

template <typename T>
bool Length<T>::operator>(const std::pair<T, T>& rhs) const noexcept {
  return value * value > rhs.first * rhs.first + rhs.second * rhs.second;
}

template <typename T>
bool Length<T>::operator==(const Length<T>& rhs) const noexcept {
  return value == rhs.value;
}

template <typename T>
bool Length<T>::operator<(const Length<T>& rhs) const noexcept {
  return value < rhs.value;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Length<T>& self) {
  return os << self.value;
}
}  // namespace intervalxt

#endif
