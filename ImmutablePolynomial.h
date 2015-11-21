#ifndef IMMUTABLE_POLYNOMIAL_H
#define IMMUTABLE_POLYNOMIAL_H

#include "Polynomial.h"

template<class P>
class ImmutablePolynomial {
  typedef typename P::CoefficientType C;
  typedef typename P::MonomialType M;
  typedef typename P::TermType T;
public:
  typedef typename P::CoefficientType CoefficientType;
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef ImmutablePolynomial<P> This;

  ImmutablePolynomial() : p() {}
  ImmutablePolynomial(const C& c) : p(c) {}
  ImmutablePolynomial(const T& t) : p(t) {}
  ImmutablePolynomial(const P& p_) : p(p_) {}
  T lterm() const { return p.lterm(); }
  C lc() const { return p.lc(); }
  M lm() const { return p.lm(); }

  class TermIterator : public std::iterator<std::forward_iterator_tag, const T> {
    typedef TermIterator This;
    TermIterator(const typename P::TermIterator& it_) : it(it_) {}
    friend This ImmutablePolynomial<P>::begin() const;
    friend This ImmutablePolynomial<P>::end() const;
  public:
    This& operator++(int) { ++it; return *this; }
    This operator++() { This r(*this); ++it; return r; }
    bool operator==(const This& other) const { return it == other.it; }
    bool operator!=(const This& other) const { return it != other.it; }
    typename std::iterator<std::forward_iterator_tag, const T>::reference operator*() const { return it.operator*(); }
    typename std::iterator<std::forward_iterator_tag, const T>::pointer operator->() const { return it.operator->(); }
  private:
    typename P::TermIterator it;
  };

  TermIterator begin() const { return TermIterator(p.begin()); }
  TermIterator end() const { return TermIterator(p.end()); }

  C operator[](const M& m) const { return p[m]; }
  bool isZero() const { return p.isZero(); }
  This operator+(const C& c) const { return This(p + c); }
  This operator-(const C& c) const { return This(p - c); }
  This operator+(const T& t) const { return This(p + t); }
  This operator-(const T& t) const { return This(p - t); }
  static This combine(const This& a, const C& afactor, const This& b, const C& bfactor) {
    return This(P::combine(a.p, afactor, b.p, bfactor));
  }
  static This combineAndRenormalize(const This& a, const C& afactor, const This& b, const C& bfactor) {
    return This(P::combineAndRenormalize(a.p, afactor, b.p, bfactor));
  }
  This operator+(const This& other) const { return This(p + other); }
  This operator-(const This& other) const { return This(p - other); }

  This operator-() const { return This(-p); }
  This operator*(const C& c) const { return This(p * c); }
  This operator*(const M& m) const { return This(p * m); }
  This operator*(const T& t) const { return This(p * t); }
  This operator*(const P& q) const { return This(p * q); }
  This operator*(const This& other) const { return operator*(other.p); }
  bool operator==(const This& other) const { return p == other.p; }
  bool operator<(const This& other) const { return lm() < other.lm(); }
  template<class T1>
  friend std::ostream& operator<<(std::ostream& out, const ImmutablePolynomial<T1>& p);
private:
  P p;
};

template<class P>
std::ostream& operator<<(std::ostream& out, const ImmutablePolynomial<P>& p) {
  return out << p.p;
}

namespace std {
  template<typename P>
  struct hash<ImmutablePolynomial<P> > {
    size_t operator()(const ImmutablePolynomial<P>& ip) const {
      return hash<P>()(ip.p);
    }
  };
}

#endif // IMMUTABLE_POLYNOMIAL_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
