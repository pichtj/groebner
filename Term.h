#ifndef TERM_H
#define TERM_H

#include <ostream>

#include "style.h"
#include "Monomial.h"

template<class C = int, class M = Monomial<char> >
class Term {
public:
  typedef C CoefficientType;
  typedef M MonomialType;
  typedef Term<C, M> This;

  Term() : coeff(), exp() {}
  Term(const C& c) : coeff(c), exp() {}
  Term(const M& e) : coeff(1), exp(e) {}
  Term(const C& c, const M& e) : coeff(c), exp(e) {}
  This& operator*=(const This& t) {
    coeff *= t.coeff;
    exp *= t.exp;
    return *this;
  }
  This& operator*=(const M& e) {
    exp *= e;
    return *this;
  }
  C coefficient() const { return coeff; }
  C& coefficient() { return coeff; }
  M exponent() const { return exp; }
  uint degree() { return exp.degree(); }
private:
  C coeff;
  M exp;
};

template<class C, class M>
bool operator==(const Term<C, M>& a, const Term<C, M>& b) {
  return a.exponent() == b.exponent() && a.coefficient() == b.coefficient();
}

template<class C, class M>
Term<C, M> operator*(const C& c, const Term<C, M>& t) {
  return t * c;
}

template<class C, class M>
Term<C, M> operator*(const Term<C, M>& t, const C& c) {
  return t * Term<C, M>(c);
}

template<class C, class M>
Term<C, M> operator*(const Term<C, M>& a, const Term<C, M>& b) {
  Term<C, M> result = a;
  result *= b;
  return result;
}

template<class C, class M>
Term<C, M> pow(const Term<C, M>& base, uint exp) {
  Term<C, M> result(1);
  for (uint i = 0; i < exp; ++i) {
    result *= base;
  }
  return result;
}

template<class C, class M>
std::ostream& operator<<(std::ostream& out, const Term<C, M>& t) {
  out << t.coefficient() << "*x^" << t.exponent();
}

#endif // TERM_H
