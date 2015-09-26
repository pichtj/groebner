#include <cstdlib>
#include "LabelledMonomial.h"
using namespace std;

template<class E>
void printExponent(const Exponent<E>& exp) {
  printf("^{");
  for (int i = 0; i < VAR_COUNT; ++i) printf("%i ", exp[i]);
  printf("}\n");
}

template<class C, class E>
void printPolynomial(const Polynomial<C, E>& p) {
  
}

int main(int argc, char* argv[]) {
  Exponent<short> exp;
  exp[23] = 17;
  auto exp2 = exp * exp;
  printExponent(exp2/exp/exp);
  printf("sizeof(Exponent<char>) = %i\n", sizeof(Exponent<char>));
  printf("sizeof(Exponent<short>) = %i\n", sizeof(Exponent<short>));
  printf("sizeof(Exponent<int>) = %i\n", sizeof(Exponent<int>));
  printf("sizeof(Exponent<long>) = %i\n", sizeof(Exponent<long>));
  return 0;
}
