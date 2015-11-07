#include <gtest/gtest.h>
#include <mpirxx.h>
#include <flint/fmpzxx.h>

#include "moGVW.h"
#include "CachedMonomial.h"

using namespace std;
using namespace flint;

TEST(moGVWTest, LCMCriterion) {
  use_abc_var_names in_this_scope;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef moGVWRunner<>::LMSet LMSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  MonRl<> e1 = MonRl<>::e(0);
  MonRl<> e2 = MonRl<>::e(1);
  MonRl<> e3 = MonRl<>::e(2);

  auto r1 = MM<>(e1, f1);
  auto r2 = MM<>(e2, f2);
  auto r3 = MM<>(e3, f3);
  auto r4 = MM<>(e1, c - 1);

  LMSet GG;
  GG[(a*b).m()] = r2;
  GG[(b*c).m()] = r3;
  GG[(b*c*c).m()] = r3;
  GG[(b*b*c).m()] = r3;
  GG[(a*b*c).m()] = r3;
  GG[(c).m()] = r4;
  GG[(c*c).m()] = r4;
  GG[(a*c).m()] = r4;
  GG[(a*c*c).m()] = r4;

  EXPECT_TRUE(runner.rejectedByLCMCriterion((a*b*c).m(), r4, GG));
}

TEST(moGVWTest, lift) {
  use_abc_var_names in_this_scope;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef moGVWRunner<>::LMSet LMSet;
  typedef moGVWRunner<>::MMSet MMSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  MonRl<> e1 = MonRl<>::e(0);
  MonRl<> e2 = MonRl<>::e(1);
  MonRl<> e3 = MonRl<>::e(2);

  auto r1 = MM<>(e1, f1);
  auto r2 = MM<>(e2, f2);
  auto r3 = MM<>(e3, f3);
  auto r4 = MM<>(e1, c - 1);

  LMSet GG;
  GG[(a*b*c).m()] = r1;
  GG[(a*b).m()] = r2;
  GG[(b*c).m()] = r3;

  LMSet todo;
  todo[(b*c).m()] = r3;

  MMSet HH = runner.lift(todo, GG);

  LMSet GG2;
  GG2[(a*b).m()] = r2;
  GG2[(b*c).m()] = r3;
  GG2[(b*c*c).m()] = r3;
  GG2[(b*b*c).m()] = r3;
  GG2[(a*b*c).m()] = r3;
  GG2[(c).m()] = r4;
  GG2[(c*c).m()] = r4;
  GG2[(a*c).m()] = r4;
  GG2[(a*c*c).m()] = r4;
}

TEST(moGVWTest, moGVW) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 3> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  vector<P> input = { f1, f2, f3 };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({c-1, a*b-1}), output);
}

TEST(moGVWTest, cyclic3) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 3> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  vector<P> input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - 1
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({pow(c,3)-1, pow(b,2)+b*c+pow(c,2), a+b+c}), output);
}

TEST(moGVWTest, cyclic3_degrevlex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 3, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  vector<P> input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - 1
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({a+b+c, pow(b,2)+b*c+pow(c,2), pow(c,3)-1}), output);
}

TEST(moGVWTest, cyclic4) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 4> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));

  vector<P> input = {
    a + b + c + d,
    a*b + b*c + a*d + c*d,
    a*b*c + a*b*d + a*c*d + b*c*d,
    a*b*c*d - 1
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    pow(c,2)*pow(d,6)-pow(c,2)*pow(d,2)-pow(d,4)+1,
    pow(c,3)*pow(d,2)+pow(c,2)*pow(d,3)-c-d,
    b*pow(d,4)-b+pow(d,5)-d,
    b*c-b*d+pow(c,2)*pow(d,4)+c*d-2*pow(d,2),
    pow(b,2)+2*b*d+pow(d,2),
    a+b+c+d
  }), output);
}

TEST(moGVWTest, cyclic4_degrevlex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<int, Monomial<char, 4, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));

  vector<P> input = {
    a + b + c + d,
    a*b + b*c + a*d + c*d,
    a*b*c + a*b*d + a*c*d + b*c*d,
    a*b*c*d - 1
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    a+b+c+d,
    pow(b, 2)+2*b*d+pow(d, 2),
    b*pow(c, 2)+pow(c, 2)*d-b*pow(d, 2)-pow(d, 3),
    b*c*pow(d, 2)+pow(c, 2)*pow(d, 2)-b*pow(d, 3)+c*pow(d, 3)-pow(d, 4)-1,
    b*pow(d, 4)+pow(d, 5)-b-d,
    pow(c, 3)*pow(d, 2)+pow(c, 2)*pow(d, 3)-c-d,
    pow(c, 2)*pow(d, 4)+b*c-b*d+c*d-2*pow(d, 2)
  }), output);
}

TEST(moGVWTest, cyclic5mpz_class) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<mpz_class, Monomial<char, 5> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));
  T e = T(1, M::x(4));

  vector<P> input = {
    a*b*c*d*e -1L,
    a*b*c*d + a*b*c*e + a*b*d*e + a*c*d*e + b*c*d*e,
    a*b*c + a*b*e + a*d*e + b*c*d + c*d*e,
    a*b + a*e + b*c + c*d + d*e,
    a + b + c + d + e
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    pow(e,15)+mpz_class(122)*pow(e,10)-mpz_class(122)*pow(e,5)-mpz_class(1),
    mpz_class(55)*pow(d,2)*pow(e,5)-mpz_class(55)*pow(d,2)-mpz_class(2)*d*pow(e,11)-mpz_class(231)*d*pow(e,6)+mpz_class(233)*d*e-mpz_class(8)*pow(e,12)-mpz_class(979)*pow(e,7)+mpz_class(987)*pow(e,2),
    mpz_class(55)*pow(d,7)+mpz_class(165)*pow(d,6)*e+mpz_class(55)*pow(d,5)*pow(e,2)-mpz_class(55)*pow(d,2)-mpz_class(398)*d*pow(e,11)-mpz_class(48554)*d*pow(e,6)+mpz_class(48787)*d*e-mpz_class(1042)*pow(e,12)-mpz_class(127116)*pow(e,7)+mpz_class(128103)*pow(e,2),
    mpz_class(55)*c*pow(e,5)-mpz_class(55)*c+pow(e,11)+mpz_class(143)*pow(e,6)-mpz_class(144)*e,
    mpz_class(275)*c*d-mpz_class(275)*c*e+mpz_class(440)*pow(d,6)*e+mpz_class(1210)*pow(d,5)*pow(e,2)-mpz_class(275)*pow(d,3)*pow(e,4)+mpz_class(275)*pow(d,2)-mpz_class(442)*d*pow(e,11)-mpz_class(53911)*d*pow(e,6)+mpz_class(53913)*d*e-mpz_class(1121)*pow(e,12)-mpz_class(136763)*pow(e,7)+mpz_class(136674)*pow(e,2),
    mpz_class(275)*pow(c,3)+mpz_class(550)*pow(c,2)*e-mpz_class(550)*c*pow(e,2)+mpz_class(275)*pow(d,6)*pow(e,2)+mpz_class(550)*pow(d,5)*pow(e,3)-mpz_class(550)*pow(d,4)*pow(e,4)+mpz_class(550)*pow(d,2)*e-mpz_class(232)*d*pow(e,12)-mpz_class(28336)*d*pow(e,7)+mpz_class(28018)*d*pow(e,2)-mpz_class(568)*pow(e,13)-mpz_class(69289)*pow(e,8)+mpz_class(69307)*pow(e,3),
    mpz_class(55)*b*pow(e,5)-mpz_class(55)*b+pow(e,11)+mpz_class(143)*pow(e,6)-mpz_class(144)*e,
    mpz_class(275)*b*d-mpz_class(275)*b*e-mpz_class(110)*pow(d,6)*e-mpz_class(440)*pow(d,5)*pow(e,2)-mpz_class(275)*pow(d,4)*pow(e,3)+mpz_class(275)*pow(d,3)*pow(e,4)+mpz_class(124)*d*pow(e,11)+mpz_class(15092)*d*pow(e,6)-mpz_class(15106)*d*e+mpz_class(346)*pow(e,12)+mpz_class(42218)*pow(e,7)-mpz_class(42124)*pow(e,2),
    mpz_class(275)*b*c-mpz_class(275)*b*e+mpz_class(275)*pow(c,2)+mpz_class(550)*c*e-mpz_class(330)*pow(d,6)*e-mpz_class(1045)*pow(d,5)*pow(e,2)-mpz_class(275)*pow(d,4)*pow(e,3)+mpz_class(275)*pow(d,3)*pow(e,4)-mpz_class(550)*pow(d,2)+mpz_class(334)*d*pow(e,11)+mpz_class(40722)*d*pow(e,6)-mpz_class(40726)*d*e+mpz_class(867)*pow(e,12)+mpz_class(105776)*pow(e,7)-mpz_class(105873)*pow(e,2),
    mpz_class(275)*pow(b,2)+mpz_class(825)*b*e+mpz_class(550)*pow(d,6)*e+mpz_class(1650)*pow(d,5)*pow(e,2)+mpz_class(275)*pow(d,4)*pow(e,3)-mpz_class(550)*pow(d,3)*pow(e,4)+mpz_class(275)*pow(d,2)-mpz_class(566)*d*pow(e,11)-mpz_class(69003)*d*pow(e,6)+mpz_class(69019)*d*e-mpz_class(1467)*pow(e,12)-mpz_class(178981)*pow(e,7)+mpz_class(179073)*pow(e,2),
    a+b+c+d+e
  }), output);
}

