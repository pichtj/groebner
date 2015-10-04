#include <gtest/gtest.h>

#include <iostream>

#include "LabelledMonomial.h"

using namespace std;

TEST(LabelledMonomialTest, Example22) {
  Term<> x = Monomial<>::x(0);
  Term<> y = Monomial<>::x(1);
  Polynomial<> f1 = x + 1;
  Polynomial<> f2 = y + 2;
  
  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);

  LabelledMonomial<> X(x.exponent(), e1, f1);
  LabelledMonomial<> Y(y.exponent(), e2, f2);

  EXPECT_TRUE(X.isPrimitive());
  EXPECT_TRUE(Y.isPrimitive());

  LabelledMonomial<> Xy((x*y).exponent(), e1, f1);
  LabelledMonomial<> Yx((x*y).exponent(), e2, f2);

  EXPECT_TRUE(Xy.collidesWith(Yx));

  EXPECT_LT(Yx.signature(), Xy.signature());
}
