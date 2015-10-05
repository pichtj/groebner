#include <gtest/gtest.h>

#include "moGVW.h"

TEST(moGVWTest, LCMCriterion) {
  moGVWRunner<> runner;
  typedef Monomial<> MType;
  typedef Term<> TType;
  typedef Polynomial<> PType;

  MType a = MType::x(0);
  MType b = MType::x(1);
  MType c = MType::x(2);

//  PType f1 = a*b*c + TType(-1);
//  PType f2 = a*b + (-1*c);
//  PType f3 = b*c + (-1*b);
}
