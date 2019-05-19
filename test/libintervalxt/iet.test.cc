#include <gtest/gtest.h>
#include <vector>

#include <intervalxt/iet.hpp>

using std::vector;

TEST(IETTest, InitializationLengths) {
  IntervalExchangeTransformation<unsigned long, unsigned long> iet(2);
  vector<unsigned long> v(2);
  v[0] = 18;
  v[1] = 3;
  iet.setLengths(v);

  EXPECT_EQ(iet.lengths(), v);
}

TEST(IETTest, InitializationTopBottom) {
  Permutation top(4);
  Permutation bot(4);

  top[0] = 0;
  top[1] = 1;
  top[2] = 3;
  top[3] = 2;
  bot[0] = 3;
  bot[1] = 0;
  bot[2] = 1;
  bot[3] = 2;

  IntervalExchangeTransformation<unsigned long, unsigned long> iet(top, bot);

  EXPECT_EQ(iet.topPermutation(), top);
  EXPECT_EQ(iet.botPermutation(), bot);
}

TEST(IETTest, IsIrreducible) {
  IntervalExchangeTransformation<unsigned long, unsigned long> iet(3);
  Permutation topperm(3);
  Permutation botperm(3);

  botperm[0] = 2;
  botperm[1] = 1;
  botperm[2] = 0;
  iet.setBot(botperm);
  EXPECT_FALSE(iet.isReducible());

  botperm[0] = 1;
  botperm[1] = 0;
  botperm[2] = 2;
  iet.setBot(botperm);
  EXPECT_TRUE(iet.isReducible());

  topperm[0] = 2;
  topperm[1] = 0;
  topperm[2] = 1;
  botperm[0] = 0;
  botperm[1] = 2;
  botperm[2] = 1;
  iet.setTop(topperm);
  iet.setBot(botperm);
  EXPECT_TRUE(iet.isReducible());
}

#include "main.hpp"
