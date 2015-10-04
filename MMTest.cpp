#include <gtest/gtest.h>

#include "MM.h"

TEST(MMTest, Order) {
  MM<> m;
  MM<> n;
  n.u[2] = 5;
  Monomial<> e;
  e[1] = 1;
  n.u[2] *= e;

  EXPECT_FALSE(m < m);
  EXPECT_TRUE(m < n);
  EXPECT_FALSE(m > n);
}
