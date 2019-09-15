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
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "intervalxt/length.hpp"

namespace intervalxt {
template <typename T, typename Quo>
Length<T, Quo>::Length() : Length(T(0)) {}

template <typename T, typename Quo>
Length<T, Quo>::Length(const T& value) : value(value) {
  assert(value >= 0 && "a length cannot be negative");
}

template <typename T, typename Quo>
template <bool enable, std::enable_if_t<enable, int>>
Length<T, Quo>::Length(const std::string& s) {
  if constexpr (std::is_integral_v<T>) {
    value = std::stoi(s);
  } else if constexpr (std::is_same_v<T, mpz_class> || std::is_same_v<T, mpq_class>) {
    value = T(s);
  }
  assert(value >= 0 && "a length cannot be negative");
}

template <typename T, typename Quo>
Length<T, Quo>& Length<T, Quo>::operator+=(const Length<T, Quo>& rhs) noexcept {
  value += rhs.value;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T, typename Quo>
Length<T, Quo>& Length<T, Quo>::operator-=(const Length<T, Quo>& rhs) noexcept {
  value -= rhs.value;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T, typename Quo>
Length<T, Quo>& Length<T, Quo>::operator*=(const Length<T, Quo>::Quotient& rhs) noexcept {
  value *= rhs;
  assert(value >= 0 && "a length cannot be negative");
  return *this;
}

template <typename T, typename Quo>
typename Length<T, Quo>::Quotient Length<T, Quo>::operator/(const Length<T, Quo>& rhs) {
  assert(static_cast<bool>(rhs) && "cannot divide by zero vector");
  if constexpr (std::is_integral_v<T> || std::is_same_v<T, mpz_class>) {
    return value / rhs.value;
  } else if constexpr (std::is_same_v<T, mpq_class>) {
    // NOTE: floor division could be smarter than that!
    mpz_class ret = (value.get_num() * rhs.value.get_den()) / (value.get_den() * rhs.value.get_num());
    return ret;
  } else if constexpr (std::is_same_v<T, eantic::renf_elem_class>) {
    // NOTE: floor division could be smarter than that!
    return (value / rhs.value).floor();
  } else {
    throw std::logic_error("not implemented: floor division for this type");
  }
}

template <typename T, typename Quo>
const T& Length<T, Quo>::length() const {
  return value;
}

template <typename T, typename Quo>
size_t Length<T, Quo>::degree() const {
  if constexpr (std::is_integral_v<T> || std::is_same_v<T, mpz_class> || std::is_same_v<T, mpq_class>) {
    return 1;
  } else if constexpr (std::is_same_v<T, eantic::renf_elem_class>) {
    return value.parent()->degree();
  } else {
    throw std::logic_error("not implemented: no degree for this type");
  }
}

template <typename T, typename Quo>
std::vector<typename Length<T, Quo>::Coefficient> Length<T, Quo>::coefficients() const {
  if constexpr (std::is_integral_v<T>) {
    std::vector<mpq_class> ret;
    if constexpr (std::is_same_v<T, long long>) {
      ret.push_back(mpq_class(boost::lexical_cast<std::string>(value)));
    } else {
      ret.push_back(value);
    }
    return ret;
  } else if constexpr (std::is_same_v<T, mpz_class> || std::is_same_v<T, mpq_class>) {
    std::vector<mpq_class> ret;
    ret.push_back(value);
    return ret;
  } else if constexpr (std::is_same_v<T, eantic::renf_elem_class>) {
    std::vector<mpq_class> ret;
    auto d = value.den();
    for (auto i : value.num_vector()) {
      auto dat = mpq_class(i, d);
      dat.canonicalize();
      ret.push_back(dat);
    }
    return ret;
  } else {
    throw std::logic_error("not implemented; coefficient for non-integral types");
  }
}

template <typename T, typename Quo>
T Length<T, Quo>::squared() const {
  auto ret = value * value;
  assert(ret >= value);
  return ret;
}

template <typename T, typename Quo>
Length<T, Quo>::operator bool() const noexcept {
  return static_cast<bool>(value);
}

template <typename T, typename Quo>
bool Length<T, Quo>::operator<(const Length<T, Quo>& rhs) const noexcept {
  return value < rhs.value;
}

template <typename T, typename Quo>
bool Length<T, Quo>::operator==(const Length<T, Quo>& rhs) const noexcept {
  return value == rhs.value;
}

template <typename T, typename Quo>
std::ostream& operator<<(std::ostream& os, const Length<T, Quo>& self) {
  return os << self.value;
}
}  // namespace intervalxt

// Create instantiations of the template for the linker.

namespace intervalxt {
template class Length<int>;
template Length<int>::Length(const std::string&);
template std::ostream& operator<<(std::ostream&, const Length<int>&);
template class Length<long long>;
template Length<long long>::Length(const std::string&);
template std::ostream& operator<<(std::ostream&, const Length<long long>&);
template class Length<mpz_class>;
template Length<mpz_class>::Length(const std::string&);
template std::ostream& operator<<(std::ostream&, const Length<mpz_class>&);
template class Length<mpq_class>;
template Length<mpq_class>::Length(const std::string&);
template std::ostream& operator<<(std::ostream&, const Length<mpq_class>&);
template class Length<eantic::renf_elem_class>;
template std::ostream& operator<<(std::ostream&, const Length<eantic::renf_elem_class>&);
}  // namespace intervalxt
