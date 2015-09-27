#ifndef PRINT_H
#define PRINT_H

#include <cstdlib>

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
  p.doPrint();
}

#endif /* PRINT_H */
