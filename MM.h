#ifndef MM_H
#define MM_H

#include "style.h"
#include "operators.h"
#include "lm_R_l.h"
#include "Polynomial.h"

template<class C = int, class E = char>
class MM {
public:
  MM() : u(), f() {}
  MM(const lm_R_l<C, E>& v, const Polynomial<C, E>& g) : u(v), f(g) {}
  lm_R_l<C, E> u;
  Polynomial<C, E> f;

  bool operator<(const MM<C, E>& other) const {
    for (uint i = 0; i < INPUT_COUNT; ++i) {
      if (u[i] != Term<C, E>()) {
        if (other.u[i] != Term<C, E>()) {
          return u[i].exponent() < other.u[i].exponent();
        } else {
          return false;
        }
      } else {
        if (other.u[i] != Term<C, E>()) {
          return true;
        }
      }
    }
    return false;
  }

  MM<C, E> jPair(const MM<C, E>& other);

  MM<C, E>& operator*=(const Exponent<E>& e) {
    u *= e;
    f *= e;
    return *this;
  }

  MM<C, E> operator*(const Exponent<E>& e) const {
    MM<C, E> result(*this);
    result *= e;
    return result;
  }
};

template<class C, class E>
MM<C, E> operator*(const Exponent<E>& e, const MM<C, E>& m) {
  return m * e;
}

#endif // MM_H
