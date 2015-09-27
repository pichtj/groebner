#include <mpirxx.h>

#include <cstdlib>
#include "LabelledMonomial.h"
using namespace std;

#include "print.h"

int main(int argc, char* argv[]) {
  mpir_ui a;
  Exponent<char> exp;
  exp[23] = 17;
  auto exp2 = exp * exp;
  print(exp2/exp/exp);
  printf("\n");
  printf("sizeof(mpz_t) = %li\n", sizeof(mpz_t));
  printf("sizeof(mpz_class) = %li\n", sizeof(mpz_class));
  printf("sizeof(Polynomial<mpz_class, char>) = %li\n", sizeof(Polynomial<mpz_class, char>));
  printf("sizeof(Polynomial<mpz_class, short>) = %li\n", sizeof(Polynomial<mpz_class, short>));
  printf("sizeof(Polynomial<mpz_class, int>) = %li\n", sizeof(Polynomial<mpz_class, int>));
  printf("sizeof(Polynomial<mpz_class, long>) = %li\n", sizeof(Polynomial<mpz_class, long>));

  Term<int,char> t;
  t.coeff = 23;
  t.exp = exp;
  print(t);
  printf("\n");
  return 0;
}
