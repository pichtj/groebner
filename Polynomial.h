#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>

#include "Term.h"

template<class C = int, class E = char>
class Polynomial {
public:
  typedef Monomial<E> MonomialType;
  typedef Term<C, E> TermType;
  typedef Polynomial<C, E> This;
  Polynomial() : pd(0) {}
  Polynomial(const C& c) : pd(new PolynomialData(TermType(c))) {}
  Polynomial(const TermType& t) : pd(new PolynomialData(t)) {}
  Polynomial(const This& other) : pd(0) {
    if (!other.pd) return;
    pd = new PolynomialData(other.pd->term);
    PolynomialData* c = pd;
    PolynomialData* oc = other.pd->next;
    while (oc) {
      c->next = new PolynomialData(oc->term);
      c = c->next;
      oc = oc->next;
    }
  }
  ~Polynomial() { deleteAll(); }
  TermType lterm() const { return pd->term; }
  C lc() const { if (pd) { return pd->term.coefficient(); } else { return C(); } }
  Monomial<E> lm() const { if (pd) { return pd->term.exponent(); } else { return Monomial<E>(); } }
  bool isZero() const { return pd == 0; }
  This& operator+=(const C& c) { *this += TermType(c); return *this; }
  This operator+(const C& c) const { This r = *this; r += c; return r; }
  This& operator-=(const C& c) { *this += (-1) * c; return *this; }
  This operator-(const C& c) const { return *this + (-1) * c; }
  This& operator+=(const TermType& t) {
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
  This operator+(const TermType& t) const { This r = *this; r += other; return r; }
  This& operator-=(const TermType& t) { *this += (-1) * t; return *this; }
  This operator-(const TermType& t) const { This r = *this; r += other; return r; }
  This& operator+=(const This& other) {
    const PolynomialData* b = other.pd;
    while (b) {
      operator+=(b->term);
      b = b->next;
    }
    return *this;
  }
  This operator+(const This& other) const { This r = *this; r += other; return r; }

  This& operator*=(const C& c) {
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
  This operator*(const MonomialType& m) const {
    This result = *this;
    result *= m;
    return result;
  }
  This& operator*=(const TermType& t) {
    PolynomialData* current = pd;
    while (current) {
      current->term *= t;
      current = current->next;
    }
    return *this;
  }
  This operator*(const TermType& t) const {
    This result = *this;
    result *= t;
    return result;
  }
  This& operator*=(const This& other) {
    This newMe = *this * other;
    deleteAll();
    *this += newMe;
  }
  This operator*(const This& other) const {
    This result;
    PolynomialData* current = other.pd;
    while (current) {
      This p = *this * current->term;
      result += p;
      current = current->next;
    }
    return result;
  }
  bool operator==(const This& other) const;
  template<class C1, class E1>
  friend std::ostream& operator<<(std::ostream& out, const Polynomial<C1, E1>& p);
private:
  void deleteAll() {
    PolynomialData* last = pd;
    while (last) {
      pd = last->next;
      delete last;
      last = pd;
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
Polynomial<C, E> operator*(const Monomial<E>& m, const Polynomial<C, E>& p) {
  return p * m;
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
