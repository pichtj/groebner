#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>
#include <set>
#include <forward_list>

#include "Term.h"

template<class T = Term<int, Monomial<char> > >
class Polynomial {
public:
  typedef typename T::CoefficientType CoefficientType;
  typedef typename T::MonomialType MonomialType;
  typedef T TermType;
  typedef Polynomial<T> This;
  Polynomial() {}
  Polynomial(const CoefficientType& c) : terms({T(c)}) {}
  Polynomial(const T& t) : terms({t}) {}
  T lterm() const { return terms.front(); }
  CoefficientType lc() const { if (!terms.empty()) { return terms.front().c(); } else { return CoefficientType(); } }
  MonomialType lm() const { if (!terms.empty()) { return terms.front().m(); } else { return MonomialType(); } }
  std::set<MonomialType> monomials() const {
    std::set<MonomialType> r;
    for (auto it = terms.begin(); it != terms.end(); ++it) {
      r.insert(it->m());
    }
    return r;
  }
  bool isZero() const { return terms.empty(); }
  This& operator+=(const CoefficientType& c) { *this += T(c); return *this; }
  This operator+(const CoefficientType& c) const { This r = *this; r += c; return r; }
  This& operator-=(const CoefficientType& c) { *this += (-1) * c; return *this; }
  This operator-(const CoefficientType& c) const { return *this + (-1) * c; }
  This& operator+=(const T& t) {
    if (terms.empty() || lm() < t.m()) {
      terms.insert_after(terms.before_begin(), t);
      return *this;
    }

    auto current = terms.begin();
    auto before = terms.before_begin();

    while (current != terms.end() && current->m() > t.m()) {
      ++before;
      ++current;
    }

    if (current != terms.end() && current->m() == t.m()) {
      current->c() += t.c();
      if (current->c() == 0) {
        terms.erase_after(before);
      }
    } else {
      terms.insert_after(before, t);
    }
    return *this;
  }
  This operator+(const T& t) const { This r = *this; r += t; return r; }
  This& operator-=(const T& t) { *this += (-1) * t; return *this; }
  This operator-(const T& t) const { This r = *this; r += t; return r; }
  This& operator+=(const This& other) {
    for (auto it = other.terms.begin(); it != other.terms.end(); ++it) {
      operator+=(*it);
    }
    return *this;
  }
  This operator+(const This& other) const { This r = *this; r += other; return r; }

  This& operator*=(const CoefficientType& c) {
    if (c == CoefficientType()) {
      terms.clear();
      return *this;
    }
    for (auto it = terms.begin(); it != terms.end(); ++it) {
      *it *= c;
    }
    return *this;
  }
  This operator-() const { This r = *this; r *= -1; return r; }
  This operator*(const CoefficientType& c) { This r = *this; r *= c; return r; }
  This& operator*=(const T& t) {
    if (t.isZero()) {
      terms.clear();
      return *this;
    }
    for (auto it = terms.begin(); it != terms.end(); ++it) {
      *it *= t;
    }
    return *this;
  }
  This operator*(const T& t) const { This r = *this; r *= t; return r; }
  This& operator*=(const MonomialType& m) {
    for (auto it = terms.begin(); it != terms.end(); ++it) {
      *it *= m;
    }
    return *this;
  }
  This operator*(const MonomialType& m) const { This r = *this; r *= m; return r; }
  This& operator*=(const This& other) {
    This newMe = *this * other;
    terms.clear();
    *this += newMe;
  }
  This operator*(const This& other) const {
    This result;
    for (auto it = other.terms.begin(); it != other.terms.end(); ++it) {
      This p = *this * *it;
      result += p;
    }
    return result;
  }
  bool operator==(const This& other) const { return terms == other.terms; }
  template<class T1>
  friend std::ostream& operator<<(std::ostream& out, const Polynomial<T1>& p);
private:
  std::forward_list<TermType> terms;
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
Polynomial<T> operator*(const T& a, const Polynomial<T>& b) { return b * a; }

template<class T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
  bool termPrinted = false;
  for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
    if (termPrinted && it->c() > 0) {
      out << "+";
    }
    out << *it;
    termPrinted = true;
  }
  if (!termPrinted) {
    out << "0";
  }
  return out;
}

#endif // POLYNOMIAL_H
