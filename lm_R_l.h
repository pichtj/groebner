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

  static This e(int i) {
    This result;
    result.lm_u[i] = 1;
    return result;
  }
  TermType operator[](uint i) const { return lm_u[i]; }
  TermType& operator[](uint i) { return lm_u[i]; }
  This lm() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (lm_u[i].c() != CoefficientType()) {
        return e(i) * lm_u[i].m();
      }
    }
    throw std::domain_error("lead monomial of u = 0 is not a monomial");
  }
  bool operator==(const This& other) const {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (lm_u[i] != other[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator<(const This& other) const {
    uint i = 0;
    uint j = 0;
    while (i < INPUT_COUNT && lm_u[i].isZero()) ++i;
    while (j < INPUT_COUNT && other.lm_u[j].isZero()) ++j;
    if (i > j) return true;
    if (i == j && lm_u[i].m() < other.lm_u[j].m()) return true;
    return false;
  }

  bool divides(const This& other) const {
    bool nonZeroFound = false;
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!lm_u[i].isZero() || !other.lm_u[i].isZero()) {
        if (nonZeroFound) return false;
        nonZeroFound = true;
      }
      if (!lm_u[i].m().divides(other.lm_u[i].m())) return false;
    }
    return true;
  }

  MonomialType operator/(const This& other) const {
    if (!divides(other)) throw std::domain_error("does not divide");
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!lm_u[i].isZero() || !other.lm_u[i].isZero())
        return lm_u[i].m() / other.lm_u[i].m();
    }
    return MonomialType();
  }

  This& operator+=(const This& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!b[i].isZero()) {
        if (!lm_u[i].isZero()) throw std::domain_error("lm_R_l: cannot add values with entries at same index");
        lm_u[i] = b[i];
      }
    }
    return *this;
  }
  This operator+(const This& b) const { This r(*this); r += b; return r; }

  This& operator-=(const This& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!b[i].isZero()) {
        if (!lm_u[i].isZero()) throw std::domain_error("lm_R_l: cannot add values with entries at same index");
        lm_u[i] = -b[i];
      }
    }
    return *this;
  }
  This operator-(const This& b) const { This r(*this); r -= b; return r; }

  This& operator*=(const TermType& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      lm_u[i] *= b;
    }
    return *this;
  }
  This operator*(const TermType& b) const { This r(*this); r *= b; return r; }

  This& operator*=(const MonomialType& m) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      lm_u[i] *= m;
    }
    return *this;
  }
  This operator*(const MonomialType& b) const { This r(*this); r *= b; return r; }

private:
  std::array<TermType, INPUT_COUNT> lm_u;
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
  typedef typename P::TermType T;
  typedef typename T::CoefficientType C;
  bool termPrinted = false;
  for (uint i = 0; i < INPUT_COUNT; ++i) {
    if (!u[i].isZero()) {
      if (termPrinted && u[i].c() > 0) out << "+";
      if (!u[i].isOne()) out << u[i] << "*";
      out << "e_" << i;
      termPrinted = true;
    }
  }
  if (!termPrinted) out << "0";
  return out;
}

#endif // LM_R_L_H
