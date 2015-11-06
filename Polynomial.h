#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ostream>
#include <sstream>
#include <set>
#include <list>
#include <forward_list>
#include <iterator>
#include <stdexcept>

#include "Term.h"
#include "integral.h"
#include "debug.h"

#define MAX_PRINT_LENGTH 50

template<class T = Term<int, Monomial<char> > >
class Polynomial {
  typedef typename T::CoefficientType C;
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

  class TermIterator : public std::iterator<std::forward_iterator_tag, const T> {
    typedef TermIterator This;
    TermIterator(const typename std::forward_list<T>::const_iterator& it_) : it(it_) {}
    friend This Polynomial<T>::begin() const;
    friend This Polynomial<T>::end() const;
  public:
    This& operator++(int) { ++it; return *this; }
    This operator++() { This r(*this); ++it; return r; }
    bool operator==(const This& other) const { return it == other.it; }
    bool operator!=(const This& other) const { return it != other.it; }
    typename std::iterator<std::forward_iterator_tag, const T>::reference operator*() const { return it.operator*(); }
    typename std::iterator<std::forward_iterator_tag, const T>::pointer operator->() const { return it.operator->(); }
  private:
    typename std::forward_list<T>::const_iterator it;
  };

  TermIterator begin() const { return TermIterator(terms.begin()); }
  TermIterator end() const { return TermIterator(terms.end()); }

  CoefficientType operator[](const MonomialType& m) const {
    auto c = terms.begin();
    while (c != terms.end() && c->m() > m) ++c;
    if (c == terms.end()) return CoefficientType();
    if (c->m() == m) return c->c();
    return CoefficientType();
  }
  bool isZero() const { return terms.empty(); }
  This& operator+=(const CoefficientType& c) { *this += T(c); return *this; }
  This operator+(const CoefficientType& c) const { This r = *this; r += c; return r; }
  This& operator-=(const CoefficientType& c) { C d = c; d *= -1; *this += d; return *this; }
  This operator-(const CoefficientType& c) const { This r = *this; r -= c; return r; }
  This& operator+=(const T& t) {
    if (t.isZero()) return *this;
    if (terms.empty() || lm() < t.m()) {
      terms.insert_after(terms.before_begin(), t);
      return *this;
    }

    auto current = terms.begin();
    auto before = terms.before_begin();

    while (current != terms.end() && current->m() > t.m()) {
      ++before; ++current;
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
  This& operator-=(const T& t) { *this += C(-1) * t; return *this; }
  This operator-(const T& t) const { This r = *this; r -= t; return r; }
  static This combine(const This& a, const C& afactor, const This& b, const C& bfactor) {
    This r;
    auto ait = a.terms.begin();
    auto aend = a.terms.end();
    auto bit = b.terms.begin();
    auto bend = b.terms.end();
    auto before = r.terms.before_begin();

    while (ait != aend && bit != bend) {
      auto am = ait->m();
      auto bm = bit->m();
      if (am > bm) {
        auto ac = ait->c();
        ac *= afactor;
        before = r.terms.insert_after(before, T(ac, am));
        ++ait;
      } else if (am < bm) {
        auto bc = bit->c();
        bc *= bfactor;
        before = r.terms.insert_after(before, T(bc, bm));
        ++bit;
      } else {
        C c = ait->c();
        c *= afactor;
        c += bfactor*bit->c();
        if (c != C()) {
          before = r.terms.insert_after(before, T(c, am));
        }
        ++ait;
        ++bit;
      }
    }
    while (ait != aend) {
      auto ac = ait->c();
      ac *= afactor;
      before = r.terms.insert_after(before, T(ac, ait->m()));
      ++ait;
    }
    while (bit != bend) {
      auto bc = bit->c();
      bc *= bfactor;
      before = r.terms.insert_after(before, T(bc, bit->m()));
      ++bit;
    }
    return r;
  }
  void renormalize() {
    auto it = terms.begin();
    auto end = terms.end();
    if (it == end) return;
    C d = it->c();
    if (d == C(1)) return;
    ++it;
    while (it != end) {
      C e = it->c();
      if (e < 0) e *= C(-1);
      d = gcd(d, e);
      if (d == C(1)) return;
      ++it;
    }
    it = terms.begin();
    while (it != end) {
      it->c() /= d;
      ++it;
    }
  }
  static This combineAndRenormalize(const This& a, const C& afactor, const This& b, const C& bfactor) {
    This r = combine(a, afactor, b, bfactor);
    r.renormalize();
    return r;
  }
  This& combine(const C& afactor, const This& b, const C& bfactor) {
    *this = combine(*this, afactor, b, bfactor);
    return *this;
  }
  This& operator+=(const This& other) { return combine(C(1), other, C(1)); }
  This operator+(const This& other) const { This r = *this; r += other; return r; }
  This& operator-=(const This& other) { return combine(C(1), other, C(-1)); }
  This operator-(const This& other) const { This r = *this; r -= other; return r; }

  This& operator*=(const C& c) {
    if (c == C()) {
      terms.clear();
      return *this;
    }
    for (auto it = terms.begin(); it != terms.end(); ++it) {
      *it *= c;
    }
    return *this;
  }
  This operator-() const { This r = *this; r *= C(-1); return r; }
  This operator*(const C& c) const { This r = *this; r *= c; return r; }
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
  bool operator<(const This& other) const { return lm() < other.lm(); }
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

template<class C, class M>
Polynomial<Term<C, M> > operator-(const Term<C, M>& a, const Term<C, M>& b) {
  Polynomial<Term<C, M> > r(a);
  r -= b;
  return r;
}

template<class T>
Polynomial<T> operator*(const typename T::CoefficientType& a, const Polynomial<T>& b) { return b.operator*(a); }

template<class T>
Polynomial<T> operator*(const T& a, const Polynomial<T>& b) { return b.operator*(a); }

template<class T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
  typedef typename T::CoefficientType C;
  std::stringstream ss;
  bool termPrinted = false;
  for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
    if (termPrinted && it->c() > C(0)) {
      ss << "+";
    }
    ss << *it;
    termPrinted = true;
    if (ss.str().length() > MAX_PRINT_LENGTH)
      break;
  }
  if (!termPrinted) {
    ss << "0";
  }
  if (ss.str().length() > MAX_PRINT_LENGTH) {
    uint termCount = 0;
    C max_abs_coefficient = C(0);
    auto it = p.terms.begin();
    auto end = p.terms.end();
    while (it != end) {
      ++termCount;
      C c = it->c();
      if (c < 0) c *= C(-1);
      max_abs_coefficient = std::max(max_abs_coefficient, c);
      ++it;
    }
    out << ss.str().substr(0,MAX_PRINT_LENGTH) << "...{" << termCount << " terms, |c| <= 2^" << log2(max_abs_coefficient) << "}";
  } else {
    out << ss.str();
  }
  return out;
}

template<class T>
std::istream& operator>>(std::istream& in, Polynomial<T>& p) {
  p = Polynomial<T>();
  auto next = in.peek();
  while (!in.eof() && (std::isalnum(next) || next == '+' || next == '-')) {
    T t;
    in >> t;
    if (!t.isZero()) p += t;
    next = in.peek();
  }
  D("read " << p);
  return in;
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