TEST(moGVWTest, cyclic5fmpzxx) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<fmpzxx, Monomial<char, 5> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(fmpzxx(1), M::x(0));
  T b = T(fmpzxx(1), M::x(1));
  T c = T(fmpzxx(1), M::x(2));
  T d = T(fmpzxx(1), M::x(3));
  T e = T(fmpzxx(1), M::x(4));

  vector<P> input = {
    a*b*c*d*e - fmpzxx(1),
    a*b*c*d + a*b*c*e + a*b*d*e + a*c*d*e + b*c*d*e,
    a*b*c + a*b*e + a*d*e + b*c*d + c*d*e,
    a*b + a*e + b*c + c*d + d*e,
    a + b + c + d + e
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    pow(e,15)+fmpzxx(122)*pow(e,10)-fmpzxx(122)*pow(e,5)-fmpzxx(1),
    fmpzxx(55)*pow(d,2)*pow(e,5)-fmpzxx(55)*pow(d,2)-fmpzxx(2)*d*pow(e,11)-fmpzxx(231)*d*pow(e,6)+fmpzxx(233)*d*e-fmpzxx(8)*pow(e,12)-fmpzxx(979)*pow(e,7)+fmpzxx(987)*pow(e,2),
    fmpzxx(55)*pow(d,7)+fmpzxx(165)*pow(d,6)*e+fmpzxx(55)*pow(d,5)*pow(e,2)-fmpzxx(55)*pow(d,2)-fmpzxx(398)*d*pow(e,11)-fmpzxx(48554)*d*pow(e,6)+fmpzxx(48787)*d*e-fmpzxx(1042)*pow(e,12)-fmpzxx(127116)*pow(e,7)+fmpzxx(128103)*pow(e,2),
    fmpzxx(55)*c*pow(e,5)-fmpzxx(55)*c+pow(e,11)+fmpzxx(143)*pow(e,6)-fmpzxx(144)*e,
    fmpzxx(275)*c*d-fmpzxx(275)*c*e+fmpzxx(440)*pow(d,6)*e+fmpzxx(1210)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,3)*pow(e,4)+fmpzxx(275)*pow(d,2)-fmpzxx(442)*d*pow(e,11)-fmpzxx(53911)*d*pow(e,6)+fmpzxx(53913)*d*e-fmpzxx(1121)*pow(e,12)-fmpzxx(136763)*pow(e,7)+fmpzxx(136674)*pow(e,2),
    fmpzxx(275)*pow(c,3)+fmpzxx(550)*pow(c,2)*e-fmpzxx(550)*c*pow(e,2)+fmpzxx(275)*pow(d,6)*pow(e,2)+fmpzxx(550)*pow(d,5)*pow(e,3)-fmpzxx(550)*pow(d,4)*pow(e,4)+fmpzxx(550)*pow(d,2)*e-fmpzxx(232)*d*pow(e,12)-fmpzxx(28336)*d*pow(e,7)+fmpzxx(28018)*d*pow(e,2)-fmpzxx(568)*pow(e,13)-fmpzxx(69289)*pow(e,8)+fmpzxx(69307)*pow(e,3),
    fmpzxx(55)*b*pow(e,5)-fmpzxx(55)*b+pow(e,11)+fmpzxx(143)*pow(e,6)-fmpzxx(144)*e,
    fmpzxx(275)*b*d-fmpzxx(275)*b*e-fmpzxx(110)*pow(d,6)*e-fmpzxx(440)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,4)*pow(e,3)+fmpzxx(275)*pow(d,3)*pow(e,4)+fmpzxx(124)*d*pow(e,11)+fmpzxx(15092)*d*pow(e,6)-fmpzxx(15106)*d*e+fmpzxx(346)*pow(e,12)+fmpzxx(42218)*pow(e,7)-fmpzxx(42124)*pow(e,2),
    fmpzxx(275)*b*c-fmpzxx(275)*b*e+fmpzxx(275)*pow(c,2)+fmpzxx(550)*c*e-fmpzxx(330)*pow(d,6)*e-fmpzxx(1045)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,4)*pow(e,3)+fmpzxx(275)*pow(d,3)*pow(e,4)-fmpzxx(550)*pow(d,2)+fmpzxx(334)*d*pow(e,11)+fmpzxx(40722)*d*pow(e,6)-fmpzxx(40726)*d*e+fmpzxx(867)*pow(e,12)+fmpzxx(105776)*pow(e,7)-fmpzxx(105873)*pow(e,2),
    fmpzxx(275)*pow(b,2)+fmpzxx(825)*b*e+fmpzxx(550)*pow(d,6)*e+fmpzxx(1650)*pow(d,5)*pow(e,2)+fmpzxx(275)*pow(d,4)*pow(e,3)-fmpzxx(550)*pow(d,3)*pow(e,4)+fmpzxx(275)*pow(d,2)-fmpzxx(566)*d*pow(e,11)-fmpzxx(69003)*d*pow(e,6)+fmpzxx(69019)*d*e-fmpzxx(1467)*pow(e,12)-fmpzxx(178981)*pow(e,7)+fmpzxx(179073)*pow(e,2),
    a+b+c+d+e
  }), output);
}

TEST(moGVWTest, cyclic5fmpzxx_cached) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<fmpzxx, CachedMonomial<Monomial<char, 5> > > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(fmpzxx(1), M::x(0));
  T b = T(fmpzxx(1), M::x(1));
  T c = T(fmpzxx(1), M::x(2));
  T d = T(fmpzxx(1), M::x(3));
  T e = T(fmpzxx(1), M::x(4));

  vector<P> input = {
    a*b*c*d*e - fmpzxx(1),
    a*b*c*d + a*b*c*e + a*b*d*e + a*c*d*e + b*c*d*e,
    a*b*c + a*b*e + a*d*e + b*c*d + c*d*e,
    a*b + a*e + b*c + c*d + d*e,
    a + b + c + d + e
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    pow(e,15)+fmpzxx(122)*pow(e,10)-fmpzxx(122)*pow(e,5)-fmpzxx(1),
    fmpzxx(55)*pow(d,2)*pow(e,5)-fmpzxx(55)*pow(d,2)-fmpzxx(2)*d*pow(e,11)-fmpzxx(231)*d*pow(e,6)+fmpzxx(233)*d*e-fmpzxx(8)*pow(e,12)-fmpzxx(979)*pow(e,7)+fmpzxx(987)*pow(e,2),
    fmpzxx(55)*pow(d,7)+fmpzxx(165)*pow(d,6)*e+fmpzxx(55)*pow(d,5)*pow(e,2)-fmpzxx(55)*pow(d,2)-fmpzxx(398)*d*pow(e,11)-fmpzxx(48554)*d*pow(e,6)+fmpzxx(48787)*d*e-fmpzxx(1042)*pow(e,12)-fmpzxx(127116)*pow(e,7)+fmpzxx(128103)*pow(e,2),
    fmpzxx(55)*c*pow(e,5)-fmpzxx(55)*c+pow(e,11)+fmpzxx(143)*pow(e,6)-fmpzxx(144)*e,
    fmpzxx(275)*c*d-fmpzxx(275)*c*e+fmpzxx(440)*pow(d,6)*e+fmpzxx(1210)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,3)*pow(e,4)+fmpzxx(275)*pow(d,2)-fmpzxx(442)*d*pow(e,11)-fmpzxx(53911)*d*pow(e,6)+fmpzxx(53913)*d*e-fmpzxx(1121)*pow(e,12)-fmpzxx(136763)*pow(e,7)+fmpzxx(136674)*pow(e,2),
    fmpzxx(275)*pow(c,3)+fmpzxx(550)*pow(c,2)*e-fmpzxx(550)*c*pow(e,2)+fmpzxx(275)*pow(d,6)*pow(e,2)+fmpzxx(550)*pow(d,5)*pow(e,3)-fmpzxx(550)*pow(d,4)*pow(e,4)+fmpzxx(550)*pow(d,2)*e-fmpzxx(232)*d*pow(e,12)-fmpzxx(28336)*d*pow(e,7)+fmpzxx(28018)*d*pow(e,2)-fmpzxx(568)*pow(e,13)-fmpzxx(69289)*pow(e,8)+fmpzxx(69307)*pow(e,3),
    fmpzxx(55)*b*pow(e,5)-fmpzxx(55)*b+pow(e,11)+fmpzxx(143)*pow(e,6)-fmpzxx(144)*e,
    fmpzxx(275)*b*d-fmpzxx(275)*b*e-fmpzxx(110)*pow(d,6)*e-fmpzxx(440)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,4)*pow(e,3)+fmpzxx(275)*pow(d,3)*pow(e,4)+fmpzxx(124)*d*pow(e,11)+fmpzxx(15092)*d*pow(e,6)-fmpzxx(15106)*d*e+fmpzxx(346)*pow(e,12)+fmpzxx(42218)*pow(e,7)-fmpzxx(42124)*pow(e,2),
    fmpzxx(275)*b*c-fmpzxx(275)*b*e+fmpzxx(275)*pow(c,2)+fmpzxx(550)*c*e-fmpzxx(330)*pow(d,6)*e-fmpzxx(1045)*pow(d,5)*pow(e,2)-fmpzxx(275)*pow(d,4)*pow(e,3)+fmpzxx(275)*pow(d,3)*pow(e,4)-fmpzxx(550)*pow(d,2)+fmpzxx(334)*d*pow(e,11)+fmpzxx(40722)*d*pow(e,6)-fmpzxx(40726)*d*e+fmpzxx(867)*pow(e,12)+fmpzxx(105776)*pow(e,7)-fmpzxx(105873)*pow(e,2),
    fmpzxx(275)*pow(b,2)+fmpzxx(825)*b*e+fmpzxx(550)*pow(d,6)*e+fmpzxx(1650)*pow(d,5)*pow(e,2)+fmpzxx(275)*pow(d,4)*pow(e,3)-fmpzxx(550)*pow(d,3)*pow(e,4)+fmpzxx(275)*pow(d,2)-fmpzxx(566)*d*pow(e,11)-fmpzxx(69003)*d*pow(e,6)+fmpzxx(69019)*d*e-fmpzxx(1467)*pow(e,12)-fmpzxx(178981)*pow(e,7)+fmpzxx(179073)*pow(e,2),
    a+b+c+d+e
  }), output);
}

