#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, Multiplication) {
  Term<> t = 1;
  t *= Monomial<>::x(1);

  EXPECT_EQ(t, Term<>(Monomial<>::x(1)));
}
