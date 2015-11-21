#include <gtest/gtest.h>

#include "integral.h"

TEST(integralTest, log2_int) {
  EXPECT_EQ(0, log2(1));
  EXPECT_EQ(1, log2(2));
  EXPECT_EQ(2, log2(4));
  EXPECT_EQ(3, log2(8));
  EXPECT_EQ(4, log2(16));

  EXPECT_EQ(2, log2(3));
  EXPECT_EQ(3, log2(5));
  EXPECT_EQ(4, log2(9));
  EXPECT_EQ(5, log2(17));
}

TEST(integralTest, log2_mpz) {
  EXPECT_EQ(0, log2(mpz_class(1)));
  EXPECT_EQ(1, log2(mpz_class(2)));
  EXPECT_EQ(2, log2(mpz_class(4)));
  EXPECT_EQ(3, log2(mpz_class(8)));
  EXPECT_EQ(4, log2(mpz_class(16)));

  EXPECT_EQ(2, log2(mpz_class(3)));
  EXPECT_EQ(3, log2(mpz_class(5)));
  EXPECT_EQ(4, log2(mpz_class(9)));
  EXPECT_EQ(5, log2(mpz_class(17)));
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