TEST(moGVWTest, cyclic5fmpzxx_degrevlex_minimal) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<fmpzxx, Monomial<char, 5, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(fmpzxx(1), M::x(0));
  T b = T(fmpzxx(1), M::x(1));
  T c = T(fmpzxx(1), M::x(2));
  T d = T(fmpzxx(1), M::x(3));
  T e = T(fmpzxx(1), M::x(4));

  vector<P> input = {
    a+b+c+d+e,
    pow(b, 2)+b*d-c*d+fmpzxx(2)*b*e+c*e+pow(e, 2),
    b*pow(c, 2)-b*c*d+pow(c, 2)*d-pow(c, 2)*e+b*d*e+c*d*e+pow(d, 2)*e-b*pow(e, 2)-fmpzxx(2)*c*pow(e, 2)+d*pow(e, 2)-pow(e, 3),
    pow(c, 3)*e+b*c*d*e-fmpzxx(2)*b*pow(d, 2)*e-c*pow(d, 2)*e-pow(d, 3)*e+fmpzxx(3)*pow(c, 2)*pow(e, 2)-fmpzxx(2)*b*d*pow(e, 2)-fmpzxx(2)*c*d*pow(e, 2)-fmpzxx(3)*pow(d, 2)*pow(e, 2)+fmpzxx(3)*b*pow(e, 3)+fmpzxx(3)*c*pow(e, 3)-fmpzxx(2)*d*pow(e, 3)+fmpzxx(2)*pow(e, 4),
    pow(d, 4)+fmpzxx(14)*b*c*d*e+fmpzxx(6)*pow(c, 2)*d*e-fmpzxx(27)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-fmpzxx(15)*pow(d, 3)*e-b*c*pow(e, 2)+fmpzxx(7)*pow(c, 2)*pow(e, 2)-fmpzxx(10)*b*d*pow(e, 2)-fmpzxx(9)*c*d*pow(e, 2)-fmpzxx(33)*pow(d, 2)*pow(e, 2)+fmpzxx(24)*b*pow(e, 3)+fmpzxx(33)*c*pow(e, 3)-fmpzxx(14)*d*pow(e, 3)+fmpzxx(22)*pow(e, 4),
    c*pow(d, 3)-fmpzxx(2)*b*c*d*e-pow(c, 2)*d*e+fmpzxx(5)*b*pow(d, 2)*e+fmpzxx(4)*pow(d, 3)*e-b*c*pow(e, 2)-fmpzxx(2)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(7)*pow(d, 2)*pow(e, 2)-fmpzxx(4)*b*pow(e, 3)-fmpzxx(7)*c*pow(e, 3)+fmpzxx(2)*d*pow(e, 3)-fmpzxx(4)*pow(e, 4),
    b*pow(d, 3)-fmpzxx(5)*b*c*d*e-fmpzxx(2)*pow(c, 2)*d*e+fmpzxx(10)*b*pow(d, 2)*e+c*pow(d, 2)*e+fmpzxx(6)*pow(d, 3)*e-fmpzxx(3)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(2)*c*d*pow(e, 2)+fmpzxx(13)*pow(d, 2)*pow(e, 2)-fmpzxx(8)*b*pow(e, 3)-fmpzxx(13)*c*pow(e, 3)+fmpzxx(4)*d*pow(e, 3)-fmpzxx(8)*pow(e, 4),
    pow(c, 2)*pow(d, 2)+b*c*d*e-fmpzxx(2)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-pow(d, 3)*e-fmpzxx(2)*b*d*pow(e, 2)-fmpzxx(2)*c*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+fmpzxx(3)*b*pow(e, 3)+fmpzxx(2)*c*pow(e, 3)-fmpzxx(2)*d*pow(e, 3)+fmpzxx(2)*pow(e, 4),
    b*c*pow(d, 2)+b*c*d*e+pow(c, 2)*d*e-b*pow(d, 2)*e+c*pow(d, 2)*e-pow(d, 3)*e-b*c*pow(e, 2)-b*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+b*pow(e, 3)+c*pow(e, 3)-d*pow(e, 3)+pow(e, 4),
    pow(c, 3)*d+b*c*d*e+fmpzxx(3)*pow(c, 2)*d*e-fmpzxx(3)*b*pow(d, 2)*e+c*pow(d, 2)*e-pow(d, 3)*e-b*c*pow(e, 2)-pow(c, 2)*pow(e, 2)-fmpzxx(2)*c*d*pow(e, 2)+fmpzxx(3)*b*pow(e, 3)-c*pow(e, 3)-d*pow(e, 3)+pow(e, 4),
    pow(c, 4)-fmpzxx(4)*b*c*d*e-pow(c, 2)*d*e+fmpzxx(5)*b*pow(d, 2)*e+fmpzxx(4)*c*pow(d, 2)*e+fmpzxx(3)*pow(d, 3)*e+b*c*pow(e, 2)-fmpzxx(8)*pow(c, 2)*pow(e, 2)+fmpzxx(4)*b*d*pow(e, 2)+fmpzxx(2)*c*d*pow(e, 2)+fmpzxx(9)*pow(d, 2)*pow(e, 2)-fmpzxx(6)*b*pow(e, 3)-fmpzxx(9)*c*pow(e, 3)+fmpzxx(4)*d*pow(e, 3)-fmpzxx(5)*pow(e, 4),
    fmpzxx(2)*b*c*d*pow(e, 2)+pow(c, 2)*d*pow(e, 2)+fmpzxx(2)*c*pow(d, 2)*pow(e, 2)-b*c*pow(e, 3)-fmpzxx(2)*b*d*pow(e, 3)-c*d*pow(e, 3)-pow(d, 2)*pow(e, 3)+b*pow(e, 4)+c*pow(e, 4)-fmpzxx(2)*d*pow(e, 4)+pow(e, 5)-fmpzxx(1),
    b*pow(e, 5)-c*pow(e, 5)-b+c,
    fmpzxx(15)*c*pow(d, 2)*pow(e, 3)+fmpzxx(10)*pow(d, 3)*pow(e, 3)-fmpzxx(20)*b*c*pow(e, 4)-fmpzxx(20)*pow(c, 2)*pow(e, 4)+fmpzxx(5)*b*d*pow(e, 4)-fmpzxx(25)*c*d*pow(e, 4)+fmpzxx(30)*pow(d, 2)*pow(e, 4)-fmpzxx(23)*c*pow(e, 5)-fmpzxx(3)*d*pow(e, 5)-fmpzxx(4)*pow(e, 6)+fmpzxx(15)*b-fmpzxx(7)*c+fmpzxx(3)*d+fmpzxx(24)*e,
    fmpzxx(3)*b*pow(d, 2)*pow(e, 3)+pow(d, 3)*pow(e, 3)+b*c*pow(e, 4)+pow(c, 2)*pow(e, 4)-b*d*pow(e, 4)+fmpzxx(2)*c*d*pow(e, 4)-fmpzxx(2)*c*pow(e, 5)-pow(e, 6)-fmpzxx(3)*b+fmpzxx(2)*c-fmpzxx(3)*e,
    fmpzxx(2)*c*d*pow(e, 5)+pow(d, 2)*pow(e, 5)+fmpzxx(8)*c*pow(e, 6)+d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(2)*c*d-pow(d, 2)-fmpzxx(8)*c*e-d*e-fmpzxx(3)*pow(e, 2),
    pow(c, 2)*pow(e, 5)+fmpzxx(3)*c*pow(e, 6)+pow(e, 7)-pow(c, 2)-fmpzxx(3)*c*e-pow(e, 2),
    fmpzxx(5)*pow(d, 3)*pow(e, 4)+fmpzxx(11)*pow(d, 2)*pow(e, 5)+fmpzxx(9)*c*pow(e, 6)+fmpzxx(2)*d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(10)*b*c-fmpzxx(10)*pow(c, 2)+fmpzxx(10)*b*d-fmpzxx(5)*c*d+fmpzxx(4)*pow(d, 2)-fmpzxx(24)*c*e-fmpzxx(2)*d*e+fmpzxx(7)*pow(e, 2),
    fmpzxx(177881)*pow(e, 8)-fmpzxx(19363958)*pow(c, 3)-fmpzxx(11892956)*b*c*d+fmpzxx(3735501)*pow(c, 2)*d+fmpzxx(9377551)*b*pow(d, 2)+fmpzxx(26834960)*c*pow(d, 2)+fmpzxx(9580503)*pow(d, 3)-fmpzxx(13518956)*b*c*e-fmpzxx(67875329)*pow(c, 2)*e+fmpzxx(41040369)*b*d*e+fmpzxx(15425505)*c*d*e+fmpzxx(54356373)*pow(d, 2)*e-fmpzxx(25006008)*b*pow(e, 2)-fmpzxx(54356373)*c*pow(e, 2)+fmpzxx(31256914)*d*pow(e, 2)+fmpzxx(228023)*pow(e, 3),
    fmpzxx(177881)*d*pow(e, 7)-fmpzxx(10265418)*pow(c, 3)-fmpzxx(29832328)*b*c*d-fmpzxx(9783455)*pow(c, 2)*d+fmpzxx(29780648)*b*pow(d, 2)+fmpzxx(20938278)*c*pow(d, 2)+fmpzxx(17202777)*pow(d, 3)+fmpzxx(5158549)*b*c*e-fmpzxx(35421160)*pow(c, 2)*e+fmpzxx(14482882)*b*d*e+fmpzxx(2386974)*c*d*e+fmpzxx(52141974)*pow(d, 2)*e-fmpzxx(19589751)*b*pow(e, 2)-fmpzxx(52141974)*c*pow(e, 2)+fmpzxx(39919865)*d*pow(e, 2)-fmpzxx(25155742)*pow(e, 3),
    fmpzxx(355762)*c*pow(e, 7)+fmpzxx(14797269)*pow(c, 3)+fmpzxx(9105077)*b*c*d-fmpzxx(2846096)*pow(c, 2)*d-fmpzxx(7181532)*b*pow(d, 2)-fmpzxx(20489461)*c*pow(d, 2)-fmpzxx(7326267)*pow(d, 3)+fmpzxx(10317098)*b*c*e+fmpzxx(51862809)*pow(c, 2)*e-fmpzxx(31373348)*b*d*e-fmpzxx(11806438)*c*d*e-fmpzxx(41545711)*pow(d, 2)*e+fmpzxx(19132705)*b*pow(e, 2)+fmpzxx(41189949)*c*pow(e, 2)-fmpzxx(23902346)*d*pow(e, 2)-fmpzxx(289470)*pow(e, 3),
    fmpzxx(177881)*pow(d, 2)*pow(e, 6)+fmpzxx(1380212)*pow(c, 3)+fmpzxx(6360880)*b*c*d+fmpzxx(2490334)*pow(c, 2)*d-fmpzxx(6495925)*b*pow(d, 2)-fmpzxx(3514784)*c*pow(d, 2)-fmpzxx(3514784)*pow(d, 3)-fmpzxx(1956691)*b*c*e+fmpzxx(4674279)*pow(c, 2)*e-fmpzxx(1159495)*b*d*e+fmpzxx(263553)*c*d*e-fmpzxx(9477066)*pow(d, 2)*e+fmpzxx(3251231)*b*pow(e, 2)+fmpzxx(9299185)*c*pow(e, 2)-fmpzxx(7741092)*d*pow(e, 2)+fmpzxx(5962282)*pow(e, 3)
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    a+b+c+d+e,
    pow(b, 2)+b*d-c*d+fmpzxx(2)*b*e+c*e+pow(e, 2),
    pow(c, 3)+b*c*d-fmpzxx(2)*b*pow(d, 2)-c*pow(d, 2)-pow(d, 3)+fmpzxx(3)*pow(c, 2)*e-fmpzxx(2)*b*d*e-fmpzxx(2)*c*d*e-fmpzxx(3)*pow(d, 2)*e+fmpzxx(3)*b*pow(e, 2)+fmpzxx(3)*c*pow(e, 2)-fmpzxx(2)*d*pow(e, 2)+fmpzxx(2)*pow(e, 3),
    b*pow(c, 2)-b*c*d+pow(c, 2)*d-pow(c, 2)*e+b*d*e+c*d*e+pow(d, 2)*e-b*pow(e, 2)-fmpzxx(2)*c*pow(e, 2)+d*pow(e, 2)-pow(e, 3),
    pow(d, 4)+fmpzxx(14)*b*c*d*e+fmpzxx(6)*pow(c, 2)*d*e-fmpzxx(27)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-fmpzxx(15)*pow(d, 3)*e-b*c*pow(e, 2)+fmpzxx(7)*pow(c, 2)*pow(e, 2)-fmpzxx(10)*b*d*pow(e, 2)-fmpzxx(9)*c*d*pow(e, 2)-fmpzxx(33)*pow(d, 2)*pow(e, 2)+fmpzxx(24)*b*pow(e, 3)+fmpzxx(33)*c*pow(e, 3)-fmpzxx(14)*d*pow(e, 3)+fmpzxx(22)*pow(e, 4),
    c*pow(d, 3)-fmpzxx(2)*b*c*d*e-pow(c, 2)*d*e+fmpzxx(5)*b*pow(d, 2)*e+fmpzxx(4)*pow(d, 3)*e-b*c*pow(e, 2)-fmpzxx(2)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(7)*pow(d, 2)*pow(e, 2)-fmpzxx(4)*b*pow(e, 3)-fmpzxx(7)*c*pow(e, 3)+fmpzxx(2)*d*pow(e, 3)-fmpzxx(4)*pow(e, 4),
    b*pow(d, 3)-fmpzxx(5)*b*c*d*e-fmpzxx(2)*pow(c, 2)*d*e+fmpzxx(10)*b*pow(d, 2)*e+c*pow(d, 2)*e+fmpzxx(6)*pow(d, 3)*e-fmpzxx(3)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(2)*c*d*pow(e, 2)+fmpzxx(13)*pow(d, 2)*pow(e, 2)-fmpzxx(8)*b*pow(e, 3)-fmpzxx(13)*c*pow(e, 3)+fmpzxx(4)*d*pow(e, 3)-fmpzxx(8)*pow(e, 4),
    pow(c, 2)*pow(d, 2)+b*c*d*e-fmpzxx(2)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-pow(d, 3)*e-fmpzxx(2)*b*d*pow(e, 2)-fmpzxx(2)*c*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+fmpzxx(3)*b*pow(e, 3)+fmpzxx(2)*c*pow(e, 3)-fmpzxx(2)*d*pow(e, 3)+fmpzxx(2)*pow(e, 4),
    b*c*pow(d, 2)+b*c*d*e+pow(c, 2)*d*e-b*pow(d, 2)*e+c*pow(d, 2)*e-pow(d, 3)*e-b*c*pow(e, 2)-b*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+b*pow(e, 3)+c*pow(e, 3)-d*pow(e, 3)+pow(e, 4),
    fmpzxx(2)*b*c*d*pow(e, 2)+pow(c, 2)*d*pow(e, 2)+fmpzxx(2)*c*pow(d, 2)*pow(e, 2)-b*c*pow(e, 3)-fmpzxx(2)*b*d*pow(e, 3)-c*d*pow(e, 3)-pow(d, 2)*pow(e, 3)+b*pow(e, 4)+c*pow(e, 4)-fmpzxx(2)*d*pow(e, 4)+pow(e, 5)-fmpzxx(1),
    b*pow(e, 5)-c*pow(e, 5)-b+c,
    fmpzxx(15)*c*pow(d, 2)*pow(e, 3)+fmpzxx(10)*pow(d, 3)*pow(e, 3)-fmpzxx(20)*b*c*pow(e, 4)-fmpzxx(20)*pow(c, 2)*pow(e, 4)+fmpzxx(5)*b*d*pow(e, 4)-fmpzxx(25)*c*d*pow(e, 4)+fmpzxx(30)*pow(d, 2)*pow(e, 4)-fmpzxx(23)*c*pow(e, 5)-fmpzxx(3)*d*pow(e, 5)-fmpzxx(4)*pow(e, 6)+fmpzxx(15)*b-fmpzxx(7)*c+fmpzxx(3)*d+fmpzxx(24)*e,
    fmpzxx(3)*b*pow(d, 2)*pow(e, 3)+pow(d, 3)*pow(e, 3)+b*c*pow(e, 4)+pow(c, 2)*pow(e, 4)-b*d*pow(e, 4)+fmpzxx(2)*c*d*pow(e, 4)-fmpzxx(2)*c*pow(e, 5)-pow(e, 6)-fmpzxx(3)*b+fmpzxx(2)*c-fmpzxx(3)*e,
    fmpzxx(2)*c*d*pow(e, 5)+pow(d, 2)*pow(e, 5)+fmpzxx(8)*c*pow(e, 6)+d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(2)*c*d-pow(d, 2)-fmpzxx(8)*c*e-d*e-fmpzxx(3)*pow(e, 2),
    pow(c, 2)*pow(e, 5)+fmpzxx(3)*c*pow(e, 6)+pow(e, 7)-pow(c, 2)-fmpzxx(3)*c*e-pow(e, 2),
    fmpzxx(5)*pow(d, 3)*pow(e, 4)+fmpzxx(11)*pow(d, 2)*pow(e, 5)+fmpzxx(9)*c*pow(e, 6)+fmpzxx(2)*d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(10)*b*c-fmpzxx(10)*pow(c, 2)+fmpzxx(10)*b*d-fmpzxx(5)*c*d+fmpzxx(4)*pow(d, 2)-fmpzxx(24)*c*e-fmpzxx(2)*d*e+fmpzxx(7)*pow(e, 2),
    pow(e, 8)+fmpzxx(42)*b*c*d+fmpzxx(21)*pow(c, 2)*d-fmpzxx(165)*b*pow(d, 2)+fmpzxx(42)*c*pow(d, 2)-fmpzxx(55)*pow(d, 3)-fmpzxx(76)*b*c*e-fmpzxx(55)*pow(c, 2)*e+fmpzxx(13)*b*d*e-fmpzxx(131)*c*d*e-fmpzxx(21)*pow(d, 2)*e+fmpzxx(186)*b*pow(e, 2)+fmpzxx(21)*c*pow(e, 2)-fmpzxx(42)*d*pow(e, 2)+fmpzxx(219)*pow(e, 3),
    d*pow(e, 7)-fmpzxx(110)*b*c*d-fmpzxx(55)*pow(c, 2)*d+fmpzxx(52)*b*pow(d, 2)+fmpzxx(60)*c*pow(d, 2)+fmpzxx(39)*pow(d, 3)+fmpzxx(29)*b*c*e-fmpzxx(26)*pow(c, 2)*e-fmpzxx(34)*b*d*e-fmpzxx(102)*c*d*e+fmpzxx(120)*pow(d, 2)*e+fmpzxx(63)*b*pow(e, 2)-fmpzxx(120)*c*pow(e, 2)+fmpzxx(109)*d*pow(e, 2)-fmpzxx(26)*pow(e, 3),
    c*pow(e, 7)-fmpzxx(16)*b*c*d-fmpzxx(8)*pow(c, 2)*d+fmpzxx(63)*b*pow(d, 2)-fmpzxx(16)*c*pow(d, 2)+fmpzxx(21)*pow(d, 3)+fmpzxx(29)*b*c*e+fmpzxx(21)*pow(c, 2)*e-fmpzxx(5)*b*d*e+fmpzxx(50)*c*d*e+fmpzxx(8)*pow(d, 2)*e-fmpzxx(71)*b*pow(e, 2)-fmpzxx(9)*c*pow(e, 2)+fmpzxx(16)*d*pow(e, 2)-fmpzxx(84)*pow(e, 3),
    pow(d, 2)*pow(e, 6)+fmpzxx(28)*b*c*d+fmpzxx(14)*pow(c, 2)*d-fmpzxx(21)*b*pow(d, 2)-fmpzxx(12)*c*pow(d, 2)-fmpzxx(12)*pow(d, 3)-fmpzxx(11)*b*c*e+fmpzxx(3)*pow(c, 2)*e+fmpzxx(9)*b*d*e+fmpzxx(17)*c*d*e-fmpzxx(30)*pow(d, 2)*e-fmpzxx(5)*b*pow(e, 2)+fmpzxx(29)*c*pow(e, 2)-fmpzxx(28)*d*pow(e, 2)+fmpzxx(18)*pow(e, 3)
  }), output);
}

