#include <gtest/gtest.h>

#include "Exponent.h"

TEST(ExponentTest, Order) {
  Exponent<> e;
  Exponent<> f;
  Exponent<> g;
  g[1] = 1;
  Exponent<> h;
  h[1] = 12;
  EXPECT_FALSE(e < f);
  EXPECT_FALSE(f < e);
  EXPECT_TRUE(e < g);
  EXPECT_TRUE(e < h);
  EXPECT_TRUE(h > e);
  EXPECT_TRUE(g < h);
  EXPECT_FALSE(h < g);
}

TEST(ExponentTest, Equal) {
  Exponent<> e;
  Exponent<> f;
  e[1] = 12;
  EXPECT_NE(e, f);
  f[1] = 12;
  EXPECT_EQ(e, f);
}
