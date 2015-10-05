#include <gtest/gtest.h>

#include "moGVW.h"

TEST(moGVWTest, LCMCriterion) {
  typedef Monomial<> M;
  typedef Term<> T;
  typedef Polynomial<> P;
  typedef moGVWRunner<>::LMSet LMSet;
  moGVWRunner<> runner;

  T a = M::x(0);
  T b = M::x(1);
  T c = M::x(2);

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
  //r LabelledMonomial<> abc_r1 = LabelledMonomial<>((a*b*c).m(), r1);
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

  LMSet GG = { ab_r2, bc_r3, bcc_r3, bbc_r3, abc_r3, c_r4, cc_r4, ac_r4, acc_r4 };

  EXPECT_TRUE(runner.rejectedByLCMCriterion(abc_r4, GG));
}
