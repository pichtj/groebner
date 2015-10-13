#include <gtest/gtest.h>

#define VAR_COUNT 3

#include "moGVW.h"

std::string abc_var_names(uint index) {
  const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
  return std::string(alphabet + index, alphabet + index + 1);
}

TEST(moGVWTest, LCMCriterion) {
  get_var_name = abc_var_names;
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

  get_var_name = default_get_var_name;
}

TEST(moGVWTest, lift) {
  get_var_name = abc_var_names;
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

  cout << "GG = {" << endl;
  for (auto it = GG.begin(); it != GG.end(); ++it) {
    cout << *it << endl;
  }
  cout << "}" << endl;

  cout << "HH = {" << endl;
  for (auto it = HH.begin(); it != HH.end(); ++it) {
    cout << *it << endl;
  }
  cout << "}" << endl;

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

  get_var_name = default_get_var_name;
}

TEST(moGVWTest, moGVW) {
  get_var_name = abc_var_names;
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef moGVWRunner<>::LMSet LMSet;
  typedef moGVWRunner<>::MMSet MMSet;
  typedef vector<P> PSet;
  moGVWRunner<> runner;

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  P f1 = a*b*c - 1;
  P f2 = a*b - c;
  P f3 = b*c - b;

  PSet input = { f1, f2, f3 };

  PSet output = runner.moGVW(input);

  cout << "output = {" << endl;
  for (auto it = output.begin(); it != output.end(); ++it) {
    cout << *it << endl;
  }
  cout << "}" << endl;

  EXPECT_EQ(PSet({ a*b - c, b*c - b, c - 1, -c*c + c }), output);

  get_var_name = default_get_var_name;
}
