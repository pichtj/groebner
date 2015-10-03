#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, Multiplication) {
  Term<> t = 1;
  t *= Exponent<>::x(1);

  EXPECT_EQ(t, Term<>(Exponent<>::x(1)));
}
