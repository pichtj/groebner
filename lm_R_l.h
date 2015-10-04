#ifndef LM_R_L_H
#define LM_R_L_H

#include "style.h"
#include "Polynomial.h"

#define INPUT_COUNT 33

/* lead monomials of elements of R^l */
template<class C = int, class E = char>
class lm_R_l {
public:
  typedef Monomial<E> MonomialType;
  typedef Term<C, E> TermType;
  typedef lm_R_l<C, E> This;

  static This e(int i) {
    This result;
    result.lm_u[i] = 1;
    return result;
  }
  TermType operator[](uint i) const { return lm_u[i]; }
  TermType& operator[](uint i) { return lm_u[i]; }
  MonomialType lm() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (lm_u[i].coefficient() != C()) {
        return lm_u[i].exponent();
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
  This& operator*=(const MonomialType& e) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      lm_u[i] *= e;
    }
    return *this;
  }
private:
  TermType lm_u[INPUT_COUNT];
};

#endif // LM_R_L_H
