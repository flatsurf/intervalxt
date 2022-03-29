/**********************************************************************
 *  This file is part of intervalxt.
 *
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

// Include this header to get {fmt} support for all types define by this library.

// Note that we would have liked to give {fmt} first class support and deduce
// ostream<< from this. However, this is not possible with the encapsulation
// that we are presently using: Since a Formatter's format() is templatized, it
// cannot be in a .cc file. Of course printing should not use any private
// information that is not readily available otherwise, but we do not want
// implementations in header files and also, this would mean that every header
// has to pull in <fmt/core.h> or we'd need some other header scheme for this.

#ifndef LIBINTERVALXT_FMT_HPP
#define LIBINTERVALXT_FMT_HPP

#include <fmt/core.h>

#include <sstream>

#include "length.hpp"

namespace intervalxt {

template <typename T>
struct GenericFormatter {
  constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const T& value, FormatContext& ctx) {
    std::ostringstream ss;
    ss << value;
    return fmt::format_to(ctx.out(), "{}", ss.str());
  }
};

}  // namespace intervalxt

template <>
struct fmt::formatter<::intervalxt::IntervalExchangeTransformation> : ::intervalxt::GenericFormatter<::intervalxt::IntervalExchangeTransformation> {};
template <>
struct fmt::formatter<::intervalxt::DynamicalDecomposition> : ::intervalxt::GenericFormatter<::intervalxt::DynamicalDecomposition> {};
template <>
struct fmt::formatter<::intervalxt::Component> : ::intervalxt::GenericFormatter<::intervalxt::Component> {};
template <>
struct fmt::formatter<::intervalxt::HalfEdge> : ::intervalxt::GenericFormatter<::intervalxt::HalfEdge> {};
template <>
struct fmt::formatter<::intervalxt::Separatrix> : ::intervalxt::GenericFormatter<::intervalxt::Separatrix> {};
template <>
struct fmt::formatter<::intervalxt::DecompositionStep> : ::intervalxt::GenericFormatter<::intervalxt::DecompositionStep> {};
template <>
struct fmt::formatter<::intervalxt::InductionStep> : ::intervalxt::GenericFormatter<::intervalxt::InductionStep> {};
template <>
struct fmt::formatter<::intervalxt::Connection> : ::intervalxt::GenericFormatter<::intervalxt::Connection> {};
template <>
struct fmt::formatter<::intervalxt::Side> : ::intervalxt::GenericFormatter<::intervalxt::Side> {};
template <>
struct fmt::formatter<::intervalxt::Length> : ::intervalxt::GenericFormatter<::intervalxt::Length> {};
template <>
struct fmt::formatter<::intervalxt::Label> : ::intervalxt::GenericFormatter<::intervalxt::Label> {};

#endif
