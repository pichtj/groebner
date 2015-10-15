#ifndef MM_H
#define MM_H

#include "style.h"
#include "operators.h"
#include "lm_R_l.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
class MM {
public:
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef P PolynomialType;
  typedef lm_R_l<P> lm_R_lType;
  typedef MM<P> This;

  MM() : u_(), f_() {}
  MM(const lm_R_lType& v, const P& g) : u_(v), f_(g) {}
  lm_R_lType u() const { return u_; }
  P f() const { return f_; }

  bool operator<(const This& other) const {
    return u_ < other.u_;
  }

  bool operator==(const This& other) const {
    return u_ == other.u_ && f_ == other.f_;
  }

  This& operator*=(const MonomialType& e) {
    u_ *= e;
    f_ *= e;
    return *this;
  }

  This operator*(const MonomialType& e) const { This r(*this); r *= e; return r; }

private:
  lm_R_lType u_;
  P f_;
};

template<class P>
MM<P> operator*(const typename P::MonomialType& e, const MM<P>& m) {
  return m * e;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const MM<P>& uf) {
  return out << "(" << uf.u() << ", " << uf.f() << ")";
}

namespace std {
  template<class P>
  struct hash<MM<P> > { 
    size_t operator()(const MM<P>& mm) const {
      size_t result = 0;
      hash<typename P::MonomialType> mhash;
      result += mhash(mm.f().lm());
      return result;
    }
  };
}

#endif // MM_H
