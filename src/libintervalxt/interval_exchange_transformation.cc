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

#include <gmpxx.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/copied.hpp>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <list>
#include <set>
#include <vector>

#include "intervalxt/interval_exchange_transformation.hpp"
#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

using boost::numeric_cast;
using boost::lexical_cast;
using boost::adaptors::transformed;
using boost::adaptors::copied;
using std::list;
using std::vector;
using std::set;

namespace intervalxt {
namespace {

template <typename Label>
struct Interval {
  Label& label; 
  const typename std::list<Interval<Label>>::iterator twin;
};
}  // namespace

template <typename Label>
class IntervalExchangeTransformation<Label>::Implementation {
  friend IntervalExchangeTransformation<Label>;
  // The labels used by all our intervals (we keep the references alive by adding them to this set.)
  set<Label> labels;
  std::list<Interval<Label>> top, bottom;

 public:
  Implementation(const std::vector<Label>& top, const std::vector<Label>& bottom) : labels(top.begin(), top.end()) {
    assert(top.size() == bottom.size() && "top and bottom must have the same length");

    for (const Label& t : labels)
      this->top.push_back(Interval<Label>{const_cast<Label&>(t), this->bottom.begin()});

    for (const Label& b : bottom) {
      auto it = labels.find(b);
      assert(it != labels.end() && "bottom label not found in top labels");
      this->bottom.push_back(Interval<Label>{const_cast<Label&>(*it), std::find_if(this->top.begin(), this->top.end(), [&](const auto& t) { return t.label == b; })});
    }

    for (auto it = this->bottom.begin(); it != this->bottom.end(); it++) {
      const_cast<typename std::list<Interval<Label>>::iterator&>(it->twin->twin) = it;
    }

    assert(std::all_of(this->top.begin(), this->top.end(), [](auto& t) { return &t == &*t.twin->twin; }) && "twin pointers of top do not point back to the original element.");
    assert(std::all_of(this->bottom.begin(), this->bottom.end(), [](auto& t) { return &t == &*t.twin->twin; }) && "twin pointers of bottom do not point back to the original element.");
    assert(std::all_of(this->labels.begin(), this->labels.end(), [&](auto& l) { return std::count_if(this->top.begin(), this->top.end(), [&](auto& t) { return t.label == l; }); }) && "top does not contain every label exactly once.");
    assert(std::all_of(this->labels.begin(), this->labels.end(), [&](auto& l) { return std::count_if(this->bottom.begin(), this->bottom.end(), [&](auto& t) { return t.label == l; }); }) && "bottom does not contain every label exactly once.");
  }
};

template <typename Label>
IntervalExchangeTransformation<Label>::IntervalExchangeTransformation(const std::vector<Label>& top, const std::vector<size_t>& bottom) : impl(spimpl::make_unique_impl<Implementation>(top, [&]() {
  std::vector<Label> bot;
  for(auto b : bottom) bot.push_back(top[b]);
  return bot;
}())) {}

template <typename Label>
IntervalExchangeTransformation<Label>::IntervalExchangeTransformation(const std::vector<Label>& top, const std::vector<Label>& bottom) : impl(spimpl::make_unique_impl<Implementation>(top, bottom)) {}

template <typename Label>
std::optional<std::pair<const Label&, const Label&>> IntervalExchangeTransformation<Label>::reduce() const {
  std::set<Label> seen;

  int top_ahead = 0;
  int bottom_ahead = 0;

  auto top = impl->top.begin();
  auto bottom = impl->bottom.begin();
  while(true) {
    assert(top != impl->top.end() && "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");
    assert(bottom != impl->bottom.end() && "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");

    if (seen.find(top->label) != seen.end()) {
      bottom_ahead--;
    } else {
      top_ahead++;
      seen.insert(top->label);
    }

    if (seen.find(bottom->label) != seen.end()) {
      top_ahead--;
    } else {
      bottom_ahead++;
      seen.insert(bottom->label);
    }

    if (top_ahead == 0 && bottom_ahead == 0) {
      break;
    }

    ++top;
    ++bottom;
  }

  auto ret = std::make_pair(top->label, bottom->label);
  ++top;
  ++bottom;

  if (top == impl->top.end()) {
    assert(bottom == impl->bottom.end() && "top & bottom do not have the same size");
    return {};
  }

  return ret;
}

template <typename Label>
std::vector<Label> IntervalExchangeTransformation<Label>::top() const noexcept {
  std::vector<Label> ret;

  for (auto& t : impl->top)
    ret.push_back(t.label);

  return ret;
}

template <typename Label>
std::vector<Label> IntervalExchangeTransformation<Label>::bottom() const noexcept {
  std::vector<Label> ret;

  for (auto& t : impl->bottom)
    ret.push_back(t.label);

  return ret;
}

template <typename Label>
void IntervalExchangeTransformation<Label>::zorichInduction(int n) {
  if (n != 1) {
    throw std::logic_error("not implemented: zorichInduction(n!=1)");
  }

  typename Label::Length length_to_subtract;

  const auto top = impl->top.begin();
  auto bottom = impl->bottom.begin();

  assert(top->label != bottom->label && "top and bottom start with the same label");

  while (bottom->label != top->label && length_to_subtract + bottom->label.length() < top->label.length()) {
    length_to_subtract += bottom->label.length();
    ++bottom;
  }

  if (bottom->label == top->label) {
    // Zorich acceleration step (= perform m full Dehn twists)
    auto m = top->label.length() / length_to_subtract;

    for (auto b = impl->bottom.begin(); b != bottom; ++b) {
      top->label.subtract(b->label, m);
    }

    // recompute from what is left
    bottom = impl->bottom.begin();
    length_to_subtract = typename Label::Length();
    while (length_to_subtract + bottom->label.length() < top->label.length()) {
      length_to_subtract += bottom->label.length();
      ++bottom;
    }
  }

  // partial twist
  for (auto b = impl->bottom.begin(); b != bottom; ++b) {
    top->label.subtract(b->label);
  }

  impl->bottom.splice(top->twin, impl->bottom, impl->bottom.begin(), bottom);
}

template <typename Label>
void IntervalExchangeTransformation<Label>::swap() {
  std::swap(impl->top, impl->bottom);
}

template <typename Label>
std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label>& self) {
  auto id = [&](const Label& query) {
    std::string ret = boost::lexical_cast<std::string>(query);
    int clashes = 0;
    for (auto& t : self.top()) {
      std::string repr = boost::lexical_cast<std::string>(t);
      if (t == query) {
        if (clashes) {
          return ret + boost::lexical_cast<std::string>(clashes);
        } else {
          return ret;
        }
      }
      if (repr == ret) {
        clashes++;
      }
    }
    assert(false && "each label must be contained in top()");
  };

  os << join(self.top() | transformed(id), " ") <<std::endl;
  os << join(self.bottom() | transformed(id), " ") <<std::endl;

  return os;
}

}  // namespace intervalxt

// Explicit instantiations of templates so that code is generated for the linker.
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/rational_field_traits.hpp>
#include <exact-real/number_field_traits.hpp>

namespace intervalxt {
template class IntervalExchangeTransformation<Label<long long>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<long long>>&);
template class IntervalExchangeTransformation<Label<int>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<int>>&);

template class IntervalExchangeTransformation<Label<mpz_class>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<mpz_class>>&);

template class IntervalExchangeTransformation<Label<eantic::renf_elem_class>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<eantic::renf_elem_class>>&);

template class IntervalExchangeTransformation<Label<exactreal::Element<exactreal::IntegerRingTraits>>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<exactreal::Element<exactreal::IntegerRingTraits>>>&);
template class IntervalExchangeTransformation<Label<exactreal::Element<exactreal::RationalFieldTraits>>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<exactreal::Element<exactreal::RationalFieldTraits>>>&);
template class IntervalExchangeTransformation<Label<exactreal::Element<exactreal::NumberFieldTraits>>>;
template std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Label<exactreal::Element<exactreal::NumberFieldTraits>>>&);
}
