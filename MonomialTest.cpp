#include <sstream>

#include <gtest/gtest.h>

#include "Monomial.h"

using namespace std;

TEST(MonomialTest, lex) {
  Monomial<> e;
  Monomial<> f;
  Monomial<> g;
  g[1] = 1;
  Monomial<> h;
  h[1] = 12;
  EXPECT_FALSE(e < f);
  EXPECT_FALSE(f < e);
  EXPECT_TRUE(e < g);
  EXPECT_TRUE(e < h);
  EXPECT_TRUE(h > e);
  EXPECT_TRUE(g < h);
  EXPECT_FALSE(h < g);
}

TEST(MonomialTest, degrevlex) {
  use_abc_var_names in_this_scope;
  Monomial<> a = Monomial<>::x(0);
  Monomial<> b = Monomial<>::x(1);
  Monomial<> c = Monomial<>::x(2);

  EXPECT_FALSE(a < b);
  EXPECT_FALSE(b < c);
  EXPECT_TRUE(b < a);
  EXPECT_TRUE(c < b);
  EXPECT_TRUE(a*b*b > b*b*c);
  EXPECT_TRUE(a*b*c < a*a*c);
  EXPECT_TRUE(a*b*c < pow(a, 5));
}

TEST(MonomialTest, Equal) {
  Monomial<> e;
  Monomial<> f;
  e[1] = 12;
  EXPECT_NE(e, f);
  f[1] = 12;
  EXPECT_EQ(e, f);
}

TEST(MonomialTest, x_i) {
  Monomial<> e = Monomial<>::x(0);
  EXPECT_EQ(e.degree(), 1);
}

TEST(MonomialTest, hash) {
  Monomial<> e = Monomial<>::x(1);
  Monomial<> f;

  std::hash<Monomial<> > h;

  EXPECT_NE(h(e), h(f));
}

TEST(MonomialTest, ostreamOperator) {
  Monomial<> e = Monomial<>::x(1);
  EXPECT_EQ("x1", to_string(e));
  EXPECT_EQ("1", to_string(Monomial<>()));
}

TEST(MonomialTest, divides) {
  Monomial<> x = Monomial<>::x(0);
  Monomial<> y = Monomial<>::x(1);

  Monomial<> a = x*x*x*y*y;
  Monomial<> b = x*x*y*y*y;
  Monomial<> c = x*y*y;

  EXPECT_FALSE(a.divides(b));
  EXPECT_FALSE(b.divides(a));
  EXPECT_FALSE(a.divides(c));
  EXPECT_TRUE(c.divides(a));
  EXPECT_FALSE(b.divides(c));
  EXPECT_TRUE(c.divides(b));
}

TEST(MonomialTest, lcm) {
  Monomial<> x = Monomial<>::x(0);
  Monomial<> y = Monomial<>::x(1);

  Monomial<> a = x*x*x*y*y;
  Monomial<> b = x*x*y*y*y;

  EXPECT_EQ(pow(x, 3)*pow(y, 3), lcm(a, b));
}

TEST(MonomialTest, Multiplication) {
  Monomial<> x = Monomial<>::x(0);
  Monomial<> y = Monomial<>::x(1);

  Monomial<> a = x*x*x*y*y;
  Monomial<> b = x*x*y*y*y;

  Monomial<> p = a;
  p *= b;

  EXPECT_EQ(pow(x, 5)*pow(y, 5), a * b);
  EXPECT_EQ(pow(x, 5)*pow(y, 5), b * a);
  EXPECT_EQ(pow(x, 5)*pow(y, 5), p);

  EXPECT_EQ(pow(x, 5)*pow(y, 5), a * b * Monomial<>());
  EXPECT_EQ(pow(x, 5)*pow(y, 5), b * a * Monomial<>());
}

TEST(MonomialTest, Division) {
  Monomial<> x = Monomial<>::x(0);
  Monomial<> y = Monomial<>::x(1);

  Monomial<> a = x*x*x*y*y;
  Monomial<> b = x*x*y*y*y;

  Monomial<> p = a;
  p /= x;

  EXPECT_EQ(pow(x, 2)*pow(y, 2), a / x);
  EXPECT_EQ(pow(x, 2)*pow(y, 2), b / y);
  EXPECT_EQ(pow(x, 2)*pow(y, 2), p);
}

TEST(MonomialTest, Input) {
  typedef Monomial<> M;
  use_abc_var_names in_this_scope;

  EXPECT_EQ(M::x(0), from_string<M>("a"));
  EXPECT_EQ(M::x(1), from_string<M>("b"));
  EXPECT_EQ(pow(M::x(0), 3), from_string<M>("a^3"));
  EXPECT_EQ(pow(M::x(1), 4), from_string<M>("b^4"));
  EXPECT_EQ(pow(M::x(0), 1) * pow(M::x(1), 4), from_string<M>("a*b^4"));
  EXPECT_EQ(pow(M::x(0), 3) * pow(M::x(1), 4), from_string<M>("a^3*b^4"));
}
