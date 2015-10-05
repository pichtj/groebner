#ifndef LM_R_L_H
#define LM_R_L_H

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
      TermType zero;
      if (b[i] != zero) {
        if (lm_u[i] != zero) throw "lm_R_l: cannot add values with entries at same index";
        lm_u[i] = b[i];
      }
    }
    return *this;
  }
  This operator+(const This& b) const { This r; r += b; return *this; }

  This& operator-=(const This& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      TermType zero;
      if (b[i] != zero) {
        if (lm_u[i] != zero) throw "lm_R_l: cannot add values with entries at same index";
        lm_u[i] = -b[i];
      }
    }
    return *this;
  }
  This operator-(const This& b) const { This r; r -= b; return *this; }

  This& operator*=(const TermType& b) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      lm_u[i] *= b;
    }
    return *this;
  }

  This operator*(const TermType& b) const { This r; r *= b; return *this; }

private:
  TermType lm_u[INPUT_COUNT];
};

template<class P>
lm_R_l<P> operator*(const typename P::TermType& a, const lm_R_l<P>& b) {
  return b * a;
}

#endif // LM_R_L_H