TEST(moGVWTest, DISABLED_cyclic5fmpzxx_degrevlex) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<fmpzxx, Monomial<char, 5, degrevlex> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(fmpzxx(1), M::x(0));
  T b = T(fmpzxx(1), M::x(1));
  T c = T(fmpzxx(1), M::x(2));
  T d = T(fmpzxx(1), M::x(3));
  T e = T(fmpzxx(1), M::x(4));

  vector<P> input = {
    a*b*c*d*e - fmpzxx(1),
    a*b*c*d + a*b*c*e + a*b*d*e + a*c*d*e + b*c*d*e,
    a*b*c + a*b*e + a*d*e + b*c*d + c*d*e,
    a*b + a*e + b*c + c*d + d*e,
    a + b + c + d + e
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    a+b+c+d+e,
    pow(b, 2)+b*d-c*d+fmpzxx(2)*b*e+c*e+pow(e, 2),
    pow(c, 3)+b*c*d-fmpzxx(2)*b*pow(d, 2)-c*pow(d, 2)-pow(d, 3)+fmpzxx(3)*pow(c, 2)*e-fmpzxx(2)*b*d*e-fmpzxx(2)*c*d*e-fmpzxx(3)*pow(d, 2)*e+fmpzxx(3)*b*pow(e, 2)+fmpzxx(3)*c*pow(e, 2)-fmpzxx(2)*d*pow(e, 2)+fmpzxx(2)*pow(e, 3),
    b*pow(c, 2)-b*c*d+pow(c, 2)*d-pow(c, 2)*e+b*d*e+c*d*e+pow(d, 2)*e-b*pow(e, 2)-fmpzxx(2)*c*pow(e, 2)+d*pow(e, 2)-pow(e, 3),
    pow(d, 4)+fmpzxx(14)*b*c*d*e+fmpzxx(6)*pow(c, 2)*d*e-fmpzxx(27)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-fmpzxx(15)*pow(d, 3)*e-b*c*pow(e, 2)+fmpzxx(7)*pow(c, 2)*pow(e, 2)-fmpzxx(10)*b*d*pow(e, 2)-fmpzxx(9)*c*d*pow(e, 2)-fmpzxx(33)*pow(d, 2)*pow(e, 2)+fmpzxx(24)*b*pow(e, 3)+fmpzxx(33)*c*pow(e, 3)-fmpzxx(14)*d*pow(e, 3)+fmpzxx(22)*pow(e, 4),
    c*pow(d, 3)-fmpzxx(2)*b*c*d*e-pow(c, 2)*d*e+fmpzxx(5)*b*pow(d, 2)*e+fmpzxx(4)*pow(d, 3)*e-b*c*pow(e, 2)-fmpzxx(2)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(7)*pow(d, 2)*pow(e, 2)-fmpzxx(4)*b*pow(e, 3)-fmpzxx(7)*c*pow(e, 3)+fmpzxx(2)*d*pow(e, 3)-fmpzxx(4)*pow(e, 4),
    b*pow(d, 3)-fmpzxx(5)*b*c*d*e-fmpzxx(2)*pow(c, 2)*d*e+fmpzxx(10)*b*pow(d, 2)*e+c*pow(d, 2)*e+fmpzxx(6)*pow(d, 3)*e-fmpzxx(3)*pow(c, 2)*pow(e, 2)+fmpzxx(2)*b*d*pow(e, 2)+fmpzxx(2)*c*d*pow(e, 2)+fmpzxx(13)*pow(d, 2)*pow(e, 2)-fmpzxx(8)*b*pow(e, 3)-fmpzxx(13)*c*pow(e, 3)+fmpzxx(4)*d*pow(e, 3)-fmpzxx(8)*pow(e, 4),
    pow(c, 2)*pow(d, 2)+b*c*d*e-fmpzxx(2)*b*pow(d, 2)*e+fmpzxx(2)*c*pow(d, 2)*e-pow(d, 3)*e-fmpzxx(2)*b*d*pow(e, 2)-fmpzxx(2)*c*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+fmpzxx(3)*b*pow(e, 3)+fmpzxx(2)*c*pow(e, 3)-fmpzxx(2)*d*pow(e, 3)+fmpzxx(2)*pow(e, 4),
    b*c*pow(d, 2)+b*c*d*e+pow(c, 2)*d*e-b*pow(d, 2)*e+c*pow(d, 2)*e-pow(d, 3)*e-b*c*pow(e, 2)-b*d*pow(e, 2)-fmpzxx(2)*pow(d, 2)*pow(e, 2)+b*pow(e, 3)+c*pow(e, 3)-d*pow(e, 3)+pow(e, 4),
    fmpzxx(2)*b*c*d*pow(e, 2)+pow(c, 2)*d*pow(e, 2)+fmpzxx(2)*c*pow(d, 2)*pow(e, 2)-b*c*pow(e, 3)-fmpzxx(2)*b*d*pow(e, 3)-c*d*pow(e, 3)-pow(d, 2)*pow(e, 3)+b*pow(e, 4)+c*pow(e, 4)-fmpzxx(2)*d*pow(e, 4)+pow(e, 5)-fmpzxx(1),
    b*pow(e, 5)-c*pow(e, 5)-b+c,
    fmpzxx(15)*c*pow(d, 2)*pow(e, 3)+fmpzxx(10)*pow(d, 3)*pow(e, 3)-fmpzxx(20)*b*c*pow(e, 4)-fmpzxx(20)*pow(c, 2)*pow(e, 4)+fmpzxx(5)*b*d*pow(e, 4)-fmpzxx(25)*c*d*pow(e, 4)+fmpzxx(30)*pow(d, 2)*pow(e, 4)-fmpzxx(23)*c*pow(e, 5)-fmpzxx(3)*d*pow(e, 5)-fmpzxx(4)*pow(e, 6)+fmpzxx(15)*b-fmpzxx(7)*c+fmpzxx(3)*d+fmpzxx(24)*e,
    fmpzxx(3)*b*pow(d, 2)*pow(e, 3)+pow(d, 3)*pow(e, 3)+b*c*pow(e, 4)+pow(c, 2)*pow(e, 4)-b*d*pow(e, 4)+fmpzxx(2)*c*d*pow(e, 4)-fmpzxx(2)*c*pow(e, 5)-pow(e, 6)-fmpzxx(3)*b+fmpzxx(2)*c-fmpzxx(3)*e,
    fmpzxx(2)*c*d*pow(e, 5)+pow(d, 2)*pow(e, 5)+fmpzxx(8)*c*pow(e, 6)+d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(2)*c*d-pow(d, 2)-fmpzxx(8)*c*e-d*e-fmpzxx(3)*pow(e, 2),
    pow(c, 2)*pow(e, 5)+fmpzxx(3)*c*pow(e, 6)+pow(e, 7)-pow(c, 2)-fmpzxx(3)*c*e-pow(e, 2),
    fmpzxx(5)*pow(d, 3)*pow(e, 4)+fmpzxx(11)*pow(d, 2)*pow(e, 5)+fmpzxx(9)*c*pow(e, 6)+fmpzxx(2)*d*pow(e, 6)+fmpzxx(3)*pow(e, 7)-fmpzxx(10)*b*c-fmpzxx(10)*pow(c, 2)+fmpzxx(10)*b*d-fmpzxx(5)*c*d+fmpzxx(4)*pow(d, 2)-fmpzxx(24)*c*e-fmpzxx(2)*d*e+fmpzxx(7)*pow(e, 2),
    pow(e, 8)+fmpzxx(42)*b*c*d+fmpzxx(21)*pow(c, 2)*d-fmpzxx(165)*b*pow(d, 2)+fmpzxx(42)*c*pow(d, 2)-fmpzxx(55)*pow(d, 3)-fmpzxx(76)*b*c*e-fmpzxx(55)*pow(c, 2)*e+fmpzxx(13)*b*d*e-fmpzxx(131)*c*d*e-fmpzxx(21)*pow(d, 2)*e+fmpzxx(186)*b*pow(e, 2)+fmpzxx(21)*c*pow(e, 2)-fmpzxx(42)*d*pow(e, 2)+fmpzxx(219)*pow(e, 3),
    d*pow(e, 7)-fmpzxx(110)*b*c*d-fmpzxx(55)*pow(c, 2)*d+fmpzxx(52)*b*pow(d, 2)+fmpzxx(60)*c*pow(d, 2)+fmpzxx(39)*pow(d, 3)+fmpzxx(29)*b*c*e-fmpzxx(26)*pow(c, 2)*e-fmpzxx(34)*b*d*e-fmpzxx(102)*c*d*e+fmpzxx(120)*pow(d, 2)*e+fmpzxx(63)*b*pow(e, 2)-fmpzxx(120)*c*pow(e, 2)+fmpzxx(109)*d*pow(e, 2)-fmpzxx(26)*pow(e, 3),
    c*pow(e, 7)-fmpzxx(16)*b*c*d-fmpzxx(8)*pow(c, 2)*d+fmpzxx(63)*b*pow(d, 2)-fmpzxx(16)*c*pow(d, 2)+fmpzxx(21)*pow(d, 3)+fmpzxx(29)*b*c*e+fmpzxx(21)*pow(c, 2)*e-fmpzxx(5)*b*d*e+fmpzxx(50)*c*d*e+fmpzxx(8)*pow(d, 2)*e-fmpzxx(71)*b*pow(e, 2)-fmpzxx(9)*c*pow(e, 2)+fmpzxx(16)*d*pow(e, 2)-fmpzxx(84)*pow(e, 3),
    pow(d, 2)*pow(e, 6)+fmpzxx(28)*b*c*d+fmpzxx(14)*pow(c, 2)*d-fmpzxx(21)*b*pow(d, 2)-fmpzxx(12)*c*pow(d, 2)-fmpzxx(12)*pow(d, 3)-fmpzxx(11)*b*c*e+fmpzxx(3)*pow(c, 2)*e+fmpzxx(9)*b*d*e+fmpzxx(17)*c*d*e-fmpzxx(30)*pow(d, 2)*e-fmpzxx(5)*b*pow(e, 2)+fmpzxx(29)*c*pow(e, 2)-fmpzxx(28)*d*pow(e, 2)+fmpzxx(18)*pow(e, 3)
  }), output);
}

