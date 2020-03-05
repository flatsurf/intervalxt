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

#include "../intervalxt/label.hpp"

#include "impl/decomposition_state.hpp"
#include "impl/forward.hpp"
#include "impl/lengths_with_connections.hpp"

#include "util/assert.ipp"

namespace intervalxt {

LengthsWithConnections::LengthsWithConnections(std::shared_ptr<Lengths> lengths, std::shared_ptr<DecompositionState> decomposition) : lengths(lengths),
                                                                                                                                      decomposition(decomposition) {}

void LengthsWithConnections::push(Label label) {
  ASSERT(std::find(begin(stack), end(stack), label) == end(stack), "label cannot be pushed more than once");
  stack.push_back(label);
  lengths->push(label);
}

void LengthsWithConnections::pop() {
  ASSERT(not stack.empty(), "cannot pop from an empty stack of labels");
  stack.pop_back();
  lengths->pop();
}

void LengthsWithConnections::subtract(Label minuend) {
  if (not stack.empty()) {
    lengths->subtract(minuend);

    subtract(minuend, *begin(stack));
    stack.clear();
  }
}

void LengthsWithConnections::subtract(Label minuend, Label subtrahend) {
  // Normalize top and bottom such that the minuend is on top and the
  // subtrahend on the bottom. Note that this search is very inefficient, see
  // #71.
  bool minuendOnTop = std::any_of(begin(decomposition->components), end(decomposition->components),
                                  [&](const auto& component) { return *begin(component.iet.top()) == minuend && !component.iet.swapped(); });

  auto& top = minuendOnTop ? decomposition->top : decomposition->bottom;
  auto& bottom = minuendOnTop ? decomposition->bottom : decomposition->top;

  // The subtrahend takes the minuends top (left) list with it (this might be a
  // nop since bottom and top share this initial list of connections.)
  auto& bottomSubtrahend = bottom.at(subtrahend).left;

  if (minuendOnTop)
    bottomSubtrahend.splice(end(bottomSubtrahend), top.at(minuend).left);
  else {
    bottomSubtrahend.splice(begin(bottomSubtrahend), top.at(minuend).left);
  }

  // The subtrahend takes the minuends bottom left list.
  bottomSubtrahend.splice(
      minuendOnTop ? end(bottomSubtrahend) : begin(bottomSubtrahend),
      bottom.at(minuend).left);
}

Label LengthsWithConnections::subtractRepeated(Label minuend) {
  ASSERT(not stack.empty(), "cannot subtract nothing repeatedly as no return value can be determined");

  Label ret = lengths->subtractRepeated(minuend);

  subtract(minuend, *begin(stack));
  stack.clear();

  return ret;
}

std::vector<mpq_class> LengthsWithConnections::coefficients(Label label) const {
  return lengths->coefficients(label);
}

int LengthsWithConnections::cmp(Label label) const {
  return lengths->cmp(label);
}

int LengthsWithConnections::cmp(Label lhs, Label rhs) const {
  return lengths->cmp(lhs, rhs);
}

Length LengthsWithConnections::get(Label label) const {
  return lengths->get(label);
}

std::string LengthsWithConnections::render(Label label) const {
  return lengths->render(label);
}

Lengths LengthsWithConnections::only(const std::unordered_set<Label>& labels) const {
  return forget().only(labels);
}

Lengths LengthsWithConnections::forget() const {
  return lengths->forget();
}

bool LengthsWithConnections::similar(Label a, Label b, const ::intervalxt::Lengths& other, Label aa, Label bb) const {
  return forget().similar(a, b, other, aa, bb);
}

}  // namespace intervalxt
