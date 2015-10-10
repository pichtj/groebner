#ifndef LM_R_L_H
#define LM_R_L_H

#include <ostream>
#include <array>

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
  MonomialType lm() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (lm_u[i].c() != CoefficientType()) {
        return lm_u[i].m();
      }
    }
    return MonomialType();
  }
  bool operator==(const This& other) const {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (lm_u[i] != other[i]) {
        return false;
      }
    }
    return true;
  }

  This& operator+=(const This& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!b[i].isZero()) {
        if (!lm_u[i].isZero()) throw "lm_R_l: cannot add values with entries at same index";
        lm_u[i] = b[i];
      }
    }
    return *this;
  }
  This operator+(const This& b) const { This r(*this); r += b; return r; }

  This& operator-=(const This& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (!b[i].isZero()) {
        if (!lm_u[i].isZero()) throw "lm_R_l: cannot add values with entries at same index";
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
