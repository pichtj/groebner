#include <gtest/gtest.h>

#include "moGVW.h"

std::string abc_var_names(uint index) {
  const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
  return std::string(alphabet + index, alphabet + index + 1);
}

struct use_abc_var_names {
  use_abc_var_names() { get_var_name = abc_var_names; }
  ~use_abc_var_names() { get_var_name = default_get_var_name; }
};

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

  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  MM<>r1 = MM<>(e1, f1);
  MM<>r2 = MM<>(e2, f2);
  MM<>r3 = MM<>(e3, f3);
  MM<>r4 = MM<>(e1 - e2 - a*e3, c - 1);

  // initially
  LabelledMonomial<> abc_r1 = LabelledMonomial<>((a*b*c).m(), r1);
  LabelledMonomial<> ab_r2 = LabelledMonomial<>((a*b).m(), r2);
  LabelledMonomial<> bc_r3 = LabelledMonomial<>((b*c).m(), r3);

  // LOOP 1
  LabelledMonomial<> bcc_r3 = LabelledMonomial<>((b*c*c).m(), r3);
  LabelledMonomial<> bbc_r3 = LabelledMonomial<>((b*b*c).m(), r3);
  LabelledMonomial<> abc_r3 = LabelledMonomial<>((a*b*c).m(), r3);
  LabelledMonomial<> c_r4 = LabelledMonomial<>((c).m(), r4);

  // LOOP 2
  LabelledMonomial<> cc_r4 = LabelledMonomial<>((c*c).m(), r4);
  // LabelledMonomial<> bc_r4 = LabelledMonomial<>((b*c).m(), r4);
  LabelledMonomial<> ac_r4 = LabelledMonomial<>((a*c).m(), r4);

  // LOOP 3
  LabelledMonomial<> acc_r4 = LabelledMonomial<>((a*c*c).m(), r4);
  LabelledMonomial<> abc_r4 = LabelledMonomial<>((a*b*c).m(), r4);

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

  EXPECT_TRUE(runner.rejectedByLCMCriterion(abc_r4, GG));
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

  lm_R_l<> e1 = lm_R_l<>::e(0);
  lm_R_l<> e2 = lm_R_l<>::e(1);
  lm_R_l<> e3 = lm_R_l<>::e(2);

  MM<>r1 = MM<>(e1, f1);
  MM<>r2 = MM<>(e2, f2);
  MM<>r3 = MM<>(e3, f3);
  MM<>r4 = MM<>(e1 - e2 - a*e3, c - 1);

  // initially
  LabelledMonomial<> abc_r1 = LabelledMonomial<>((a*b*c).m(), r1);
  LabelledMonomial<> ab_r2 = LabelledMonomial<>((a*b).m(), r2);
  LabelledMonomial<> bc_r3 = LabelledMonomial<>((b*c).m(), r3);

  LMSet GG;
  GG[(a*b*c).m()] = r1;
  GG[(a*b).m()] = r2;
  GG[(b*c).m()] = r3;

  LMSet todo;
  todo[(b*c).m()] = r3;

  MMSet HH = runner.lift(todo, GG);

  // LOOP 1
  LabelledMonomial<> bcc_r3 = LabelledMonomial<>((b*c*c).m(), r3);
  LabelledMonomial<> bbc_r3 = LabelledMonomial<>((b*b*c).m(), r3);
  LabelledMonomial<> abc_r3 = LabelledMonomial<>((a*b*c).m(), r3);
  LabelledMonomial<> c_r4 = LabelledMonomial<>((c).m(), r4);

  // LOOP 2
  LabelledMonomial<> cc_r4 = LabelledMonomial<>((c*c).m(), r4);
  // LabelledMonomial<> bc_r4 = LabelledMonomial<>((b*c).m(), r4);
  LabelledMonomial<> ac_r4 = LabelledMonomial<>((a*c).m(), r4);

  // LOOP 3
  LabelledMonomial<> acc_r4 = LabelledMonomial<>((a*c*c).m(), r4);
  LabelledMonomial<> abc_r4 = LabelledMonomial<>((a*b*c).m(), r4);

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
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef unordered_set<P> PSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  PSet input = { f1, f2, f3 };

  PSet output = runner.moGVW(input);

  EXPECT_EQ(PSet({ a*b - c, b*c - b, -c*c + c, c - 1 }), output);
}

TEST(moGVWTest, cyclic3) {
  use_abc_var_names in_this_scope;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef unordered_set<P> PSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  PSet input = {
    a + b + c,
    a*b + a*c + b*c,
    a*b*c - 1
  };

  PSet output = runner.moGVW(input);

  EXPECT_EQ(PSet({pow(b,3)-1, -a*c+pow(b,2), a+b+c, a*b+a*c+b*c, a*pow(c,2)+b*pow(c,2)+1, a*b*c-1, pow(b,2)*pow(c,2)+b+c, -pow(b,3)*c+c}), output);
}

TEST(moGVWTest, cyclic4) {
  use_abc_var_names in_this_scope;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef unordered_set<P> PSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));

  PSet input = {
    a + b + c + d,
    a*b + b*c + a*d + c*d,
    a*b*c + a*b*d + a*c*d + b*c*d,
    a*b*c*d - 1
  };

  PSet output = runner.moGVW(input);

  EXPECT_EQ(PSet({
    -pow(b,3)-pow(b,2)*d+b*pow(c,2)+pow(c,2)*d,
    a+b+c+d,
    -a*d+pow(b,2)+b*d-c*d,
    a*b*c+a*b*d+a*c*d+b*c*d,
    a*b*c*d-1,
    a*b+a*d+b*c+c*d
  }), output);
}

TEST(moGVWTest, cyclic5) {
  use_abc_var_names in_this_scope;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef unordered_set<P> PSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));
  T d = T(1, M::x(3));
  T e = T(1, M::x(4));

  PSet input = {
    a*b*c*d*e -1,
    a*b*c*d + a*b*c*e + a*b*d*e + a*c*d*e + b*c*d*e,
    a*b*c + a*b*e + a*d*e + b*c*d + c*d*e,
    a*b + a*e + b*c + c*d + d*e,
    a + b + c + d + e
  };

  PSet output = runner.moGVW(input);

  EXPECT_EQ(PSet({
    -pow(b,3)-pow(b,2)*d+b*pow(c,2)+pow(c,2)*d,
    a+b+c+d,
    -a*d+pow(b,2)+b*d-c*d,
    a*b*c+a*b*d+a*c*d+b*c*d,
    a*b*c*d-1,
    a*b+a*d+b*c+c*d
  }), output);
}
