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

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/length.hpp>
#include <intervalxt/label.hpp>

using namespace intervalxt;

template <class T>
void Induction5(benchmark::State& state) {
  using Length = Length<T>;

  // This is not a meaningful benchmark. We just wanted to try the benchmark
  // interface out here.
  for (auto _ : state) {
    IntervalExchangeTransformation<Length> iet({Length(997), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1});

    iet.zorichInduction();
  }
}
BENCHMARK_TEMPLATE(Induction5, int);

#include "main.hpp"
