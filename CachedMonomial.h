#ifndef CACHED_MONOMIAL_H
#define CACHED_MONOMIAL_H

#include <deque>

#include "Monomial.h"

template<class M>
class CachedMonomial {
public:
  typedef typename M::ExponentType ExponentType;
  typedef CachedMonomial<M> This;
  static const uint VAR_COUNT = M::VAR_COUNT;

  CachedMonomial() : m_(addIfNecessary(M())) {}
  CachedMonomial(const M& m) : m_(addIfNecessary(m)) {}

  ExponentType operator[](uint index) const { return m()[index]; }

  uint degree() const { return m().degree(); }
  bool isConstant() const { return m().isConstant(); }

  This operator*(const This& other) const { return This(m() * other.m()); }
  This& operator*=(const This& other) { *this = *this * other; return *this; }

  This operator|(const This& other) const { return This(m() | other.m()); }
  This& operator|=(const This& other) { *this = *this | other; return *this; }

  This operator/(const This& other) const { return This(m() / other.m()); }
  This& operator/=(const This& other) { *this = *this / other; return *this; }

  bool divides(const This& other) const { return m().divides(other.m()); }

  bool operator<(const This& other) const { return m() < other.m(); }
  bool operator>(const This& other) const { return other < *this; }

  bool operator==(const This& other) const { return m_ == other.m_; }
  bool operator!=(const This& other) const { return !(operator==(other)); }
  bool isZero() const { return m().isZero(); }

  static This x(uint i) { return This(M::x(i)); }

  const M& m() const { return *m_; }
private:
  M* addIfNecessary(const M& m) {
    auto it = std::find(cache.begin(), cache.end(), m);
    if (it != cache.end()) {
      return &*it;
    } else {
      D("adding " << m << " to cache");
      cache.push_back(m);
      return &cache.back();
    }
  }

  M* m_;
public:
  static std::deque<M> cache;
};

template<class M>
std::deque<M> CachedMonomial<M>::cache = std::deque<M>();

namespace std {
  template<typename M>
  struct hash<CachedMonomial<M> > {
    size_t operator()(const CachedMonomial<M>& e) const {
      size_t result = 0;
      for (uint i = 0; i < M::VAR_COUNT; ++i) {
        result *= 2147483647;
        result += e[i];
      }
      return result;
    }
  };
}

template<class M>
CachedMonomial<M> pow(const CachedMonomial<M>& m, uint e) {
  return CachedMonomial<M>(pow(m.m(), e));
}

template<class M>
CachedMonomial<M> lcm(const CachedMonomial<M>& a, const CachedMonomial<M>& b) {
  return CachedMonomial<M>(lcm(a.m(), b.m()));
}

template<class M>
inline std::ostream& operator<<(std::ostream& out, const CachedMonomial<M>& cm) {
  return out << cm.m();
}

template<class M>
std::istream& operator>>(std::istream& in, CachedMonomial<M>& cm) {
  M m;
  in >> m;
  cm = CachedMonomial<M>(m);
  return in;
}

#endif // CACHED_MONOMIAL_H
