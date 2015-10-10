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
class ULMSet {
public:
  typedef typename P::MonomialType MonomialType;
  typedef MM<P> MMType;
  typedef lm_R_l<P> lm_R_lType;

  unordered_map<MonomialType, MMType> byMonomial;
};

template<class P = Polynomial<Term<int, Monomial<char> > > >
class LMSet {
public:
  typedef typename P::MonomialType MonomialType;
  typedef MM<P> MMType;
  typedef lm_R_l<P> lm_R_lType;

  unordered_multimap<MonomialType, MMType> byMonomial;
};

template<class P = Polynomial<Term<int, Monomial<char> > > >
class LabelledMonomial {
public:
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef lm_R_l<P> lm_R_lType;
  typedef MM<P> MMType;
  typedef LMSet<P> LMSetType;
  typedef ULMSet<P> ULMSetType;
  typedef LabelledMonomial<P> This;
  LabelledMonomial(const MonomialType& e, const MMType& g) : m_(e), u_(g.u()), f_(g.f()), wasLifted_(false) {}
  LabelledMonomial(const MonomialType& e, const lm_R_lType& v, const P& g) : m_(e), u_(v), f_(g), wasLifted_(false) {}
  MonomialType m() const { return this->m_; }
  lm_R_lType u() const { return this->u_; }
  P f() const { return this->f_; }
  bool wasLifted() const { return this->wasLifted_; }
  uint degree() const { return m_.degree(); }
  MMType signature() {
    MonomialType t = m_ / f_.lm();
    return MMType(u_, TermType(1, t));
  }
  bool isPrimitive() const { return m_ == f_.lm(); }

  bool collidesWith(const This& other) { return m_ == other.m_ && !m_.isZero(); }

  bool operator==(const This& other) const {
    return m_ == other.m_ && u_ == other.u_ && f_ == other.f_;
  }

  This operator*(const MonomialType& e) const {
    This result(*this);
    result.m_ *= e;
    return result;
  }
private:
  MonomialType m_;
  lm_R_lType u_;
  P f_;
  bool wasLifted_;
};

namespace std {
  template<class P>
  struct hash<LabelledMonomial<P> > {
    size_t operator()(const LabelledMonomial<P>& lm) const {
      hash<typename LabelledMonomial<P>::MonomialType > h;
      return h(lm.m());
    }
  };
}

template<class P>
LabelledMonomial<P> operator*(const typename P::MonomialType& exp, const LabelledMonomial<P>& lmon) {
  return lmon * exp;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const LabelledMonomial<P>& muf) {
  return out << "(" << muf.m() << ", (" << muf.u() << ", " << muf.f() << "))";
}

#endif // LABELLED_MONOMIAL_H
