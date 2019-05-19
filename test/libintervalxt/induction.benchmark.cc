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

#include <intervalxt/iet.hpp>

using namespace intervalxt;

template <class T>
void Induction5(benchmark::State& state) {
  IntervalExchangeTransformation<unsigned long, unsigned long> iet(5);
  Permutation botperm(5);
  Permutation topperm(5);

  // This is not a meaningful benchmark. We just wanted to try the benchmark
  // interface out here.
  for (auto _ : state) {
    // 0 1 2 3 4   lengths = 977 351 143 321 12
    // 3 2 0 4 1
    botperm[0] = 3;
    botperm[1] = 2;
    botperm[2] = 0;
    botperm[3] = 4;
    botperm[4] = 1;
    topperm[0] = 0;
    topperm[1] = 1;
    topperm[2] = 2;
    topperm[3] = 3;
    topperm[4] = 4;

    iet.setTop(topperm);
    iet.setBot(botperm);

    std::vector<unsigned long> l0(5);
    l0[0] = 977;
    l0[1] = 351;
    l0[2] = 143;
    l0[3] = 321;
    l0[4] = 12;
    iet.setLengths(l0);

    std::vector<unsigned long> l(l0);

    l[0] = 49;

    iet.check();
    iet.zorichInductionStep();
    iet.check();
  }
}
BENCHMARK_TEMPLATE(Induction5, unsigned long);

#include "main.hpp"
