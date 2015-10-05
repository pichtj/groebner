#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>

#include "Term.h"

template<class T = Term<int, Monomial<char> > >
class Polynomial {
public:
  typedef typename T::CoefficientType CoefficientType;
  typedef typename T::MonomialType MonomialType;
  typedef T TermType;
  typedef Polynomial<T> This;
  Polynomial() : pd(0) {}
  Polynomial(const CoefficientType& c) : pd(new PolynomialData(T(c))) {}
  Polynomial(const T& t) : pd(new PolynomialData(t)) {}
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
  T lterm() const { return pd->term; }
  CoefficientType lc() const { if (pd) { return pd->term.c(); } else { return CoefficientType(); } }
  MonomialType lm() const { if (pd) { return pd->term.m(); } else { return MonomialType(); } }
  bool isZero() const { return pd == 0; }
  This& operator+=(const CoefficientType& c) { *this += T(c); return *this; }
  This operator+(const CoefficientType& c) const { This r = *this; r += c; return r; }
  This& operator-=(const CoefficientType& c) { *this += (-1) * c; return *this; }
  This operator-(const CoefficientType& c) const { return *this + (-1) * c; }
  This& operator+=(const T& t) {
    if (pd == 0 || pd->term.m() < t.m()) {
      PolynomialData* new_pd = new PolynomialData(t);
      new_pd->next = pd;
      pd = new_pd;
      return *this;
    }

    PolynomialData* current = pd;
    PolynomialData* before = 0;

    while (current->term.m() > t.m() && current->next) {
      before = current;
      current = current->next;
    }

    if (current->term.m() == t.m()) {
      current->term.c() += t.c();
      if (current->term.c() == 0) {
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
  This operator+(const T& t) const { This r = *this; r += t; return r; }
  This& operator-=(const T& t) { *this += (-1) * t; return *this; }
  This operator-(const T& t) const { This r = *this; r += t; return r; }
  This& operator+=(const This& other) {
    const PolynomialData* b = other.pd;
    while (b) {
      operator+=(b->term);
      b = b->next;
    }
    return *this;
  }
  This operator+(const This& other) const { This r = *this; r += other; return r; }

  This& operator*=(const CoefficientType& c) {
    if (c == CoefficientType()) {
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
  This operator-() const { This r = *this; r *= -1; return r; }
  This operator*(const CoefficientType& c) { This r = *this; r *= c; return r; }
  This& operator*=(const T& t) {
    PolynomialData* current = pd;
    while (current) {
      current->term *= t;
      current = current->next;
    }
    return *this;
  }
  This operator*(const T& t) const { This r = *this; r *= t; return r; }
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
  template<class T1>
  friend std::ostream& operator<<(std::ostream& out, const Polynomial<T1>& p);
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
    PolynomialData(const T& t) : next(0), term(t) {}
    PolynomialData* next;
    T term;
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

template<class T>
Polynomial<T> operator+(const T& a, const typename T::CoefficientType& b) {
  Polynomial<T> r(a);
  r += T(b);
  return r;
}

template<class T>
Polynomial<T> operator+(const typename T::CoefficientType& a, const T& b) { return b + a; }

template<class T>
Polynomial<T> operator+(const T& a, const T& b) {
  Polynomial<T> r(a);
  r += b;
  return r;
}

template<class T>
Polynomial<T> operator-(const T& a, const typename T::CoefficientType& b) {
  Polynomial<T> r(a);
  r -= T(b);
  return r;
}

template<class T>
Polynomial<T> operator-(const typename T::CoefficientType& a, const T& b) { return b - a; }

template<class T>
Polynomial<T> operator-(const T& a, const T& b) {
  Polynomial<T> r(a);
  r -= b;
  return r;
}

template<class T>
Polynomial<T> operator*(const typename T::CoefficientType& a, const Polynomial<T>& b) { return b * a; }

template<class T>
Polynomial<T> operator*(const T& a, const Polynomial<T>& b) { return b * a; }

template<class T>
bool Polynomial<T>::operator==(const Polynomial<T>& other) const {
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

template<class T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
  if (p.pd) {
    out << *(p.pd);
  } else {
    out << "0";
  }
  return out;
}

#endif // POLYNOMIAL_H
