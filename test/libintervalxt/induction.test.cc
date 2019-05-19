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

#include <gtest/gtest.h>
#include <vector>

#include <intervalxt/interval_exchange_transformation.hpp>

using namespace intervalxt;

namespace {

using v = std::vector<int>;

TEST(InductionTest, Induction2) {
  IntervalExchangeTransformation<int, int> iet(2);
  iet.setBot({1, 0});
  iet.setLengths({23, 5});

  iet.check();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{23 - 4 * 5, 5}));
}

TEST(InductionTest, Induction5) {
  IntervalExchangeTransformation<int, int> iet(5);
  Permutation botperm(5);
  Permutation topperm(5);

  iet.setTop({0, 1, 2, 3, 4});
  iet.setBot({3, 2, 0, 4, 1});

  iet.setLengths({977, 351, 143, 321, 12});

  iet.check();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{49, 351, 143, 321, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{3, 2, 0, 4, 1}));
  EXPECT_EQ(iet.topPermutation(), (v{0, 1, 2, 3, 4}));

  iet.swapTopBot();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{49, 351, 143, 272, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{1, 2, 0, 3, 4}));
  EXPECT_EQ(iet.topPermutation(), (v{3, 2, 0, 4, 1}));

  iet.swapTopBot();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{49, 79, 143, 272, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{2, 0, 4, 3, 1}));
  EXPECT_EQ(iet.topPermutation(), (v{1, 2, 0, 3, 4}));

  iet.swapTopBot();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{49, 79, 64, 272, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{1, 2, 0, 3, 4}));
  EXPECT_EQ(iet.topPermutation(), (v{2, 0, 4, 3, 1}));

  iet.swapTopBot();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{49, 15, 64, 272, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{0, 4, 3, 2, 1}));
  EXPECT_EQ(iet.topPermutation(), (v{1, 2, 0, 3, 4}));

  iet.swapTopBot();
  iet.zorichInductionStep();
  iet.check();

  EXPECT_EQ(iet.lengths(), (v{34, 15, 64, 272, 12}));
  EXPECT_EQ(iet.botPermutation(), (v{2, 1, 0, 3, 4}));
  EXPECT_EQ(iet.topPermutation(), (v{0, 4, 3, 2, 1}));
}

}  // namespace

#include "main.hpp"
