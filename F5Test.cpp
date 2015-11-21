#include <gtest/gtest.h>
#include <mpirxx.h>
#include <flint/fmpzxx.h>

#include "F5.h"

#include "ImmutablePolynomial.h"
#include "CachedMonomial.h"

using namespace std;
using namespace flint;

TEST(F5Test, f5) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 4, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  F5Runner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T t = T(1, M::x(3));

  P f1 = a*b*c;
  P f2 = a*b - c;
  P f3 = b*c - b;

  vector<P> input = { f1, f2, f3 };

  auto output = runner.f5(input);

  EXPECT_EQ(vector<P>({b*c-b*t, a*b-c*t, c*pow(t, 2), pow(c, 2)*t, b*pow(t, 3)}), output);
}

TEST(F5Test, hcyclic3_degrevlex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 4, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  F5Runner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T t = T(1, M::x(3));

  vector<P> input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - pow(t, 3)
  };

  auto output = runner.f5(input);

  EXPECT_EQ(vector<P>({a+b+c, pow(b, 2)+b*c+pow(c, 2), pow(c, 3)-pow(t, 3)}), output);
}

TEST(F5Test, hcyclic3_cached_degrevlex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, CachedMonomial<Monomial<char, 4, degrevlex> > > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  F5Runner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T t = T(1, M::x(3));

  vector<P> input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - pow(t, 3)
  };

  auto output = runner.f5(input);

  EXPECT_EQ(vector<P>({a+b+c, pow(b, 2)+b*c+pow(c, 2), pow(c, 3)-pow(t, 3)}), output);
}

TEST(F5Test, hcyclic3_lex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 4, lex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  F5Runner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T t = T(1, M::x(3));

  vector<P> input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - pow(t, 3)
  };

  auto output = runner.f5(input);

  EXPECT_EQ(vector<P>({ pow(c, 3)-pow(t, 3), pow(b, 2)+b*c+pow(c,2), a+b+c }), output);
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
