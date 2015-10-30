#ifndef MM_H
#define MM_H

#include <memory>
#include <future>

#include "style.h"
#include "operators.h"
#include "MonRl.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct MM {
  typedef typename P::MonomialType M;
  typedef typename P::TermType T;
  typedef typename P::CoefficientType C;
  typedef MonRl<P> MonRlType;
  typedef MM<P> This;

  MM() : mmData(std::async([] { return std::make_shared<MMData>(); })) { mmData.get(); }
  MM(const MonRlType& v, const P& g) : mmData(std::async([&] { return std::make_shared<MMData>(v, g); })) { mmData.get(); }

  const P& f() const { return mmData.get()->f_; }
  const MonRlType& u() const { return mmData.get()->u_; }

  bool operator<(const This& other) const {
    return u() < other.u();
  }

  bool operator==(const This& other) const {
    return u() == other.u() && f() == other.f();
  }

  This operator*(const M& e) const {
    return This(u() * e, f() * e);
  }

  void combineAndRenormalize(const C& afactor, This b, const C& bfactor) {
    auto a = mmData.get();
    mmData = std::shared_future<std::shared_ptr<MMData> >(std::async(std::launch::async, [=] {
#ifdef DEBUG
      D("a.u < b.u is " << (a->u_ < b.u() ? "TRUE" : "FALSE") << ": a.u = " << a->u_ << ", b.u = " << b.u());
      D("a.u == b.u is " << (a->u_ == b.u() ? "TRUE" : "FALSE") << ": a.u = " << a->u_ << ", b.u = " << b.u());
      if (a->u_ == b.u()) {
        D("a.f = " << a->f_);
        D("b.f = " << b.f());
      }
#endif // DEBUG
      return std::make_shared<MMData>(std::max(a->u_, b.u()), P::combineAndRenormalize(a->f_, afactor, b.f(), bfactor));
    }));
  }

private:
  struct MMData {
    MMData() : u_(), f_() {}
    MMData(const MonRlType& u, const P& f) : u_(u), f_(f) {}
    MonRlType u_;
    P f_;
  };
  std::shared_future<std::shared_ptr<MMData> > mmData;
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
