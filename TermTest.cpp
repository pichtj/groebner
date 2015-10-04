#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, Multiplication) {
  Monomial<> x = Monomial<>::x(0);
  Monomial<> y = Monomial<>::x(1);

  Term<> t = 1;
  t *= y;

  EXPECT_EQ(Term<>(1, y), t);

  Term<> t2 = 5 * x;

  EXPECT_EQ(Term<>(5, x), t2);

  Term<> t3 = y * -8;

  EXPECT_EQ(Term<>(-8, y), t3);

  Term<> t4 = t2;
  t4 *= 3;

  EXPECT_EQ(Term<>(15, x), t4);

  t4 *= x;

  EXPECT_EQ(Term<>(15, x*x), t4);

  t4 *= t3;

  EXPECT_EQ(Term<>(-120, x*x*y), t4);
}

TEST(TermTest, MultiplicationMonomialTimesTerm) {
  Term<> t = 1;
  Term<> u = Monomial<>::x(0) * t;

  EXPECT_EQ(Term<>(Monomial<>::x(0)), u);
}
