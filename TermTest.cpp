#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, MultiplicationAssignmentTimesMonomial) {
  Term<> t = 1;
  t *= Monomial<>::x(1);

  EXPECT_EQ(t, Term<>(Monomial<>::x(1)));
}

TEST(TermTest, MultiplicationMonomialTimesTerm) {
  Term<> t = 1;
  Term<> u = Monomial<>::x(0) * t;

  EXPECT_EQ(Term<>(Monomial<>::x(0)), u);
}
