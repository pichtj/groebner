#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>
#include <set>
#include <forward_list>
#include <stdexcept>

#include "Term.h"

template<class T = Term<int, Monomial<char> > >
class Polynomial {
public:
  typedef typename T::CoefficientType CoefficientType;
  typedef typename T::MonomialType MonomialType;
  typedef T TermType;
  typedef Polynomial<T> This;
  Polynomial() {}
  Polynomial(const CoefficientType& c) : terms_({T(c)}) {}
  Polynomial(const T& t) : terms_({t}) {}
  T lterm() const { return terms_.front(); }
  CoefficientType lc() const { if (!terms_.empty()) { return terms_.front().c(); } else { return CoefficientType(); } }
  MonomialType lm() const { if (!terms_.empty()) { return terms_.front().m(); } else { return MonomialType(); } }

  std::set<MonomialType> monomials() const {
    std::set<MonomialType> r;
    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
      r.insert(it->m());
    }
    return r;
  }
  std::forward_list<TermType> terms() const {
    return terms_;
  }

  TermType operator[](const MonomialType& m) const {
    auto c = terms_.begin();
    while (c->m() >= m) ++c;
    if (c->m() == m) return *c;
    return TermType();
  }
  bool isZero() const { return terms_.empty(); }
  This& operator+=(const CoefficientType& c) { *this += T(c); return *this; }
  This operator+(const CoefficientType& c) const { This r = *this; r += c; return r; }
  This& operator-=(const CoefficientType& c) { *this += (-1) * c; return *this; }
  This operator-(const CoefficientType& c) const { return *this + (-1) * c; }
  This& operator+=(const T& t) {
    if (terms_.empty() || lm() < t.m()) {
      terms_.insert_after(terms_.before_begin(), t);
      return *this;
    }

    auto current = terms_.begin();
    auto before = terms_.before_begin();

    while (current != terms_.end() && current->m() > t.m()) {
      ++before;
      ++current;
    }

    if (current != terms_.end() && current->m() == t.m()) {
      current->c() += t.c();
      if (current->c() == 0) {
        terms_.erase_after(before);
      }
    } else {
      terms_.insert_after(before, t);
    }
    return *this;
  }
  This operator+(const T& t) const { This r = *this; r += t; return r; }
  This& operator-=(const T& t) { *this += (-1) * t; return *this; }
  This operator-(const T& t) const { This r = *this; r += t; return r; }
  This& operator+=(const This& other) {
    for (auto it = other.terms_.begin(); it != other.terms_.end(); ++it) {
      operator+=(*it);
    }
    return *this;
  }
  This operator+(const This& other) const { This r = *this; r += other; return r; }
  This& operator-=(const This& other) {
    for (auto it = other.terms_.begin(); it != other.terms_.end(); ++it) {
      operator-=(*it);
    }
    return *this;
  }
  This operator-(const This& other) const { This r = *this; r -= other; return r; }

  This& operator*=(const CoefficientType& c) {
    if (c == CoefficientType()) {
      terms_.clear();
      return *this;
    }
    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
      *it *= c;
    }
    return *this;
  }
  This operator-() const { This r = *this; r *= -1; return r; }
  This operator*(const CoefficientType& c) const { This r = *this; r *= c; return r; }
  This& operator*=(const T& t) {
    if (t.isZero()) {
      terms_.clear();
      return *this;
    }
    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
      *it *= t;
    }
    return *this;
  }
  This operator*(const T& t) const { This r = *this; r *= t; return r; }
  This& operator*=(const MonomialType& m) {
    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
      *it *= m;
    }
    return *this;
  }
  This operator*(const MonomialType& m) const { This r = *this; r *= m; return r; }
  This& operator*=(const This& other) {
    This newMe = *this * other;
    terms_.clear();
    *this += newMe;
  }
  This operator*(const This& other) const {
    This result;
    for (auto it = other.terms_.begin(); it != other.terms_.end(); ++it) {
      This p = *this * *it;
      result += p;
    }
    return result;
  }
  bool operator==(const This& other) const { return terms_ == other.terms_; }
  template<class T1>
  friend std::ostream& operator<<(std::ostream& out, const Polynomial<T1>& p);
private:
  std::forward_list<TermType> terms_;
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
Polynomial<T> operator*(const typename T::CoefficientType& a, const Polynomial<T>& b) { return b.operator*(a); }

template<class T>
Polynomial<T> operator*(const T& a, const Polynomial<T>& b) { return b.operator*(a); }

template<class T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
  bool termPrinted = false;
  for (auto it = p.terms_.begin(); it != p.terms_.end(); ++it) {
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

namespace std {
  template<typename T>
  struct hash<Polynomial<T> > {
    size_t operator()(const Polynomial<T>& p) const {
      return hash<typename Polynomial<T>::MonomialType>()(p.lm());
    }
  };
}

#endif // POLYNOMIAL_H
