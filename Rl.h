#ifndef RL_H
#define RL_H

#include <ostream>
#include <array>
#include <stdexcept>
#include <limits>

#include "style.h"
#include "Polynomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
class Rl {
  typedef typename P::TermType::MonomialType M;
  typedef typename P::TermType::CoefficientType C;
  typedef typename P::TermType T;
public:
  typedef typename P::TermType::MonomialType MonomialType;
  typedef typename P::TermType::CoefficientType CoefficientType;
  typedef typename P::TermType TermType;
  typedef Rl<P> This;

  Rl() : m() {}
  Rl(const P& p, uint i) : m(i, P()) { m[i] = p; }

  static This e(uint i) {
    return Rl(P(1), i);
  }
  bool operator==(const This& other) const {
    return m == other.m;
  }

  uint index() const {
    uint r = 0;
    while (m.size() > r && m[r].isZero()) ++r;
    if (r == m.size()) return -1;
    return r;
  }

  bool operator<(const This& other) const {
    uint i = index();
    uint j = other.index();
    if (i > j) return true;
    if (i == j) return m[i] < other.m[j];
    return false;
  }
  bool operator>(const This& other) const { return other < *this; }

  template<class P1>
  friend std::ostream& operator<<(std::ostream&, const Rl<P1>&);

private:
  std::vector<P> m;
};

template<class P>
std::ostream& operator<<(std::ostream& out, const Rl<P>& u) {
  out << "[";
  bool first = true;
  for (auto p : u.m) {
    if (!first) out << ", ";
    out << p;
    first = false;
  }
  return out << "]";
}

#endif // RL_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
