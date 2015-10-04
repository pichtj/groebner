#include <sstream>

#include <gtest/gtest.h>

#include "Monomial.h"

using namespace std;

TEST(MonomialTest, Order) {
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
  stringstream s;
  s << e;
  EXPECT_EQ("{0 1}", s.str());
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
