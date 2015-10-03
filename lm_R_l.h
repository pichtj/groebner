#ifndef LM_R_L_H
#define LM_R_L_H

#include "style.h"
#include "Polynomial.h"

#define INPUT_COUNT 33

/* lead monomials of elements of R^l */
template<class C = int, class E = char>
class lm_R_l {
public:
  static lm_R_l<C, E> e(int i);
  Term<C, E> operator[](uint i) const { return lm_u[i]; }
  Term<C, E>& operator[](uint i) { return lm_u[i]; }
  Exponent<E> lm() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (lm_u[i].coefficient() != C()) {
        return lm_u[i].exponent();
      }
    }
    return Exponent<E>();
  }
  bool operator==(const lm_R_l<C, E>& other) const {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (lm_u[i] != other[i]) {
        return false;
      }
    }
    return true;
  }
  lm_R_l<C, E>& operator*=(const Exponent<E>& e) {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      lm_u[i] *= e;
    }
    return *this;
  }
private:
  Term<C, E> lm_u[INPUT_COUNT];
};

template<class C, class E>
lm_R_l<C, E> lm_R_l<C, E>::e(int i) {
  lm_R_l<C, E> result;
  result.lm_u[i] = 1;
  return result;
}

#endif // LM_R_L_H
