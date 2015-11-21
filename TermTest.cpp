#include <gtest/gtest.h>

#include "Term.h"

TEST(TermTest, Multiplication) {
  typedef Term<> T;
  typedef Monomial<> M;

  M X = M::x(0);
  M Y = M::x(1);
  T x = T(1, M::x(0));
  T y = T(1, M::x(1));

  T t = 1;
  t *= y;

  EXPECT_EQ(T(1, Y), t);

  T t2 = 5 * x;

  EXPECT_EQ(T(5, X), t2);

  T t3 = y * -8;

  EXPECT_EQ(T(-8, Y), t3);

  T t4 = t2;
  t4 *= 3;

  EXPECT_EQ(T(15, X), t4);

  t4 *= x;

  EXPECT_EQ(T(15, X*X), t4);

  t4 *= t3;

  EXPECT_EQ(T(-120, X*X*Y), t4);
}

TEST(TermTest, MultiplicationWithZero) {
  typedef Term<> T;
  typedef Monomial<> M;

  T t;
  EXPECT_EQ(0, t.c());
  EXPECT_EQ(M(), t.m());
  EXPECT_TRUE(t.isZero());
  EXPECT_TRUE(t.m().isConstant());

  t *= M::x(0);
  EXPECT_EQ(0, t.c());
  EXPECT_EQ(M(), t.m());
  EXPECT_TRUE(t.isZero());
  EXPECT_TRUE(t.m().isConstant());

  T s(5, M::x(1));
  s *= 0;
  EXPECT_EQ(0, s.c());
  EXPECT_EQ(M(), s.m());
  EXPECT_TRUE(s.isZero());
  EXPECT_TRUE(s.m().isConstant());
}

TEST(TermTest, Input) {
  use_abc_var_names in_this_scope;

  typedef Term<> T;
  typedef Monomial<> M;

  auto a = M::x(0);
  auto b = M::x(1);

  EXPECT_EQ(T(1, a), from_string<T>("a"));
  EXPECT_EQ(T(-1, a), from_string<T>("-a"));

  EXPECT_EQ(T(1, pow(a, 3)), from_string<T>("a^3"));
  EXPECT_EQ(T(1, pow(b, 4)), from_string<T>("b^4"));
  EXPECT_EQ(T(1, pow(a, 1) * pow(M::x(1), 4)), from_string<T>("a*b^4"));
  EXPECT_EQ(T(1, pow(a, 3) * pow(M::x(1), 4)), from_string<T>("a^3*b^4"));

  EXPECT_EQ(T(17, a), from_string<T>("+17*a"));
  EXPECT_EQ(T(-17, b), from_string<T>("-17*b"));
  EXPECT_EQ(T(17, pow(a, 3)), from_string<T>("17*a^3"));
  EXPECT_EQ(T(17, pow(b, 4)), from_string<T>("17*b^4"));
  EXPECT_EQ(T(17, pow(a, 1) * pow(b, 4)), from_string<T>("17*a*b^4"));
  EXPECT_EQ(T(17, pow(a, 3) * pow(b, 42)), from_string<T>("17*a^3*b^42"));

}
// vim:ruler:cindent:shiftwidth=2:expandtab:
