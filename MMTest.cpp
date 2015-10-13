#include <gtest/gtest.h>

#include "MM.h"

TEST(MMTest, Order) {
  MM<> m;

  lm_R_l<> u;
  u[2] = 5;

  Monomial<> e;
  e[1] = 1;
  u[2] *= e;

  MM<> n(u, 0);

  EXPECT_FALSE(m < m);
  EXPECT_TRUE(m < n);
  EXPECT_FALSE(m > n);
}
