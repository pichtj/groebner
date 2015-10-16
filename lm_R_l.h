#ifndef LM_R_L_H
#define LM_R_L_H

#include <ostream>
#include <array>
#include <stdexcept>

#include "style.h"
#include "Polynomial.h"

#define INPUT_COUNT 33

/* lead monomials of elements of R^l */
template<class P = Polynomial<Term<int, Monomial<char> > > >
class lm_R_l {
public:
  typedef typename P::TermType::MonomialType MonomialType;
  typedef typename P::TermType::CoefficientType CoefficientType;
  typedef typename P::TermType TermType;
  typedef lm_R_l<P> This;

  lm_R_l() : m_(), index_(INPUT_COUNT) {}

  MonomialType m() const { return m_; }
  uint index() const { return index_; }

  static This e(int i) {
    This result;
    result.index_ = i;
    return result;
  }
  MonomialType operator[](uint i) const { if (i == index_) return m_; else return MonomialType(); }
  This lm() const { return *this; }
  bool operator==(const This& other) const {
    return index_ == other.index_ && m_ == other.m_;
  }

  bool operator<(const This& other) const {
    uint i = index_;
    uint j = other.index_;
    if (i > j) return true;
    if (i == j && m_ < other.m_) return true;
    return false;
  }

  bool divides(const This& other) const {
    if (index_ != other.index_) return false;
    return m_.divides(other.m_);
  }

  MonomialType operator/(const This& other) const {
    if (!other.divides(*this)) throw std::domain_error("does not divide");
    return m_ / other.m_;
  }

  This& operator+=(const This& b) {
    if (index_ < b.index_) return *this;
    if (index_ > b.index_) {
      index_ = b.index_;
      m_ = b.m_;
      return *this;
    }
    m_ = std::max(m_, b.m_);
  }
  This operator+(const This& b) const { This r(*this); r += b; return r; }

  This& operator-=(const This& b) { return operator+=(b); }
  This operator-(const This& b) const { This r(*this); r -= b; return r; }

  This& operator*=(const TermType& b) { m_ *= b.m(); return *this; }
  This operator*(const TermType& b) const { This r(*this); r *= b; return r; }

  This& operator*=(const MonomialType& b) { m_ *= b; return *this; }
  This operator*(const MonomialType& b) const { This r(*this); r *= b; return r; }

  template<class P1>
  friend std::ostream& operator<<(std::ostream&, const lm_R_l<P1>&);
private:
  MonomialType m_;
  uint index_;
};

template<class P>
lm_R_l<P> operator*(const typename P::MonomialType& a, const lm_R_l<P>& b) {
  return b * a;
}

template<class P>
lm_R_l<P> operator*(const typename P::TermType& a, const lm_R_l<P>& b) {
  return b * a;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const lm_R_l<P>& u) {
  return out << u.m_ << "*e_" << u.index_;
}

#endif // LM_R_L_H
