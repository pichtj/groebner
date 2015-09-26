#include "Polynomial.h"

#include <set>
using namespace std;

#define INPUT_COUNT 33

template<class E>
class lm_R_l {
public:
  Exponent<E> lm_u[INPUT_COUNT];
  Exponent<E> lm() const;
};

template<class C, class E>
class M {
public:
  lm_R_l<E> u;
  Polynomial<C, E> f;

  M<C, E> jPair(const M<C, E>& other);
};

template<class C, class E>
class LabelledMonomial {
public:
  Exponent<E> m;
  M<C, E> generator;
  inline Exponent<E> monomial() const { return m; }
  inline unsigned int degree() const { return m.degree(); }
  inline Exponent<E> signature() {
    Exponent<E> t = m / generator.f.lm();
    return t * generator.u.lm();
  }

  inline bool collidesWith(const LabelledMonomial<C, E>& other) {
    if (m != other.m) {
      return false;
    }
    if (m.isZero()) {
      return false;
    }
    return true;
  }

  template<class LMSet>
  bool reducible(const LMSet& B) const;

  template<class LMSet>
  void reduce(const LMSet& B);

  template<class LMSet>
  void mutual_reduce(LMSet& b);
};

template<class C, class E>
LabelledMonomial<C, E> operator*(Exponent<E> exp, LabelledMonomial<C, E> lmon);

template<class C, class E, class LMSet>
set<M<C, E> > lift(const LMSet& todo, const LMSet& G);

template<class C, class E, class MSet, class LMSet>
void append(MSet& H, const LMSet& G);

template<class C, class E, class MSet>
MSet eliminate(const MSet& H);

template<class C, class E, class MSet, class LMSet>
void update(MSet& P, const LMSet& G);

template<class C, class E, class PSet>
PSet moGVW(const PSet& input);
