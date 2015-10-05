#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, Multiplication) {
  Monomial<> X = Monomial<>::x(0);
  Monomial<> Y = Monomial<>::x(1);
  Term<> x = Monomial<>::x(0);
  Term<> y = Monomial<>::x(1);

  Term<> t = 1;
  t *= y;

  EXPECT_EQ(Term<>(1, Y), t);

  Term<> t2 = 5 * x;

  EXPECT_EQ(Term<>(5, X), t2);

  Term<> t3 = y * -8;

  EXPECT_EQ(Term<>(-8, Y), t3);

  Term<> t4 = t2;
  t4 *= 3;

  EXPECT_EQ(Term<>(15, X), t4);

  t4 *= x;

  EXPECT_EQ(Term<>(15, X*X), t4);

  t4 *= t3;

  EXPECT_EQ(Term<>(-120, X*X*Y), t4);
}
