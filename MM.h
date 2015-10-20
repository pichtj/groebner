#ifndef MM_H
#define MM_H

#include <memory>

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

  MM() : mmData(std::make_shared<MMData>()) {}
  MM(const MonRlType& v, const P& g) : mmData(std::make_shared<MMData>(v, g)) {}

  const P& f() const { return mmData->f_; }
  const MonRlType& u() const { return mmData->u_; }

  bool operator<(const This& other) const {
    return u() < other.u();
  }

  bool operator==(const This& other) const {
    return u() == other.u() && f() == other.f();
  }

  This operator*(const MonomialType& e) const {
    return This(u() * e, f() * e);
  }

private:
  struct MMData {
    MMData() : u_(), f_() {}
    MMData(const MonRlType& u, const P& f) : u_(u), f_(f) {}
    MonRlType u_;
    P f_;
  };
  std::shared_ptr<MMData> mmData;
};

template<class P>
MM<P> operator*(const typename P::MonomialType& e, MM<P> m) {
  return m * e;
}

template<class P>
std::ostream& operator<<(std::ostream& out, MM<P> uf) {
  return out << "(" << uf.u() << ", " << uf.f() << ")";
}

namespace std {
  template<class P>
  struct hash<MM<P> > { 
    size_t operator()(MM<P> mm) const {
      size_t result = 0;
      hash<typename P::MonomialType> mhash;
      result += mhash(mm.f().lm());
      return result;
    }
  };
}

#endif // MM_H
