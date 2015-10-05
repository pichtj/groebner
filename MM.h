#ifndef MM_H
#define MM_H

#include "style.h"
#include "operators.h"
#include "lm_R_l.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
class MM {
public:
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef P PolynomialType;
  typedef lm_R_l<P> lm_R_lType;
  typedef MM<P> This;

  MM() : u(), f() {}
  MM(const lm_R_lType& v, const P& g) : u(v), f(g) {}
  lm_R_lType u;
  P f;

  bool operator<(const This& other) const {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (u[i] != TermType()) {
        if (other.u[i] != TermType()) {
          return u[i].exponent() < other.u[i].exponent();
        } else {
          return false;
        }
      } else {
        if (other.u[i] != TermType()) {
          return true;
        }
      }
    }
    return false;
  }

  This jPair(const This& other);

  This& operator*=(const MonomialType& e) {
    u *= e;
    f *= e;
    return *this;
  }

  This operator*(const MonomialType& e) const {
    This result(*this);
    result *= e;
    return result;
  }
};

template<class P>
MM<P> operator*(const typename P::MonomialType& e, const MM<P>& m) {
  return m * e;
}

#endif // MM_H
