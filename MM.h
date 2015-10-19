#ifndef MM_H
#define MM_H

#include "style.h"
#include "operators.h"
#include "MonRl.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct MM {
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef P PolynomialType;
  typedef MonRl<P> MonRlType;
  typedef MM<P> This;

  MM() : u(), f() {}
  MM(const MonRlType& v, const P& g) : u(v), f(g) {}

  bool operator<(const This& other) const {
    return u < other.u;
  }

  bool operator==(const This& other) const {
    return u == other.u && f == other.f;
  }

  This& operator*=(const MonomialType& e) {
    u *= e;
    f *= e;
    return *this;
  }

  This operator*(const MonomialType& e) const { This r(*this); r *= e; return r; }

  MonRlType u;
  P f;
};

template<class P>
MM<P> operator*(const typename P::MonomialType& e, const MM<P>& m) {
  return m * e;
}

template<class P>
std::ostream& operator<<(std::ostream& out, const MM<P>& uf) {
  return out << "(" << uf.u << ", " << uf.f << ")";
}

namespace std {
  template<class P>
  struct hash<MM<P> > { 
    size_t operator()(const MM<P>& mm) const {
      size_t result = 0;
      hash<typename P::MonomialType> mhash;
      result += mhash(mm.f.lm());
      return result;
    }
  };
}

#endif // MM_H
