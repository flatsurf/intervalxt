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

#include <benchmark/benchmark.h>

#include <iostream>

#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/length.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/integer_floor_division.hpp"
#include "../intervalxt/sample/lengths.hpp"

using namespace intervalxt;

namespace intervalxt::test {

template <class T>
void Induction(benchmark::State& state) {
  using TLengths = sample::Lengths<T>;

  const int LIMIT = static_cast<int>(state.range(0));

  for (auto _ : state) {
    for (int a = 1; a < LIMIT - 6; a++) {
      for (int b = 1; b < LIMIT - a - 5; b++) {
        for (int c = 1; c < LIMIT - a - b - 4; c++) {
          for (int d = 1; d < LIMIT - a - b - c - 3; d++) {
            for (int e = 1; e < LIMIT - a - b - c - d - 2; e++) {
              for (int f = 1; f < LIMIT - a - b - c - d - e - 1; f++) {
                int g = LIMIT - a - b - c - d - e - f;
                assert(g > 0);

                auto&& [lengths, aa, bb, cc, dd, ee, ff, gg] = TLengths::make(a, b, c, d, e, f, g);

                auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {aa, bb, cc, dd, ee, ff, gg}, {gg, ff, ee, dd, cc, bb, aa});
                auto decomposition = DynamicalDecomposition(iet);
                decomposition.decompose();
              }
            }
          }
        }
      }
    }
  }
}
BENCHMARK_TEMPLATE(Induction, int)->Arg(8)->Arg(34);

}  // namespace intervalxt::test
