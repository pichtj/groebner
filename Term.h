#include "Exponent.h"

template<class C = int, class E = char>
class Term {
public:
  Term() : coeff(), exp() {}
  Term(const C& c) : coeff(c), exp() {}
  Term(const C& c, const Exponent<E>& e) : coeff(c), exp(e) {}
  Term<C, E>& operator*=(const Term<C, E>& t) {
    coeff *= t.coeff;
    exp *= t.exp;
    return *this;
  }
  C coeff;
  Exponent<E> exp;
  unsigned int degree() { return exp.degree(); }
};
