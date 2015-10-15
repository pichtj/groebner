#include <gtest/gtest.h>

#include "lm_R_l.h"

using namespace std;

TEST(lm_R_lTest, Addition) {
  typedef Monomial<> M;
  typedef Term<> T;
  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  lm_R_l<> u;
  u += e1;
  u = e1 - e2;
  u -= M::x(0)*e3;

  EXPECT_EQ(M(), u[0]);
  EXPECT_EQ(M(), u[1]);
  EXPECT_EQ(M(), u[2]);
}

TEST(lm_R_lTest, Multiplication) {
  typedef Monomial<> M;
  typedef Term<> T;
  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  lm_R_l<> u;
  u += e3;

  u *= T(1, M::x(0));

  EXPECT_TRUE(u[0].isZero());
  EXPECT_TRUE(u[1].isZero());
  EXPECT_EQ(M::x(0), u[2]);
}

TEST(lm_R_lTest, lm) {
  typedef Monomial<> M;
  typedef Term<> T;
  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  M a = M::x(0);
  M b = M::x(1);

  lm_R_l<> u = e1 - e2 - a * e3;
  lm_R_l<> v = b*e1 - b*e2 - b*a * e3;

  EXPECT_EQ(e1, u.lm());
  EXPECT_EQ(b*e1, v.lm());
}
