#ifndef MM_H
#define MM_H

#include "style.h"
#include "operators.h"
#include "lm_R_l.h"
#include "Polynomial.h"

template<class C = int, class E = char>
class MM {
public:
  typedef Monomial<E> MonomialType;
  typedef Term<C, E> TermType;
  typedef lm_R_l<C, E> lm_R_lType;
  typedef Polynomial<C, E> PolynomialType;
  typedef MM<C, E> This;

  MM() : u(), f() {}
  MM(const lm_R_lType& v, const PolynomialType& g) : u(v), f(g) {}
  lm_R_lType u;
  PolynomialType f;

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

template<class C, class E>
MM<C, E> operator*(const Monomial<E>& e, const MM<C, E>& m) {
  return m * e;
}

#endif // MM_H
