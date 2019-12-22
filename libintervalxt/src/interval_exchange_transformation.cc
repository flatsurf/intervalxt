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

#include <algorithm>
#include <list>
#include <unordered_set>
#include <valarray>

#include <gmpxx.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/transform.hpp>

#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/induction_step.hpp"

#include "impl/interval_exchange_transformation.impl.hpp"
#include "impl/rational_linear_subspace.hpp"
#include "impl/interval.hpp"

#include "util/assert.ipp"

using std::vector;
using ranges::views::zip;
using ranges::views::transform;
using ranges::to;

namespace intervalxt {

namespace {

template <typename T>
std::valarray<T> wedge(std::valarray<T> v1, std::valarray<T> v2) {
  if (v1.size() != v2.size())
    throw std::logic_error("vectors must have same size");

  size_t d = v1.size();
  std::valarray<T> res(d * (d - 1) / 2);
  if (d == 0) return res;
  size_t k = 0;
  for (size_t i = 0; i < d - 1; i++)
    for (size_t j = i + 1; j < d; j++) {
      res[k] += v1[i] * v2[j] - v1[j] * v2[i];
      k += 1;
    }

  return res;
}

}

bool IntervalExchangeTransformation::zorichInduction(void) {
  const auto top = begin(impl->top);
  auto bottom = begin(impl->bottom);
  auto& lengths = *impl->lengths;

  if (*top == *bottom)
    // The IET starts with a Cylinder
    return true;

  while(true) {
    if (*bottom == *top) {
      // Zorich acceleration step: perform m full Dehn twists
      // and a partial twist.
      Label stop = lengths.subtractRepeated(*top);
      while (*bottom != stop) bottom--;

      bottom++;
      break;
    }
    
    lengths.push(*bottom);
    if (lengths.cmp(*top) >= 0) {
      lengths.pop();

      // partial twist
      lengths.subtract(*top);

      break;
    }

    bottom++;
  }

  impl->bottom.splice(top->twin, impl->bottom, begin(impl->bottom), bottom);

  return lengths.cmp(*begin(impl->top), *begin(impl->bottom)) == 0;
}

std::valarray<mpq_class> IntervalExchangeTransformation::safInvariant() const {
  if (impl->degree == 1) {
    // empty vector for integers or rationals
    return std::valarray<mpq_class>();
  } else {
    std::valarray<mpq_class> w;

    w.resize(impl->degree * (impl->degree - 1) / 2);

    for (auto& i : impl->top) {
      w += wedge(impl->coefficients(i), impl->translation(i));
    }

    return w;
  }
}

bool IntervalExchangeTransformation::boshernitzanNoPeriodicTrajectory() const {
  if (impl->degree <= 1) {
    return false;
  } else {
    // Build the QQ-module of relations between translations, that is the space generated
    // by integer vectors (a0, ..., an) such that a0 t0 + a1 t1 + ... + an tn = 0
    // Note: it would be nice to access the dimension of the module
    // in some more straightforward way...
    vector<vector<mpq_class>> translations(impl->degree);
    for (auto& l : impl->top) {
      const auto t = impl->translation(l);
      for(size_t d = 0; d < impl->degree; d++)
        translations[d].push_back(t[d]);
    }
    return not RationalLinearSubspace::fromEquations(translations).hasNonZeroNonNegativeVector();
  }
}

InductionStep IntervalExchangeTransformation::induce(int limit) {
  using Result = InductionStep::Result;

  if (size() == 1) {
    return { Result::CYLINDER };
  }

  bool foundSaddleConnection = false;

  for (int i = 0; limit == -1 || i < limit; i++) {
    foundSaddleConnection = zorichInduction();
    if (foundSaddleConnection) break;

    swap();
    foundSaddleConnection = zorichInduction();
    swap();
    if (foundSaddleConnection) break;
  }

  const Interval firstTop = *begin(impl->top);
  const Interval firstBottom = *begin(impl->bottom);

  auto reducible = reduce();
  if (reducible) {
    return {
        Result::SEPARATING_CONNECTION,
        std::pair(rbegin(impl->bottom)->label, rbegin(impl->top)->label),
        std::move(*reducible)};
  }

  if (impl->lengths->cmp(firstTop, firstBottom) == 0) {
    auto connection = std::pair(begin(impl->bottom)->label, begin(impl->top)->label);

    // We merge the labels on top and bottom by
    // replacing the top one with the bottom one.
    auto bottomReplacement = impl->bottom.insert(firstTop.twin, firstBottom);
    impl->bottom.erase(begin(impl->bottom));

    // Fix the twin pointer of the bottom's twin to point to its new location.
    bottomReplacement->twin->twin = bottomReplacement;

    // Now drop the top label from top and bottom
    impl->top.erase(begin(impl->top));
    impl->bottom.erase(firstTop.twin);

    return {
        Result::NON_SEPARATING_CONNECTION,
        connection,
    };
  }

  ASSERT(!foundSaddleConnection, "Zorich Induction found a Saddle Connection in " << *this << " but induce() failed to see it.");

  if (boshernitzanNoPeriodicTrajectory()) {
    return { Result::WITHOUT_PERIODIC_TRAJECTORY };
  }

  return { Result::LIMIT_REACHED };
}

IntervalExchangeTransformation::IntervalExchangeTransformation() :
  IntervalExchangeTransformation(std::shared_ptr<Lengths>(nullptr), vector<Label>(), vector<Label>()) {}

IntervalExchangeTransformation::IntervalExchangeTransformation(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  impl(spimpl::make_unique_impl<Implementation>(std::move(lengths), top, bottom)) {}

vector<Label> IntervalExchangeTransformation::top() const noexcept {
  return impl->top | transform([](auto& i) { return i.label; }) | to<vector<Label>>();
}

vector<Label> IntervalExchangeTransformation::bottom() const noexcept {
  return impl->bottom | transform([](auto& i) { return i.label; }) | to<vector<Label>>();
}

void IntervalExchangeTransformation::swap() {
  impl->top.swap(impl->bottom);
  impl->swap = not impl->swap;
}

bool IntervalExchangeTransformation::swapped() const noexcept {
  return impl->swap;
}

size_t IntervalExchangeTransformation::size() const noexcept {
  return impl->top.size();
}

std::optional<IntervalExchangeTransformation> IntervalExchangeTransformation::reduce() {
  // NOTE: using a set here is algorithmically worse than the bitarray that
  // used to be. We know exactly how many labels we have to fit. Instead of
  // constant time searches and additions we have here logarithmic search and
  // addition...then again many consider logarithms just big constants.
  std::unordered_set<Label> seen;

  int top_ahead = 0;
  int bottom_ahead = 0;

  auto topIterator = begin(impl->top);
  auto bottomIterator = begin(impl->bottom);
  while (true) {
    ASSERT(topIterator != end(impl->top), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");
    ASSERT(bottomIterator != end(impl->bottom), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");

    if (seen.find(*topIterator) != end(seen)) {
      bottom_ahead--;
    } else {
      top_ahead++;
      seen.insert(*topIterator);
    }

    if (seen.find(*bottomIterator) != end(seen)) {
      top_ahead--;
    } else {
      bottom_ahead++;
      seen.insert(*bottomIterator);
    }

    if (top_ahead == 0 && bottom_ahead == 0) {
      break;
    }

    ++topIterator;
    ++bottomIterator;
  }

  ++topIterator;
  ++bottomIterator;

  if (topIterator == end(impl->top)) {
    ASSERT(bottomIterator == end(impl->bottom), "top & bottom do not have the same size");
    return {};
  } else {
    vector<Label> newComponentTop;
    vector<Label> newComponentBottom;

    for (; topIterator != end(impl->top) ; topIterator = impl->top.erase(topIterator))
      newComponentTop.push_back(topIterator->label);
    for (; bottomIterator != end(impl->bottom) ; bottomIterator = impl->bottom.erase(bottomIterator))
      newComponentBottom.push_back(bottomIterator->label);

    ASSERT(impl->top.size() == impl->bottom.size(), "top and bottom must have the same length after splitting of a component");

    return IntervalExchangeTransformation(impl->lengths, newComponentTop, newComponentBottom);
  }
}

bool IntervalExchangeTransformation::operator==(const IntervalExchangeTransformation& rhs) const {
  const std::vector<Label> labels = impl->top | transform([](const auto& interval) { return interval.label; }) | to<std::vector<Label>>;
  if (impl->top != rhs.impl->top || impl->bottom != rhs.impl->bottom)
    return false;

  for (auto label : impl->top)
    if (impl->lengths->get(label) != rhs.impl->lengths->get(label))
      return false;

  return true;
}

Implementation<IntervalExchangeTransformation>::Implementation(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  top(top | transform([](const Label label) { return Interval(label); }) | to<std::list<Interval>>()),
  bottom(bottom | transform([](const Label label) { return Interval(label); }) | to<std::list<Interval>>()),
  lengths(std::move(lengths)),
  degree(top.size() == 0 ? 0 : this->lengths->coefficients(*top.begin()).size()) {
  ASSERT(top.size() == bottom.size(), "top and bottom must have the same length");
  ASSERT(top.size() != 0, "IntervalExchangeTransformation cannot be empty");

  ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == std::unordered_set<Label>(begin(bottom), end(bottom)).size(), "top and bottom must consist of the same labels");

  ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == top.size(), "top and bottom must not contain duplicates");

  for (auto t = begin(this->top); t != end(this->top); t++) {
    for (auto b = begin(this->bottom); b != end(this->bottom); b++) {
      if (t->label == b->label) {
        t->twin = b;
        b->twin = t;
      }
    }
  }

  ASSERT(std::all_of(top.begin(), top.end(), [&](Label label) { return this->lengths->coefficients(label).size() == degree; }), "Degrees of elements over Q do not match.");
}

std::valarray<mpq_class> Implementation<IntervalExchangeTransformation>::coefficients(Label label) const {
  auto coefficients = lengths->coefficients(label);
  ASSERT(degree == coefficients.size(), "All Lengths must have the same number of Rational Coefficients but " << lengths->get(label) << " reported " << coefficients.size() << " instead of " << degree);
  return std::valarray<mpq_class>(coefficients.data(), coefficients.size());
}

// Return the translation vector for the label i The output is a vector of
// mpq_class with respect to the irrational basis used for the lengths of
// the iet.
std::valarray<mpq_class> Implementation<IntervalExchangeTransformation>::translation(Label i) const {
  std::valarray<mpq_class> translation(degree);

  for (auto& j : top) {
    if (j == i) break;
    translation -= coefficients(j);
  }

  for (auto& j : bottom) {
    if (j == i) break;
    translation += coefficients(j);
  }

  return translation;
}

IntervalExchangeTransformation Implementation<IntervalExchangeTransformation>::withLengths(const IntervalExchangeTransformation& iet, const std::function<std::shared_ptr<Lengths>(std::shared_ptr<Lengths>)>& createLengths) {
  return IntervalExchangeTransformation(createLengths(iet.impl->lengths), iet.top(), iet.bottom());
}

std::string Implementation<IntervalExchangeTransformation>::render(const IntervalExchangeTransformation& iet, Label label) {
  return iet.impl->lengths->render(label);
}

std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation& self) {
  return os << boost::algorithm::join(self.impl->top | transform([&](const auto& interval) {
      return "[" + self.impl->lengths->render(interval) + ": " + boost::lexical_cast<std::string>(self.impl->lengths->get(interval)) + "]";
    }), " ") << " / " << boost::algorithm::join(self.impl->bottom | transform([&](const auto& interval){
      return "[" + self.impl->lengths->render(interval) + "]";
    }), " ");
}

}  // namespace intervalxt
