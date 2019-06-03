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

#include <variant>

#include "intervalxt/length.hpp"

namespace intervalxt {
namespace {
template <typename Coordinate>
struct Linear {
  Coordinate length;

  auto operator/(const Linear& rhs) const {
    return length / rhs.length;
  }
};

template <typename Coordinate>
struct Squared {
  Coordinate x, y;
};
}

template <typename Coordinate>
class Length<Coordinate>::Implementation {
 public:
  Implementation() : length(Linear<Coordinate>()) {}
  Implementation(const Coordinate& length) : length(Linear<Coordinate>{length}) {}

  std::variant<Linear<Coordinate>, Squared<Coordinate>> length;
};

template <typename Coordinate>
Length<Coordinate>::Length() : impl(spimpl::make_impl<Implementation>()) {}

template <typename Coordinate>
Length<Coordinate>::Length(const Coordinate& length) : impl(spimpl::make_impl<Implementation>(length)) {}

template <typename Coordinate>
Length<Coordinate>& Length<Coordinate>::operator+=(const Length<Coordinate>& rhs) noexcept {
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(impl->length)) {
    if (std::holds_alternative<Linear>(rhs.impl->length)) {
      std::get<Linear>(impl->length).length += std::get<Linear>(rhs.impl->length).length;
    } else {
      throw std::logic_error("not implemented: Length::operator+=");
    }
  } else {
    throw std::logic_error("not implemented: Length::operator+=");
  }

  return *this;
}

template <typename Coordinate>
Quotient<Coordinate> Length<Coordinate>::operator/(const Length& rhs) {
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(impl->length)) {
    if (std::holds_alternative<Linear>(rhs.impl->length)) {
      return std::get<Linear>(impl->length) / std::get<Linear>(rhs.impl->length);
    } else {
      throw std::logic_error("not implemented: Length::operator/");
    }
  } else {
    throw std::logic_error("not implemented: Length::operator/");
  }
}

template <typename Coordinate>
Length<Coordinate>& Length<Coordinate>::operator*=(const Quotient<Coordinate>& rhs) noexcept {
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(impl->length)) {
    std::get<Linear>(impl->length).length *= rhs;
  } else {
    throw std::logic_error("not implemented: Length::operator<");
  }
  return *this;
}

template <typename Coordinate>
Length<Coordinate>& Length<Coordinate>::operator-=(const Length<Coordinate>& rhs) noexcept {
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(impl->length)) {
    if (std::holds_alternative<Linear>(rhs.impl->length)) {
      std::get<Linear>(impl->length).length -= std::get<Linear>(rhs.impl->length).length;
    } else {
      throw std::logic_error("not implemented: Length::operator-=");
    }
  } else {
    throw std::logic_error("not implemented: Length::operator-=");
  }

  return *this;
}

template <typename Coordinate>
bool Length<Coordinate>::operator<(const Length<Coordinate>& rhs) const noexcept {
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(impl->length)) {
    if (std::holds_alternative<Linear>(rhs.impl->length)) {
      return std::get<Linear>(impl->length).length < std::get<Linear>(rhs.impl->length).length;
    } else {
      throw std::logic_error("not implemented: Length::operator<");
    }
  } else {
    throw std::logic_error("not implemented: Length::operator<");
  }
}

template <typename Coordinate>
std::ostream& operator<<(std::ostream& os, const Length<Coordinate>& self){
  using Linear = Linear<Coordinate>;
  if (std::holds_alternative<Linear>(self.impl->length)) {
    return os << std::get<Linear>(self.impl->length).length;
  } else {
    throw std::logic_error("not implemented: operator<< for Length");
  }
}
}  // namespace intervalxt

// Explicit instantiations of templates so that code is generated for the linker.
#include <gmpxx.h>

namespace intervalxt {
template class Length<int>;
template std::ostream& operator<<(std::ostream&, const Length<int>&);
template class Length<mpz_class>;
template std::ostream& operator<<(std::ostream&, const Length<mpz_class>&);
}
