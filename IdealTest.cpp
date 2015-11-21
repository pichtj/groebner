#include <gtest/gtest.h>
#include <sstream>

#include "Ideal.h"
#include "Polynomial.h"

using namespace std;

TEST(IdealTest, Input) {
  get_var_name = var_name;

  typedef Polynomial<> P;
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;

  stringstream ss("a,b,c\na,b*c,a+b+c");

  auto ideal = read_input<P>(ss);

  T a = T(1, M::x(0));
  T b = T(1, M::x(1));
  T c = T(1, M::x(2));

  EXPECT_EQ(vector<P>({a,b*c,a+b+c}), ideal);
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
