#include <gtest/gtest.h>

#include <iostream>

#include "Polynomial.h"

using namespace std;

TEST(PolynomialTest, EmptyConstruction) {
  Polynomial<> p;
  EXPECT_EQ(0, p.lc());
}

TEST(PolynomialTest, ConstantConstruction) {
  Polynomial<> p(42);
  EXPECT_EQ(42, p.lc());
  EXPECT_TRUE(p.lm().isZero());
}

TEST(PolynomialTest, ConstantAddition) {
  Polynomial<> a(17);
  Polynomial<> b(4);
  Polynomial<> p = a + b;
  EXPECT_EQ(21, p.lc());
  p += 1;
  EXPECT_EQ(22, p.lc());
  EXPECT_EQ(25, (p + 3).lc());
  EXPECT_TRUE(p.lm().isZero());
}

TEST(PolynomialTest, ConstantSubtraction) {
  Polynomial<> a(17);
  a -= 3;
  EXPECT_EQ(14, a.lc());
  EXPECT_EQ(13, (a - 1).lc());
}

TEST(PolynomialTest, Addition) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  EXPECT_EQ(5, a.lc());
  EXPECT_FALSE(a.lm().isZero());
  EXPECT_EQ(e, a.lm());
}

TEST(PolynomialTest, AdditionToZero) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  Polynomial<> b(-17);
  b += Term<>(-5, e);

  EXPECT_EQ(0, (a + b).lc());
  EXPECT_TRUE((a + b).isZero());
}

TEST(PolynomialTest, ConstantMultiplicationToZero) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  a *= 0;

  EXPECT_TRUE(a.isZero());
  EXPECT_EQ(0, a.lc());
}

TEST(PolynomialTest, ConstantMultiplication) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  a *= 3;

  EXPECT_EQ(15, a.lc());
}

TEST(PolynomialTest, TermMultiplication) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  Monomial<> f;
  f[0] = 2;

  a *= Term<>(3, f);

  Monomial<> g;
  g[0] = 2;
  g[1] = 12;

  EXPECT_EQ(15, a.lc());
  EXPECT_EQ(g, a.lm());
}

TEST(PolynomialTest, Multiplication) {
  use_abc_var_names in_this_scope;
  // 5y^12+17
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  // 3x^4+7
  Polynomial<> b(7);
  Monomial<> f;
  f[0] = 4;
  b += Term<>(3, f);

  Term<> x(1, Monomial<>::x(0));
  Term<> y(1, Monomial<>::x(1));

  Polynomial<> p = a * b;
  Monomial<> g;
  g[0] = 4;
  g[1] = 12;

  EXPECT_EQ(15, p.lc());
  EXPECT_EQ(g, p.lm());

  EXPECT_EQ(15*pow(x, 4)*pow(y, 12) + 51*pow(x, 4) + 35*pow(y, 12) + 119, p);
}

TEST(PolynomialTest, MonomialMultiplication) {
  Term<> x(1, Monomial<>::x(0));
  Term<> y(1, Monomial<>::x(1));

  // 5y^12+17
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  EXPECT_EQ(5*x*pow(y, 12) + 17*x, x*a);
  EXPECT_EQ(5*x*pow(y, 12) + 17*x, a*x);

  EXPECT_EQ(5*pow(y, 13) + 17*y, y*a);
  EXPECT_EQ(5*pow(y, 13) + 17*y, a*y);
}

TEST(PolynomialTest, Cyclic4Result) {
  use_abc_var_names in_this_scope;

  Term<> b(1, Monomial<>::x(1));
  Term<> d(1, Monomial<>::x(3));

  // b*d^4-b+d^5 - d
  Polynomial<> p = b*pow(d,4)-b+pow(d,5)-d;

  EXPECT_EQ("b*d^4-b+d^5-d", to_string(p));
}

TEST(PolynomialTest, Combine) {
  use_abc_var_names in_this_scope;

  Term<> a(1, Monomial<>::x(0));

  Polynomial<> p = 3*a*a + 5*a + 7;
  p.combine(2, a - 11, 13);

  EXPECT_EQ(6*a*a+23*a-129, p);

  Polynomial<> q = a - 11;
  q.combine(13, 3*a*a + 5*a + 7, 2);

  EXPECT_EQ(6*a*a+23*a-129, q);
}

TEST(PolynomialTest, Input) {
  typedef Polynomial<> P;
  use_abc_var_names in_this_scope;

  Term<> a(1, Monomial<>::x(0));
  Term<> b(1, Monomial<>::x(1));
  Term<> c(1, Monomial<>::x(2));
  Term<> d(1, Monomial<>::x(3));
  Term<> e(1, Monomial<>::x(4));

  EXPECT_EQ(6*pow(a, 2) + 23*b + 17, from_string<P>("6*a^2+23*b+17"));
  EXPECT_EQ(2*b*e-2*b*c+3*pow(c,2)*d-3*pow(c,2)*b, from_string<P>("2*b*e-2*b*c+3*c^2*d-3*c^2*b,"));

  EXPECT_EQ(a+b+c, from_string<P>("a+b+c\n"));
}

TEST(PolynomialTest, Size) {
  typedef Polynomial<> P;
  use_abc_var_names in_this_scope;

  Term<> a(1, Monomial<>::x(0));
  Term<> b(1, Monomial<>::x(1));
  Term<> c(1, Monomial<>::x(2));
  Term<> d(1, Monomial<>::x(3));
  Term<> e(1, Monomial<>::x(4));

  auto zero = P();
  auto constant = P(4);
  auto deg1 = P(4*e);
  auto dp4e = d + 4*e;
  auto aapbcp4e = a*a + b*c + 4*e;
  auto bcd = b + c + d;
  auto bd1 = b + 1 + d;

  EXPECT_EQ(0, zero.size());
  EXPECT_EQ(1, constant.size());
  EXPECT_EQ(1, deg1.size());
  EXPECT_EQ(2, dp4e.size());
  EXPECT_EQ(3, aapbcp4e.size());
  EXPECT_EQ(3, bcd.size());
  EXPECT_EQ(3, bd1.size());
}
