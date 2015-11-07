#ifndef MM_H
#define MM_H

#include <memory>
#include <future>
#include <boost/intrusive_ptr.hpp>

#include "style.h"
#include "MonRl.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct MM {
  typedef typename P::MonomialType M;
  typedef typename P::TermType T;
  typedef typename P::CoefficientType C;
  typedef MonRl<P> MonRlType;
  typedef MM<P> This;

  MM() : mmData(std::async(std::launch::deferred, [] { return boost::intrusive_ptr<MMData>(new MMData()); })) { mmData.get(); }
  MM(const MonRlType& v, const P& g) : mmData(std::async(std::launch::deferred, [&] { return boost::intrusive_ptr<MMData>(new MMData(v, g)); })) { mmData.get(); }

  const P& f() const { return mmData.get()->f_; }
  const MonRlType& u() const { return mmData.get()->u_; }

  bool operator<(const This& other) const {
    return u() < other.u();
  }
  bool operator>(const This& other) const { return other < *this; }

  bool operator==(const This& other) const {
    return u() == other.u() && f() == other.f();
  }

  This operator*(const M& e) const {
    return This(u() * e, f() * e);
  }

  void combineAndRenormalize(const C& afactor, This b, const C& bfactor) {
    auto a = mmData.get();
    mmData = std::shared_future<boost::intrusive_ptr<MMData> >(std::async(std::launch::async, [=] {
      return boost::intrusive_ptr<MMData>(new MMData(std::max(a->u_, b.u()), P::combineAndRenormalize(a->f_, afactor, b.f(), bfactor)));
    }));
  }

private:
  struct MMData {
    MMData() : u_(), f_(), refcount(0) {}
    MMData(const MonRlType& u, const P& f) : u_(u), f_(f), refcount(0) {}
    MonRlType u_;
    P f_;
    mutable uint refcount;
  };
  friend void intrusive_ptr_add_ref(const MM::MMData* p) {
    ++p->refcount;
  }
  friend void intrusive_ptr_release(const MM::MMData* p) {
    if (--p->refcount == 0) delete p;
  }
  std::shared_future<boost::intrusive_ptr<MMData> > mmData;
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
