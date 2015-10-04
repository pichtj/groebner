#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>

#include "Term.h"

template<class C = int, class E = char>
class Polynomial {
public:
  typedef Monomial<E> MonomialType;
  typedef Term<C, E> TermType;
  typedef Polynomial<C, E> PolynomialType;
  Polynomial() : pd(0) {}
  Polynomial(const C& c) : pd(new PolynomialData(TermType(c))) {}
  Polynomial(const TermType& t) : pd(new PolynomialData(t)) {}
  Polynomial(const PolynomialType& other) : pd(0) {
    *this += other;
  }
  ~Polynomial() { deleteAll(); }
  TermType lterm() const { return pd->term; }
  C lc() const { if (pd) { return pd->term.coefficient(); } else { return C(); } }
  Monomial<E> lm() const { if (pd) { return pd->term.exponent(); } else { return Monomial<E>(); } }
  bool isZero() const { return pd == 0; }
  PolynomialType& operator+=(const TermType& t) {
    if (pd == 0 || pd->term.exponent() < t.exponent()) {
      PolynomialData* new_pd = new PolynomialData(t);
      new_pd->next = pd;
      pd = new_pd;
      return *this;
    }

    PolynomialData* current = pd;
    PolynomialData* before = 0;

    while (current->term.exponent() > t.exponent() && current->next) {
      before = current;
      current = current->next;
    }

    if (current->term.exponent() == t.exponent()) {
      current->term.coefficient() += t.coefficient();
      if (current->term.coefficient() == 0) {
        if (before) {
          before->next = current->next;
          delete current;
        } else {
          PolynomialData* old_pd = pd;
          pd = pd->next;
          delete old_pd;
        }
      }
    } else {
      PolynomialData* after = current->next;
      current->next = new PolynomialData(t);
      current->next->next = after;
    }
    
    return *this;
  }
  PolynomialType operator+(const PolynomialType& other) const {
    Polynomial<C, E> result = Polynomial<C, E>();
    result += *this;
    result += other;
    return result;
  }
  PolynomialType& operator+=(const PolynomialType& other) {
    const PolynomialData* b = other.pd;
    while (b) {
      operator+=(b->term);
      b = b->next;
    }
    return *this;
  }
  PolynomialType& operator*=(const C& c) {
    if (c == C()) {
      deleteAll();
      return *this;
    }
    PolynomialData* current = pd;
    while (current) {
      current->term *= c;
      current = current->next;
    }
    return *this;
  }
  PolynomialType& operator*=(const TermType& t) {
    PolynomialData* current = pd;
    while (current) {
      current->term *= t;
      current = current->next;
    }
    return *this;
  }
  PolynomialType operator*(const TermType& t) const {
    PolynomialType result;
    result += *this;
    result *= t;
    return result;
  }
  PolynomialType& operator*=(const PolynomialType& other) {
    PolynomialType newMe = *this * other;
    deleteAll();
    *this += newMe;
  }
  PolynomialType operator*(const PolynomialType& other) const {
    PolynomialType result;
    PolynomialData* current = other.pd;
    while (current) {
      PolynomialType p = *this * current->term;
      result += p;
      current = current->next;
    }
    return result;
  }
  bool operator==(const PolynomialType& other) const;
  template<class C1, class E1>
  friend std::ostream& operator<<(std::ostream& out, const Polynomial<C1, E1>& p);
private:
  void deleteAll() {
    PolynomialData* last = pd;
    while (last) {
      PolynomialData* p = last->next;
      delete last;
      last = p;
    }
    pd = 0;
  }
  struct PolynomialData {
    PolynomialData(const TermType& t) : next(0), term(t) {}
    PolynomialData* next;
    TermType term;
    friend std::ostream& operator<<(std::ostream& out, const PolynomialData& pd) {
      out << pd.term;
      if (pd.next) {
        out << "+" << *(pd.next);
      }
    }
  };

  Polynomial(PolynomialData* polynomialData) : pd(polynomialData) {}
  PolynomialData* pd;
};

template<class C, class E>
Polynomial<C, E> operator+(const Term<C, E>& a, const Term<C, E>& b) {
  Polynomial<C, E> result(a);
  result += b;
  return result;
}

template<class C, class E>
Polynomial<C, E> operator+(const Term<C, E>& a, const C& b) {
  Polynomial<C, E> result(a);
  result += Term<C, E>(b);
  return result;
}

template<class C, class E>
bool Polynomial<C, E>::operator==(const Polynomial<C, E>& other) const {
  PolynomialData* ca = pd;
  PolynomialData* cb = other.pd;
  while (ca != 0 && cb != 0) {
    if (ca->term != cb->term) {
      return false;
    }
    ca = ca->next;
    cb = cb->next;
  }
  return ca == 0 && cb == 0;
}

template<class C, class E>
std::ostream& operator<<(std::ostream& out, const Polynomial<C, E>& p) {
  if (p.pd) {
    out << *(p.pd);
  } else {
    out << "0";
  }
  return out;
}

#endif // POLYNOMIAL_H
