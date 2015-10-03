#ifndef TERM_H
#define TERM_H

#include <ostream>

#include "style.h"
#include "Exponent.h"

template<class C = int, class E = char>
class Term {
public:
  Term() : coeff(), exp() {}
  Term(const C& c) : coeff(c), exp() {}
  Term(const Exponent<E>& e) : coeff(1), exp(e) {}
  Term(const C& c, const Exponent<E>& e) : coeff(c), exp(e) {}
  Term<C, E>& operator*=(const Term<C, E>& t);
  Term<C, E>& operator*=(const Exponent<E>& e);
  C coefficient() const { return coeff; }
  C& coefficient() { return coeff; }
  Exponent<E> exponent() const { return exp; }
  uint degree() { return exp.degree(); }
private:
  C coeff;
  Exponent<E> exp;
};

template<class C, class E>
Term<C, E>& Term<C, E>::operator*=(const Term<C, E>& t) {
  coeff *= t.coeff;
  exp *= t.exp;
  return *this;
}

template<class C, class E>
Term<C, E>& Term<C, E>::operator*=(const Exponent<E>& e) {
  exp *= e;
  return *this;
}

template<class C, class E>
bool operator==(const Term<C, E>& a, const Term<C, E>& b) {
  return a.exponent() == b.exponent() && a.coefficient() == b.coefficient();
}

template<class C, class E>
Term<C, E> operator*(const C& c, const Term<C, E>& t) {
  return t * c;
}

template<class C, class E>
Term<C, E> operator*(const Term<C, E>& t, const C& c) {
  return t * Term<C, E>(c);
}

template<class C, class E>
Term<C, E> operator*(const Term<C, E>& a, const Term<C, E>& b) {
  Term<C, E> result = a;
  result *= b;
  return result;
}

template<class C, class E>
Term<C, E> pow(const Term<C, E>& base, uint exp) {
  Term<C, E> result(1);
  for (uint i = 0; i < exp; ++i) {
    result *= base;
  }
  return result;
}

template<class C, class E>
std::ostream& operator<<(std::ostream& out, const Term<C, E>& t) {
  out << t.coefficient() << "*x^" << t.exponent();
}

#endif // TERM_H