TEST(moGVWTest, cyclic6mpz_class) {
  use_abc_var_names in_this_scope;
  typedef Polynomial<Term<mpz_class, Monomial<char, 6> > > P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  moGVWRunner<P> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));
  T e = T(1, M::x(4));
  T f = T(1, M::x(5));

  vector<P> input = {
    a*b*c*d*e*f-1, a*b*c*d*e+a*b*c*d*f+a*b*c*e*f+a*b*d*e*f+a*c*d*e*f+b*c*d*e*f, a*b*c*d+b*c*d*e+c*d*e*f+d*e*f*a+e*f*a*b+f*a*b*c, a*b*c+b*c*d+c*d*e+d*e*f+e*f*a+f*a*b, a*b+b*c+c*d+d*e+e*f+f*a, a+b+c+d+e+f
  };

  auto output = runner.moGVW(input);

  EXPECT_EQ(vector<P>({
    pow(f, 48)-mpz_class("2554")*pow(f, 42)-mpz_class("399710")*pow(f, 36)-mpz_class("499722")*pow(f, 30)+mpz_class("499722")*pow(f, 18)+mpz_class("399710")*pow(f, 12)+mpz_class("2554")*pow(f, 6)-1,
    mpz_class("1387545279120")*pow(e, 2)*pow(f, 12)-mpz_class("1387545279120")*pow(e, 2)+mpz_class("4321823003")*e*pow(f, 43)-mpz_class("11037922310209")*e*pow(f, 37)-mpz_class("1727510711947989")*e*pow(f, 31)-mpz_class("2165150991154425")*e*pow(f, 25)-mpz_class("5114342560755")*e*pow(f, 19)+mpz_class("2162682824948601")*e*pow(f, 13)+mpz_class("1732620732685741")*e*pow(f, 7)+mpz_class("13506088516033")*e*f+mpz_class("24177661775")*pow(f, 44)-mpz_class("61749727185325")*pow(f, 38)-mpz_class("9664106795754225")*pow(f, 32)-mpz_class("12090487758628245")*pow(f, 26)-mpz_class("8787672733575")*pow(f, 20)+mpz_class("12083693383005045")*pow(f, 14)+mpz_class("9672870290826025")*pow(f, 8)+mpz_class("68544102808525")*pow(f, 2),
    mpz_class("25438330117200")*pow(e, 3)*pow(f, 6)+mpz_class("25438330117200")*pow(e, 3)+mpz_class("76314990351600")*pow(e, 2)*pow(f, 7)+mpz_class("76314990351600")*pow(e, 2)*f-mpz_class("1594966552735")*e*pow(f, 44)+mpz_class("4073543370415745")*e*pow(f, 38)+mpz_class("637527159231148925")*e*pow(f, 32)+mpz_class("797521176113606525")*e*pow(f, 26)+mpz_class("530440941097175")*e*pow(f, 20)-mpz_class("797160527306433145")*e*pow(f, 14)-mpz_class("638132320196044965")*e*pow(f, 8)-mpz_class("4510507167940725")*e*pow(f, 2)-mpz_class("6036376800443")*pow(f, 45)+mpz_class("15416903421476909")*pow(f, 39)+mpz_class("2412807646192304449")*pow(f, 33)+mpz_class("3017679923028013705")*pow(f, 27)+mpz_class("1422320037411955")*pow(f, 21)-mpz_class("3016560402417843941")*pow(f, 15)-mpz_class("2414249368183033161")*pow(f, 9)-mpz_class("16561862361763873")*pow(f, 3),
    mpz_class("1322793166094400")*pow(e, 6)-mpz_class("3968379498283200")*pow(e, 5)*f+mpz_class("3968379498283200")*pow(e, 4)*pow(f, 2)-mpz_class("5291172664377600")*pow(e, 3)*pow(f, 3)-mpz_class("230166010900425600")*pow(e, 2)*pow(f, 10)-mpz_class("226197631402142400")*pow(e, 2)*pow(f, 4)-mpz_class("152375364610443885")*e*pow(f, 47)+mpz_class("389166626064854890415")*e*pow(f, 41)+mpz_class("60906097841360558987335")*e*pow(f, 35)+mpz_class("76167367934608798697275")*e*pow(f, 29)+mpz_class("27855066785995181125")*e*pow(f, 23)-mpz_class("76144952817052723145495")*e*pow(f, 17)-mpz_class("60933629892463517546975")*e*pow(f, 11)-mpz_class("411415071682002547795")*e*pow(f, 5)-mpz_class("209493533143822")*pow(f, 42)+mpz_class("535045979490560586")*pow(f, 36)+mpz_class("83737947964973553146")*pow(f, 30)+mpz_class("104889507084213371570")*pow(f, 24)+mpz_class("167117997269207870")*pow(f, 18)-mpz_class("104793725781390615514")*pow(f, 12)-mpz_class("83842685189903180394")*pow(f, 6)-569978796672974242,
    mpz_class("11905138494849600")*d*pow(f, 6)-mpz_class("11905138494849600")*d-mpz_class("7936758996566400")*pow(e, 5)*pow(f, 2)+mpz_class("31747035986265600")*pow(e, 4)*pow(f, 3)-mpz_class("31747035986265600")*pow(e, 3)*pow(f, 4)-mpz_class("420648226818019200")*pow(e, 2)*pow(f, 11)-mpz_class("404774708824886400")*pow(e, 2)*pow(f, 5)+mpz_class("15336187600889")*e*pow(f, 42)-mpz_class("39169739565161107")*e*pow(f, 36)-mpz_class("6127176127489690827")*e*pow(f, 30)-mpz_class("7217708742310509615")*e*pow(f, 24)+mpz_class("538628483890722735")*e*pow(f, 18)+mpz_class("7506804353843507643")*e*pow(f, 12)+mpz_class("5886160769782607203")*e*pow(f, 6)+mpz_class("63576108396535879")*e+mpz_class("71737781777066")*pow(f, 43)-mpz_class("183218856207557938")*pow(f, 37)-mpz_class("28672874271132276078")*pow(f, 31)-mpz_class("35625223686939812010")*pow(f, 25)+mpz_class("164831339634084390")*pow(f, 19)+mpz_class("35724160423073052642")*pow(f, 13)+mpz_class("28627022578664910622")*pow(f, 7)+mpz_class("187459987029680506")*f,
    mpz_class("5952569247424800")*d*e-mpz_class("5952569247424800")*d*f-mpz_class("3968379498283200")*pow(e, 5)*pow(f, 3)+mpz_class("9920948745708000")*pow(e, 4)*pow(f, 4)-mpz_class("3968379498283200")*pow(e, 3)*pow(f, 5)-mpz_class("148814231185620000")*pow(e, 2)*pow(f, 6)-mpz_class("150798420934761600")*pow(e, 2)+mpz_class("492558110242553")*e*pow(f, 43)-mpz_class("1257992359608074599")*e*pow(f, 37)-mpz_class("196883094539368513959")*e*pow(f, 31)-mpz_class("246562115745735428055")*e*pow(f, 25)-mpz_class("325698701993885505")*e*pow(f, 19)+mpz_class("246417769883651808111")*e*pow(f, 13)+mpz_class("197327352068200652911")*e*pow(f, 7)+mpz_class("1523373796389332143")*e*f+mpz_class("2679481081803026")*pow(f, 44)-mpz_class("6843392695421906608")*pow(f, 38)-mpz_class("1071020459642646913578")*pow(f, 32)-mpz_class("1339789169692041240060")*pow(f, 26)-mpz_class("852746750910750210")*pow(f, 20)+mpz_class("1339105101971878401312")*pow(f, 14)+mpz_class("1071900289758712984762")*pow(f, 8)+mpz_class("7555239072072727756")*pow(f, 2),
    mpz_class("1984189749141600")*pow(d, 3)+mpz_class("5952569247424800")*pow(d, 2)*f-mpz_class("5952569247424800")*d*pow(f, 2)-mpz_class("3968379498283200")*pow(e, 5)*pow(f, 4)+mpz_class("15873517993132800")*pow(e, 4)*pow(f, 5)+mpz_class("17857707742274400")*pow(e, 3)-mpz_class("148814231185620000")*pow(e, 2)*pow(f, 7)-mpz_class("162703559429611200")*pow(e, 2)*f-mpz_class("390000914678878")*e*pow(f, 44)+mpz_class("996062704593756434")*e*pow(f, 38)+mpz_class("155886323972034823914")*e*pow(f, 32)+mpz_class("194745956143985421330")*e*pow(f, 26)+mpz_class("6205077595574430")*e*pow(f, 20)-mpz_class("194596512653299068786")*e*pow(f, 14)-mpz_class("155796897940756922666")*e*pow(f, 8)-mpz_class("1036375759077320978")*e*pow(f, 2)-mpz_class("374998630035991")*pow(f, 45)+mpz_class("957747106595453993")*pow(f, 39)+mpz_class("149889155566764891693")*pow(f, 33)+mpz_class("187154171443494641685")*pow(f, 27)-mpz_class("127129015426348065")*pow(f, 21)-mpz_class("187241533243115040417")*pow(f, 15)-mpz_class("149719983567976534037")*pow(f, 9)-mpz_class("836654081239648061")*pow(f, 3),
    mpz_class("72152354514240")*c*pow(f, 6)-mpz_class("72152354514240")*c+mpz_class("40950859449")*pow(f, 43)-mpz_class("104588980990367")*pow(f, 37)-mpz_class("16367227395575307")*pow(f, 31)-mpz_class("20268523416527355")*pow(f, 25)+mpz_class("442205002259535")*pow(f, 19)+mpz_class("20576059935789063")*pow(f, 13)+mpz_class("15997133796970563")*pow(f, 7)-mpz_class("275099892785581")*f,
    mpz_class("7936758996566400")*c*e-mpz_class("7936758996566400")*c*f-mpz_class("4418748183673")*e*pow(f, 43)+mpz_class("11285568707456559")*e*pow(f, 37)+mpz_class("1765998617294451019")*e*pow(f, 31)+mpz_class("2173749283622606155")*e*pow(f, 25)-mpz_class("55788292195402895")*e*pow(f, 19)-mpz_class("2215291421788292951")*e*pow(f, 13)-mpz_class("1718142665347430851")*e*pow(f, 7)+mpz_class("30256569458230237")*e*f+mpz_class("4418748183673")*pow(f, 44)-mpz_class("11285568707456559")*pow(f, 38)-mpz_class("1765998617294451019")*pow(f, 32)-mpz_class("2173749283622606155")*pow(f, 26)+mpz_class("55788292195402895")*pow(f, 20)+mpz_class("2215291421788292951")*pow(f, 14)+mpz_class("1718142665347430851")*pow(f, 8)-mpz_class("30256569458230237")*pow(f, 2),
    mpz_class("23810276989699200")*c*d-mpz_class("23810276989699200")*c*f+mpz_class("23810276989699200")*pow(d, 2)+mpz_class("71430830969097600")*d*f+mpz_class("7936758996566400")*pow(e, 5)*pow(f, 3)-mpz_class("31747035986265600")*pow(e, 4)*pow(f, 4)+mpz_class("31747035986265600")*pow(e, 3)*pow(f, 5)+mpz_class("404774708824886400")*pow(e, 2)*pow(f, 6)+mpz_class("396837949828320000")*pow(e, 2)-mpz_class("1247372229446701")*e*pow(f, 43)+mpz_class("3185785654596621203")*e*pow(f, 37)+mpz_class("498594866849974751463")*e*pow(f, 31)+mpz_class("624542545845791047935")*e*pow(f, 25)+mpz_class("931085755769682885")*e*pow(f, 19)-mpz_class("624150663582417063387")*e*pow(f, 13)-mpz_class("499881859388360475647")*e*pow(f, 7)-mpz_class("3926885313819527351")*e*f-mpz_class("7026011547118141")*pow(f, 44)+mpz_class("17944427051950691243")*pow(f, 38)+mpz_class("2808383522593986603543")*pow(f, 32)+mpz_class("3513624142354807530135")*pow(f, 26)+mpz_class("2860757006705537685")*pow(f, 20)-mpz_class("3511356735642190737267")*pow(f, 14)-mpz_class("2811332494697103819887")*pow(f, 8)-mpz_class("20315011631522847311")*pow(f, 2),
    mpz_class("801692827936")*pow(c, 3)+mpz_class("2405078483808")*pow(c, 2)*f-mpz_class("2405078483808")*c*pow(f, 2)-mpz_class("13752945467")*pow(f, 45)+mpz_class("35125117815561")*pow(f, 39)+mpz_class("5496946957826433")*pow(f, 33)+mpz_class("6834659447749117")*pow(f, 27)-mpz_class("44484880462461")*pow(f, 21)-mpz_class("6873406230093057")*pow(f, 15)-mpz_class("5450844938762633")*pow(f, 9)+mpz_class("1216586044571")*pow(f, 3),
    mpz_class("11905138494849600")*b*pow(f, 6)-mpz_class("11905138494849600")*b-mpz_class("15873517993132800")*pow(e, 5)*pow(f, 2)+mpz_class("39683794982832000")*pow(e, 4)*pow(f, 3)-mpz_class("39683794982832000")*pow(e, 3)*pow(f, 4)-mpz_class("686529653202993600")*pow(e, 2)*pow(f, 11)-mpz_class("607162063237329600")*pow(e, 2)*pow(f, 5)+mpz_class("65144531306704")*e*pow(f, 42)-mpz_class("166381280901088652")*e*pow(f, 36)-mpz_class("26033434502470283472")*e*pow(f, 30)-mpz_class("31696259583860650140")*e*pow(f, 24)+mpz_class("971492093167581360")*e*pow(f, 18)+mpz_class("32220085033691389548")*e*pow(f, 12)+mpz_class("25526177666070529808")*e*pow(f, 6)+mpz_class("138603268355749244")*e+mpz_class("167620036074811")*pow(f, 43)-mpz_class("428102417974791473")*pow(f, 37)-mpz_class("66997243801231679313")*pow(f, 31)-mpz_class("83426716722148750485")*pow(f, 25)+mpz_class("203673895369980765")*pow(f, 19)+mpz_class("83523056326010432457")*pow(f, 13)+mpz_class("66995789640238066937")*pow(f, 7)+mpz_class("478592855549587901")*f,
    mpz_class("11905138494849600")*b*e-mpz_class("11905138494849600")*b*f-mpz_class("3968379498283200")*pow(e, 5)*pow(f, 3)+mpz_class("15873517993132800")*pow(e, 4)*pow(f, 4)-mpz_class("27778656487982400")*pow(e, 3)*pow(f, 5)-mpz_class("208339923659868000")*pow(e, 2)*pow(f, 6)-mpz_class("240086959646133600")*pow(e, 2)+mpz_class("786029984751110")*e*pow(f, 43)-mpz_class("2007519008182245250")*e*pow(f, 37)-mpz_class("314188062908073807090")*e*pow(f, 31)-mpz_class("393423667537929575250")*e*pow(f, 25)-mpz_class("550329120654394950")*e*pow(f, 19)+mpz_class("393196408728889612770")*e*pow(f, 13)+mpz_class("314892372799176495730")*e*pow(f, 7)+mpz_class("2409386515146668530")*e*f+mpz_class("4177638546747827")*pow(f, 44)-mpz_class("10669685294602576381")*pow(f, 38)-mpz_class("1669852980419949524601")*pow(f, 32)-mpz_class("2089077057287904170745")*pow(f, 26)-mpz_class("1569899763580278795")*pow(f, 20)+mpz_class("2087864026859015573349")*pow(f, 14)+mpz_class("1671496085945199577969")*pow(f, 8)+mpz_class("11940257226216280177")*pow(f, 2),
    mpz_class("7936758996566400")*b*d-mpz_class("7936758996566400")*b*f-mpz_class("7936758996566400")*d*f-mpz_class("7936758996566400")*pow(e, 5)*pow(f, 3)+mpz_class("23810276989699200")*pow(e, 4)*pow(f, 4)-mpz_class("23810276989699200")*pow(e, 3)*pow(f, 5)-mpz_class("337312257354072000")*pow(e, 2)*pow(f, 6)-mpz_class("369059293340337600")*pow(e, 2)+mpz_class("1176345388640471")*e*pow(f, 43)-mpz_class("3004383582891473073")*e*pow(f, 37)-mpz_class("470203502707246105653")*e*pow(f, 31)-mpz_class("588858183402644348085")*e*pow(f, 25)-mpz_class("856939308623513535")*e*pow(f, 19)+mpz_class("588472674242340526377")*e*pow(f, 13)+mpz_class("471313241958371103517")*e*pow(f, 7)+mpz_class("3659742549078552381")*e*f+mpz_class("6423170513956901")*pow(f, 44)-mpz_class("16404772137036480803")*pow(f, 38)-mpz_class("2567419165227528774463")*pow(f, 32)-mpz_class("3211938090825682172335")*pow(f, 26)-mpz_class("2330490332697587485")*pow(f, 20)+mpz_class("3210100109444754864587")*pow(f, 14)+mpz_class("2569858315395162617847")*pow(f, 8)+mpz_class("18326089487427735751")*pow(f, 2),
    mpz_class("23810276989699200")*b*c-mpz_class("23810276989699200")*b*f+mpz_class("23810276989699200")*pow(c, 2)+mpz_class("71430830969097600")*c*f-mpz_class("23810276989699200")*pow(d, 2)-mpz_class("95241107958796800")*d*f-mpz_class("55557312975964800")*pow(e, 5)*pow(f, 3)+mpz_class("174608697924460800")*pow(e, 4)*pow(f, 4)-mpz_class("174608697924460800")*pow(e, 3)*pow(f, 5)-mpz_class("2428648252949318400")*pow(e, 2)*pow(f, 6)-mpz_class("2611193709870345600")*pow(e, 2)+mpz_class("8305444561289527")*e*pow(f, 43)-mpz_class("21212087151945459641")*e*pow(f, 37)-mpz_class("3319815883093451385381")*e*pow(f, 31)-mpz_class("4157691646261657136445")*e*pow(f, 25)-mpz_class("6072721607510764095")*e*pow(f, 19)+mpz_class("4154986709036460221649")*e*pow(f, 13)+mpz_class("3327761311138587096749")*e*pow(f, 7)+mpz_class("25885340608290841637")*e*f+mpz_class("45815897629010329")*pow(f, 44)-mpz_class("117013765582151891207")*pow(f, 38)-mpz_class("18313166848970865074187")*pow(f, 32)-mpz_class("22909971239649297438915")*pow(f, 26)-mpz_class("16133250761305157265")*pow(f, 20)+mpz_class("22897305857636178256623")*pow(f, 14)+mpz_class("18329944781867242497923")*pow(f, 8)+mpz_class("130258531002020420699")*pow(f, 2),
    mpz_class("3968379498283200")*pow(b, 2)+mpz_class("15873517993132800")*b*f+mpz_class("3968379498283200")*pow(d, 2)+mpz_class("15873517993132800")*d*f+mpz_class("3968379498283200")*pow(e, 5)*pow(f, 3)-mpz_class("15873517993132800")*pow(e, 4)*pow(f, 4)+mpz_class("23810276989699200")*pow(e, 3)*pow(f, 5)+mpz_class("206355733910726400")*pow(e, 2)*pow(f, 6)+mpz_class("230166010900425600")*pow(e, 2)-mpz_class("729705987316687")*e*pow(f, 43)+mpz_class("1863667496867205421")*e*pow(f, 37)+mpz_class("291674853771731104461")*e*pow(f, 31)+mpz_class("365285994691106921745")*e*pow(f, 25)+mpz_class("549961185828911895")*e*pow(f, 19)-mpz_class("365048404038768439269")*e*pow(f, 13)-mpz_class("292382820431504027669")*e*pow(f, 7)-mpz_class("2271898467631865497")*e*f-mpz_class("3988812642545399")*pow(f, 44)+mpz_class("10187423878429609997")*pow(f, 38)+mpz_class("1594377523424314053637")*pow(f, 32)+mpz_class("1994739308439916238065")*pow(f, 26)+mpz_class("1596840088052642815")*pow(f, 20)-mpz_class("1993494118301162145413")*pow(f, 14)-mpz_class("1596049742289689815053")*pow(f, 8)-mpz_class("11488171330159667449")*pow(f, 2),
    a+b+c+d+e+f
  }), output);
}
