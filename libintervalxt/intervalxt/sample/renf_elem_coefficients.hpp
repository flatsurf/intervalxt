/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#ifndef LIBINTERVALXT_SAMPLE_RENF_ELEM_COEFFICIENTS_HPP
#define LIBINTERVALXT_SAMPLE_RENF_ELEM_COEFFICIENTS_HPP

#include <e-antic/renfxx.h>

#include "coefficients.hpp"

namespace intervalxt::sample {

namespace {

template <>
struct Coefficients<eantic::renf_elem_class> {
  using T = eantic::renf_elem_class;

  std::vector<std::vector<mpq_class>> operator()(const std::vector<T>& elements) {
    if (elements.size() == 0)
      return {};

    const auto* parent = &elements[0].parent();
    for (auto& x : elements) {
      if (x.is_rational())
        continue;
      if (parent->degree() == 1)
        parent = &x.parent();
      if (*parent != x.parent())
        throw std::logic_error("not implemented: cannot coerce elements living in different number fields to a common parent yet");
    }

    std::vector<std::vector<mpq_class>> ret;
    for (auto x : elements) {
      x = eantic::renf_elem_class(*parent, x);

      std::vector<mpq_class> coefficients;
      const auto den = x.den();
      for (auto num : x.num_vector()) {
        auto entry = mpq_class(num, den);
        entry.canonicalize();
        coefficients.push_back(entry);
      }

      ret.push_back(coefficients);
    }

    return ret;
  }
};

}  // namespace

}  // namespace intervalxt::sample

#endif
