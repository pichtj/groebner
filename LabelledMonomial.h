#ifndef LABELLED_MONOMIAL_H
#define LABELLED_MONOMIAL_H

#include "style.h"
#include "Monomial.h"
#include "Polynomial.h"
#include "MM.h"
#include "lm_R_l.h"

#include <set>
#include <unordered_set>
#include <unordered_map>
#include <limits>
using namespace std;

#define INPUT_COUNT 33

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct LabelledMonomial {
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef lm_R_l<P> lm_R_lType;
  typedef MM<P> MMType;
  typedef LabelledMonomial<P> This;
  LabelledMonomial(const MonomialType& e, const MMType& g) : m(e), u(g.u), f(g.f) {}
  LabelledMonomial(const MonomialType& e, const lm_R_lType& v, const P& g) : m(e), u(v), f(g) {}
  uint degree() const { return m.degree(); }
  lm_R_lType signature() const {
    MonomialType t = m / f.lm();
    return u * t;
  }
  bool isPrimitive() const { return m == f.lm(); }

  bool collidesWith(const This& other) { return m == other.m && !m.isZero(); }

  bool operator==(const This& other) const {
    return m == other.m && u == other.u && f == other.f;
  }

  This operator*(const MonomialType& e) const {
    This result(*this);
    result.m *= e;
    return result;
  }
  MonomialType m;
  lm_R_lType u;
  P f;
};

namespace std {
  template<class P>
  struct hash<LabelledMonomial<P> > {
    size_t operator()(const LabelledMonomial<P>& lm) const {
      hash<typename LabelledMonomial<P>::MonomialType> h;
      return h(lm.m);
    }
  };
}

template<class P>
LabelledMonomial<P> operator*(const typename P::MonomialType& exp, const LabelledMonomial<P>& lmon) {
  return lmon * exp;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const LabelledMonomial<P>& muf) {
  return out << "(" << muf.m << ", (" << muf.u << ", " << muf.f << "))";
}

#endif // LABELLED_MONOMIAL_H
