/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2025 Julian Rüth
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

#ifndef LIBINTERVALXT_SAMPLE_DETAIL_LENGTHS_IPP
#define LIBINTERVALXT_SAMPLE_DETAIL_LENGTHS_IPP

#include <cassert>
#include <numeric>
#include <vector>

#include "../../label.hpp"
#include "../coefficients.hpp"
#include "../floor_division.hpp"
#include "../lengths.hpp"

namespace intervalxt::sample {

namespace {

template <typename T, std::size_t... Indices>
auto toTuple(T&& container, std::index_sequence<Indices...>) { return std::make_tuple(container.at(Indices)...); }

template <typename T>
size_t index(Label label) {
  return std::hash<Label>()(label);
}

template <typename T>
int cmp(const T& lhs, const T& rhs) {
  if (lhs < rhs) return -1;
  if (lhs > rhs) return 1;
  return 0;
}

template <typename T, typename FloorDivision, typename Coefficients>
Lengths<T, FloorDivision, Coefficients>::Lengths() :
  stack(),
  lengths() {}

template <typename T, typename FloorDivision, typename Coefficients>
Lengths<T, FloorDivision, Coefficients>::Lengths(const std::vector<T>& lengths) :
  stack(),
  lengths(lengths) {
  if (std::any_of(begin(lengths), end(lengths), [](const auto& length) { return length < 0; }))
    throw std::invalid_argument("all lengths must be non-negative");
}

template <typename T, typename FloorDivision, typename Coefficients>
template <typename... L>
auto Lengths<T, FloorDivision, Coefficients>::make(L&&... values) {
  auto lengths = Lengths<T>(std::vector{values...});
  return std::tuple_cat(
      std::make_tuple(lengths),
      toTuple(lengths.labels(), std::make_index_sequence<sizeof...(L)>()));
}

template <typename T, typename FloorDivision, typename Coefficients>
std::vector<Label> Lengths<T, FloorDivision, Coefficients>::labels() const {
  std::vector<Label> labels;
  for (int i = 0; i < lengths.size(); i++) labels.push_back(Label(i));
  return labels;
}

template <typename T, typename FloorDivision, typename Coefficients>
Lengths<T, FloorDivision, Coefficients>::operator T() const {
  return std::accumulate(begin(stack), end(stack), T(), [&](T value, Label label) { return value + at(label); });
}

template <typename T, typename FloorDivision, typename Coefficients>
T Lengths<T, FloorDivision, Coefficients>::get(Label label) const {
  return at(label);
}

template <typename T, typename FloorDivision, typename Coefficients>
const T& Lengths<T, FloorDivision, Coefficients>::at(Label label) const {
  return lengths.at(index<T>(label));
}

template <typename T, typename FloorDivision, typename Coefficients>
T& Lengths<T, FloorDivision, Coefficients>::at(Label label) {
  return lengths.at(index<T>(label));
}

template <typename T, typename FloorDivision, typename Coefficients>
void Lengths<T, FloorDivision, Coefficients>::push(Label label) {
  stack.push_back(label);
}

template <typename T, typename FloorDivision, typename Coefficients>
void Lengths<T, FloorDivision, Coefficients>::pop() {
  stack.pop_back();
}

template <typename T, typename FloorDivision, typename Coefficients>
void Lengths<T, FloorDivision, Coefficients>::clear() {
  stack.clear();
}

template <typename T, typename FloorDivision, typename Coefficients>
int Lengths<T, FloorDivision, Coefficients>::cmp(Label rhs) const {
  return ::intervalxt::sample::cmp<T>(static_cast<T>(*this), at(rhs));
}

template <typename T, typename FloorDivision, typename Coefficients>
int Lengths<T, FloorDivision, Coefficients>::cmp(Label lhs, Label rhs) const {
  return ::intervalxt::sample::cmp<T>(at(lhs), at(rhs));
}

template <typename T, typename FloorDivision, typename Coefficients>
void Lengths<T, FloorDivision, Coefficients>::subtract(Label from) {
  at(from) -= static_cast<T>(*this);
  assert(at(from) > 0 && "all lengths must be positive.");
  clear();
}

template <typename T, typename FloorDivision, typename Coefficients>
Label Lengths<T, FloorDivision, Coefficients>::subtractRepeated(Label from) {
  if (stack.size() == 0)
    throw std::invalid_argument("Cannot subtractRepeated() without push()");

  auto quo = FloorDivision()(at(from), static_cast<T>(*this));

  at(from) -= quo * static_cast<T>(*this);

  assert(at(from) >= 0 && "Length cannot be negative.");
  if (at(from) == 0) {
    // Undo the last subtraction
    at(from) += static_cast<T>(*this);
  }

  Label stop = *rbegin(stack);
  for (Label label : stack) {
    if (at(label) >= at(from)) {
      clear();
      return stop;
    }

    stop = label;
    at(from) -= at(label);
    assert(at(from) > 0 && "all lengths must be positive.");
  }

  throw std::logic_error("Floor Division inconsistent with cmp()/subtract()");
}

template <typename T, typename FloorDivision, typename Coefficients>
std::vector<std::vector<mpq_class>> Lengths<T, FloorDivision, Coefficients>::coefficients(const std::vector<Label>& labels) const {
  std::vector<T> lengths;
  for (auto& label : labels)
    lengths.push_back(at(label));
  return Coefficients()(lengths);
}

template <typename T, typename FloorDivision, typename Coefficients>
std::string Lengths<T, FloorDivision, Coefficients>::render(Label label) const {
  std::string ret;
  size_t current = index<T>(label);
  while (current || ret.size() == 0) {
    size_t offset = current % (2u * 26u);
    if (offset < 26) {
      ret += static_cast<char>('a' + offset);
    } else {
      ret += static_cast<char>('A' + (offset - 26));
    }
    current /= (2 * 26);
  }
  return ret;
}

template <typename T, typename FloorDivision, typename Coefficients>
::intervalxt::Lengths Lengths<T, FloorDivision, Coefficients>::only(const std::unordered_set<Label>& labels) const {
  auto only = Lengths(lengths);
  for (const auto label : this->labels())
    if (labels.find(label) == labels.end())
      only.at(label) = T();
  return only;
}

template <typename T, typename FloorDivision, typename Coefficients>
::intervalxt::Lengths Lengths<T, FloorDivision, Coefficients>::forget() const {
  return Lengths(lengths);
}

template <typename T, typename FloorDivision, typename Coefficients>
bool Lengths<T, FloorDivision, Coefficients>::operator==(const Lengths& other) const {
  return lengths == other.lengths;
}

template <typename T, typename FloorDivision, typename Coefficients>
bool Lengths<T, FloorDivision, Coefficients>::similar(Label a, Label b, const ::intervalxt::Lengths& other, Label aa, Label bb) const {
  const auto& x = at(a);
  const auto otherx = other.get(aa);

  if (!x && !otherx)
    return true;

  const auto& y = at(b);
  const auto othery = other.get(bb);

  if (!y && !othery)
    return true;

  return x * othery == y * otherx;
}

}  // namespace

}  // namespace intervalxt::sample

#endif
