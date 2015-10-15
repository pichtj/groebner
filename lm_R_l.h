#ifndef LM_R_L_H
#define LM_R_L_H

#include <ostream>
#include <array>
#include <stdexcept>

#include "style.h"
#include "Polynomial.h"

#define INPUT_COUNT 33

/* lead monomials of elements of R^l */
template<class P = Polynomial<Term<int, Monomial<char> > > >
class lm_R_l {
public:
  typedef typename P::TermType::MonomialType MonomialType;
  typedef typename P::TermType::CoefficientType CoefficientType;
  typedef typename P::TermType TermType;
  typedef lm_R_l<P> This;

  lm_R_l() : m(), index(INPUT_COUNT) {}

  static This e(int i) {
    This result;
    result.index = i;
    return result;
  }
  MonomialType operator[](uint i) const { if (i == index) return m; else return MonomialType(); }
  This lm() const { return *this; }
  bool operator==(const This& other) const {
    return index == other.index && m == other.m;
  }

  bool operator<(const This& other) const {
    uint i = index;
    uint j = other.index;
    if (i > j) return true;
    if (i == j && m < other.m) return true;
    return false;
  }

  bool divides(const This& other) const {
    if (index != other.index) return false;
    return m.divides(other.m);
  }

  MonomialType operator/(const This& other) const {
    if (!other.divides(*this)) throw std::domain_error("does not divide");
    return m / other.m;
  }

  This& operator+=(const This& b) {
    if (index < b.index) return *this;
    if (index > b.index) {
      index = b.index;
      m = b.m;
      return *this;
    }
    m = std::max(m, b.m);
  }
  This operator+(const This& b) const { This r(*this); r += b; return r; }

  This& operator-=(const This& b) { return operator+=(b); }
  This operator-(const This& b) const { This r(*this); r -= b; return r; }

  This& operator*=(const TermType& b) { m *= b.m(); return *this; }
  This operator*(const TermType& b) const { This r(*this); r *= b; return r; }

  This& operator*=(const MonomialType& b) { m *= b; return *this; }
  This operator*(const MonomialType& b) const { This r(*this); r *= b; return r; }

  template<class P1>
  friend std::ostream& operator<<(std::ostream&, const lm_R_l<P1>&);
private:
  MonomialType m;
  uint index;
};

template<class P>
lm_R_l<P> operator*(const typename P::MonomialType& a, const lm_R_l<P>& b) {
  return b * a;
}

template<class P>
lm_R_l<P> operator*(const typename P::TermType& a, const lm_R_l<P>& b) {
  return b * a;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const lm_R_l<P>& u) {
  return out << u.m << "*e_" << u.index;
}

#endif // LM_R_L_H
