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
  Term(const C& c, const M& e) : coeff(c), exp(e) {}

  bool operator==(const This& b) const {
    return m() == b.m() && c() == b.c();
  }
  bool operator!=(const This& b) const { return !operator==(b); }

  This& operator*=(const This& t) {
    if (!isZero()) {
      if (t.isZero()) {
        coeff = C();
        exp = M();
      }
      coeff *= t.coeff;
      exp *= t.exp;
    }
    return *this;
  }
  This& operator/=(const This& t) {
    if (!isZero()) {
      if (t.isZero()) {
        throw std::domain_error("division by zero");
      }
      coeff /= t.coeff;
      exp /= t.exp;
    }
    return *this;
  }
  This& operator*=(const M& m) {
    if (!isZero()) {
      exp *= m;
    }
    return *this;
  }
  This operator-() const { This r = *this; r *= -1; return r; }
  C c() const { return coeff; }
  C& c() { return coeff; }
  M m() const { return exp; }
  uint degree() { return exp.degree(); }
  bool isZero() const { return coeff == 0; }
  bool isOne() const { return coeff == 1 && exp.isConstant(); }
  bool isConstant() const { return coeff == 0 || exp.isConstant(); }
private:
  C coeff;
  M exp;
};

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
  Term<C, M> r = a;
  r *= b;
  return r;
}

template<class C, class M>
Term<C, M> pow(const Term<C, M>& base, uint exp) {
  Term<C, M> result(C(1));
  for (uint i = 0; i < exp; ++i) {
    result *= base;
  }
  return result;
}

template<class C, class M>
std::ostream& operator<<(std::ostream& out, const Term<C, M>& t) {
  if (t.c() == 0) return out << "0";
  if (t.c() == 1) return out << t.m();
  if (t.c() == -1) return out << "-" << t.m();
  if (t.m() == M()) return out << t.c();
  return out << t.c() << "*" << t.m();
}

template<class C, class M>
std::istream& operator>>(std::istream& in, Term<C, M>& t) {
  auto next = in.peek();
  C coefficient = C(1);
  if (next == '+' || next == '-' || std::isdigit(next)) {
    in >> coefficient;
    next = in.peek();
    if (next != '*') {
      t = Term<C, M>(coefficient);
      return in;
    } else {
      in.get();
      next = in.peek();
    }
  }
  M monomial;
  in >> monomial;
  if (coefficient == C(0)) {
    t = Term<C, M>();
  } else {
    t = Term<C, M>(coefficient, monomial);
  }
  D("read term " << t);
  return in;
}

#endif // TERM_H
