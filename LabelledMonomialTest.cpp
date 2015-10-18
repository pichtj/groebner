#include <gtest/gtest.h>

#include <iostream>

#include "LabelledMonomial.h"

using namespace std;

TEST(LabelledMonomialTest, Example2_2) {
  typedef Term<> T;
  typedef Monomial<> M;
  typedef Polynomial<> P;
  T x = T(1, M::x(0));
  T y = T(1, M::x(1));

  P f1 = x + 1;
  P f2 = y + 2;

  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);

  LabelledMonomial<> X(x.m(), e1, f1);
  LabelledMonomial<> Y(y.m(), e2, f2);

  EXPECT_TRUE(X.isPrimitive());
  EXPECT_TRUE(Y.isPrimitive());

  LabelledMonomial<> Xy((x*y).m(), e1, f1);
  LabelledMonomial<> Yx((x*y).m(), e2, f2);

  EXPECT_TRUE(Xy.collidesWith(Yx));

  EXPECT_LT(Yx.signature(), Xy.signature());
}

TEST(LabelledMonomialTest, Example2_18) {
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  MM<>r1 = MM<>(e1, f1);
  MM<>r2 = MM<>(e2, f2);
  MM<>r3 = MM<>(e3, f3);
  MM<>r4 = MM<>(e1 - e2 - a*e3, c - 1);

  LabelledMonomial<> abc_r1 = LabelledMonomial<>((a*b*c).m(), r1);
  LabelledMonomial<> abc_r3 = LabelledMonomial<>((a*b*c).m(), r3);

  EXPECT_LT(abc_r3.signature(), abc_r1.signature());

  LabelledMonomial<> bc_r3 = LabelledMonomial<>((b*c).m(), r3);
  LabelledMonomial<> bc_r4 = LabelledMonomial<>((b*c).m(), r4);

  EXPECT_LT(bc_r3.signature(), bc_r4.signature());
}
