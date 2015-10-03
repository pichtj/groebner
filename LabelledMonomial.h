#include "style.h"
#include "Exponent.h"
#include "Polynomial.h"
#include "MM.h"
#include "lm_R_l.h"

#include <set>
#include <unordered_set>
#include <limits>
using namespace std;

#define INPUT_COUNT 33

template<class C = int, class E = char>
class LabelledMonomial {
public:
  LabelledMonomial(const Exponent<E>& e, const MM<C, E>& g) : m(e), u(g.u), f(g.f), wasLifted(false) {}
  LabelledMonomial(const Exponent<E>& e, const lm_R_l<C, E>& v, const Polynomial<C, E>& g) : m(e), u(v), f(g), wasLifted(false) {}
  Exponent<E> m;
  lm_R_l<C, E> u;
  Polynomial<C, E> f;
  bool wasLifted;
  uint degree() const { return m.degree(); }
  MM<C, E> signature() {
    Exponent<E> t = m / f.lm();
    return MM<C, E>(u, Term<C, E>(t));
  }
  bool isPrimitive() const {
    return m == f.lm();
  }

  bool collidesWith(const LabelledMonomial<C, E>& other) {
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
  void mutualReduce(LMSet& b);
};

namespace std {
  template<class C, class E>
  struct hash<LabelledMonomial<C, E> > {
    size_t operator()(const LabelledMonomial<C, E>& lm) const {
      hash<Exponent<E> > h;
      return h(lm.m);
    }
  };
}

template<class C, class E>
LabelledMonomial<C, E> operator*(const Exponent<E>& exp, const LabelledMonomial<C, E>& lmon);
