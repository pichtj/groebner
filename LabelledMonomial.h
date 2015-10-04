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

template<class C = int, class E = char>
class ULMSet {
public:
  typedef Monomial<E> MonomialType;
  typedef MM<C, E> MMType;
  typedef lm_R_l<C, E> lm_R_lType;

  unordered_map<MonomialType, MMType> byMonomial;
};

template<class C = int, class E = char>
class LMSet {
public:
  typedef Monomial<E> MonomialType;
  typedef MM<C, E> MMType;
  typedef lm_R_l<C, E> lm_R_lType;

  unordered_multimap<MonomialType, MMType> byMonomial;
};

template<class C = int, class E = char>
class LabelledMonomial {
public:
  typedef Monomial<E> MonomialType;
  typedef Polynomial<C, E> PolynomialType;
  typedef MM<C, E> MMType;
  typedef lm_R_l<C, E> lm_R_lType;
  typedef LMSet<C, E> LMSetType;
  typedef ULMSet<C, E> ULMSetType;
  typedef LabelledMonomial<C, E> This;
  LabelledMonomial(const MonomialType& e, const MM<C, E>& g) : m_(e), u_(g.u), f_(g.f), wasLifted_(false) {}
  LabelledMonomial(const MonomialType& e, const lm_R_l<C, E>& v, const Polynomial<C, E>& g) : m_(e), u_(v), f_(g), wasLifted_(false) {}
  MonomialType m() const { return this->m_; }
  lm_R_l<C, E> u() const { return this->u_; }
  Polynomial<C, E> f() const { return this->f_; }
  bool wasLifted() const { return this->wasLifted_; }
  uint degree() const { return m_.degree(); }
  MM<C, E> signature() {
    MonomialType t = m_ / f_.lm();
    return MM<C, E>(u_, Term<C, E>(t));
  }
  bool isPrimitive() const { return m_ == f_.lm(); }

  bool collidesWith(const LabelledMonomial<C, E>& other) { return m_ == other.m_ && !m_.isZero(); }

  bool reducible(const LMSetType& BB) const;

  void reduce(const LMSetType& BB);

  bool isCoveredBy(const LMSetType& BB) const;

  void mutualReduce(LMSetType& BB);

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
  PolynomialType f_;
  bool wasLifted_;
};

namespace std {
  template<class C, class E>
  struct hash<LabelledMonomial<C, E> > {
    size_t operator()(const LabelledMonomial<C, E>& lm) const {
      hash<Monomial<E> > h;
      return h(lm.m());
    }
  };
}

template<class C, class E>
LabelledMonomial<C, E> operator*(const Monomial<E>& exp, const LabelledMonomial<C, E>& lmon) {
  return lmon * exp;
}

#endif // LABELLED_MONOMIAL_H
