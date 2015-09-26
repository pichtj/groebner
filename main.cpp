#include <mpirxx.h>

#include <cstdlib>
#include "LabelledMonomial.h"
using namespace std;

void print(int i) {
  printf("%i", i);
}

template<class E>
void print(const Exponent<E>& exp) {
  printf("{");
  for (int i = 0; i < VAR_COUNT; ++i) {
    print(exp[i]);
    if (i < VAR_COUNT - 1) printf(" ");
  }
  printf("}");
}

template<class C, class E>
void print(const Term<C, E>& t) {
  print(t.coeff);
  printf("*x^");
  print(t.exp);
}

template<class C, class E>
void print(const Polynomial<C, E>& p) {
  
}

int main(int argc, char* argv[]) {
  mpir_ui a;
  Exponent<char> exp;
  exp[23] = 17;
  auto exp2 = exp * exp;
  print(exp2/exp/exp);
  printf("\n");
  printf("sizeof(mpz_t) = %i\n", sizeof(mpz_t));
  printf("sizeof(mpz_class) = %i\n", sizeof(mpz_class));
  printf("sizeof(Polynomial<mpz_class, char>) = %i\n", sizeof(Polynomial<mpz_class, char>));
  printf("sizeof(Polynomial<mpz_class, short>) = %i\n", sizeof(Polynomial<mpz_class, short>));
  printf("sizeof(Polynomial<mpz_class, int>) = %i\n", sizeof(Polynomial<mpz_class, int>));
  printf("sizeof(Polynomial<mpz_class, long>) = %i\n", sizeof(Polynomial<mpz_class, long>));

  Term<int,char> t;
  t.coeff = 23;
  t.exp = exp;
  print(t);
  printf("\n");
  return 0;
}
