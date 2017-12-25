#ifndef F5_H
#define F5_H

#include <vector>
#include <boost/optional.hpp>

#include "debug.h"
#include "GbRunner.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct F5Runner : public GbRunner {
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  typedef typename P::CoefficientType C;
  typedef Signature<P> S;
  typedef std::pair<S, P> R;

  static S signature(const R& r) { return r.first; }
  static P poly(const R& r) { return r.second; }
  static uint index(const R& r) { return signature(r).index; }

  S signature(uint i) { return signature(L[i]); }
  P poly(uint i) { return poly(L[i]); }
  uint index(uint i) { return index(L[i]); }

  struct CriticalPair {
    CriticalPair(const M& t_, const M& u1_, uint r1_, const M& u2_, uint r2_) : t(t_), u1(u1_), r1(r1_), u2(u2_), r2(r2_) {}
    bool operator<(const CriticalPair& other) const { return t < other.t; }

    friend inline std::ostream& operator<<(std::ostream& out, const typename F5Runner<P>::CriticalPair& cp) {
      return out << "(" << cp.t << ", " << cp.u1 << ", " << cp.r1 << ", " << cp.u2 << ", " << cp.r2 << ")";
    }

    M t;
    M u1;
    uint r1;
    M u2;
    uint r2;
  };

  P normalForm(P p, uint k) {
    DD("p = " << p << ", G[" << k << "] = ", G[k]);
    DD("L = ", L);

    auto t = p.begin();
    while (t != p.end()) {
      uint i = 0;
      while (i < G[k].size()) {
        const auto& q = poly(G[k][i]);
        if (q.lm().divides(t->m())) {
          D("reducing " << p << " with " << q);
          auto level = t->m();
          D("level = " << level);
          auto tq = t->m() / q.lm();
          D("tq = " << tq);
          auto r = q * tq;
          D("r = " << r);
          auto m_tc = t->c();
          m_tc *= C(-1);
          p = P::combineAndRenormalize(p, q.lc(), r, m_tc);
          D("reduced to = " << p);
          t = p.begin();
          while (t != p.end() && t->m() > level) ++t;
          break;
        } else {
          ++i;
        }
      }
      if (i == G[k].size()) {
        D("could not reduce " << t->m());
        ++t;
      }
    }
    D("returning p = " << p);
    return p;
  }

  void AddRule(uint j) {
    auto i = index(j);
    D("r_j = " << L[j] << " with index " << i << " and signature " << signature(j));
    Rules[i].insert(Rules[i].begin(), std::make_pair(signature(j).m, j));
    DD("Rules[" << i << "] = ", Rules[i]);
  }

  uint rewrite(const M& u, uint k) {
    D("u = " << u << ", r_k = " << L[k]);
    auto v = signature(k).m;
    auto l = index(k);
    auto Rules_l = Rules[l];
    auto uv = u * v;
    DD("v = " << v << ", l = " << l << ", uv = " << uv << ", Rules_l = ", Rules_l);
    for (uint j = 0; j < Rules_l.size(); ++j) {
      auto t = Rules_l[j].first;
      auto jj = Rules_l[j].second;
      if (t.divides(uv)) {
        return jj;
      }
    }
    return k;
  }

  bool rewritable(const M& u, uint k) {
    D("u = " << u << ", r_k = " << L[k]);
    auto kk = rewrite(u, k);
    if (kk != k) {
      D("returning true");
      return true;
    }
    D("returning false");
    return false;
  }

  boost::optional<CriticalPair> CritPair(uint k, uint l, uint i) {
    DD("r_k = " << L[k] << ", r_l = " << L[l] << ", i = " << i << ", G[" << i << "] = ", G[i]);
    auto p1 = poly(k);
    auto p2 = poly(l);
    auto t = lcm(p1.lm(), p2.lm());
    D("p1 = " << p1 << ", p2 = " << p2 << ", t = " << t);
    auto u1 = t / p1.lm();
    auto u2 = t / p2.lm();
    D("u1 = " << u1 << ", u2 = " << u2);

    auto Sr1 = signature(k);
    auto Sr2 = signature(l);
    D("Sr1 = " << Sr1 << ", Sr2 = " << Sr2);

    if (u1*Sr1 < u2*Sr2) {
      D(u1*Sr1 << " < " << u2*Sr2);
      return CritPair(l, k, i);
    }
    auto t1 = Sr1.m;
    auto k1 = Sr1.index;
    P u1t1 = P(C(1), u1*t1);
    D("u1*t1 = " << u1t1);
    if (topReducible(u1t1, k1 + 1)) return boost::optional<CriticalPair>();
    auto t2 = Sr2.m;
    auto k2 = Sr2.index;
    P u2t2 = P(C(1), u2*t2);
    D("u2*t2 = " << u2t2);
    if (topReducible(u2t2, k2 + 1)) return boost::optional<CriticalPair>();
    auto cp = CriticalPair(t, u1, k, u2, l);
    D("returning " << cp);
    return boost::make_optional(cp);
  }

  std::vector<uint> SPols(const std::vector<CriticalPair>& B) {
    DD("B = ", B);
    std::vector<uint> F;
    for (auto p : B) {
      auto u = p.u1;
      auto k = p.r1;
      auto v = p.u2;
      auto l = p.r2;
      D("u = " << u << ", r_k = " << L[k] << ", v = " << v << ", r_l = " << L[l]);
      if (!rewritable(u, k) && !rewritable(v, l)) {
        L.push_back(std::make_pair(p.u1*signature(p.r1), p.u1*poly(p.r1) - p.u2*poly(p.r2)));
        AddRule(L.size() - 1);
        F.push_back(L.size() - 1);
        DD("L = ", L);
      }
    }
    std::sort(F.begin(), F.end(), [this] (uint a, uint b) { return signature(a) < signature(b); });
    return F;
  }

  void ResetSimplificationRules(uint m) {
    D("m = " << m);
    Rules.clear();
    Rules.resize(m);
  }

  bool topReducible(const P& p, uint k) {
    if (p.isZero()) return false;
    return topReducible(p.lm(), k);
  }

  bool topReducible(const M& m, uint k) {
    if (k >= G.size()) return false;

    for (uint i = 0; i < G[k].size(); ++i) {
      const auto& q = poly(G[k][i]);
      if (q.lm().divides(m)) {
        D(m << " is top reducible");
        return true;
      }
    }
    D(m << " is not top reducible");
    return false;
  }

  boost::optional<uint> findReductor(uint k, const std::vector<uint>& GG) {
    auto t = poly(k).lm();
    for (auto j : GG) {
      auto tt = poly(j).lm();
      auto vj = signature(j).m;
      auto kj = index(j);
      if (!tt.divides(t)) continue;
      auto u = tt / t;
      auto uvj = u * vj;
      if (topReducible(uvj, kj + 1)) continue;
      if (rewritable(u, j)) continue;
      if (u*signature(j) == signature(k)) continue;
      return boost::make_optional(j);
    }
    return boost::optional<uint>();
  }

  std::pair<boost::optional<uint>, std::vector<uint> > TopReduction(uint k, const std::vector<uint>& GG) {
    DD("r_k = " << L[k] << ", GG = ", GG);
    DD("L = ", L);
    if (poly(k).isZero()) {
      D("the system is not a regular sequence");
      std::cerr << "!" << std::flush;
      return std::make_pair(boost::optional<uint>(), std::vector<uint>());
    }
    auto J = findReductor(k, GG);
    if (!J) {
      return std::make_pair(boost::make_optional(k), std::vector<uint>());
    }
    auto p = poly(k);
    auto j = *J;
    auto u = poly(k).lm() / poly(j).lm();
    auto q = poly(j) * u;
    auto m_plc = p.lc();
    m_plc *= C(-1);
    p = P::combineAndRenormalize(p, q.lc(), q, m_plc);
    if (u*signature(j) < signature(k)) {
      L[k] = std::make_pair(signature(k), p);
      DD("L = ", L);
      return std::make_pair(boost::optional<uint>(), std::vector<uint>({ k }));
    }
    uint N = L.size();
    L.push_back(std::make_pair(u * signature(j), p));
    AddRule(N);
    DD("L = ", L);
    return std::make_pair(boost::optional<uint>(), std::vector<uint>({ k, N }));
  }

  std::vector<uint> Reduction(std::vector<uint> todo, uint i) {
    DD("i = " << i << ", todo = ", todo);
    std::vector<uint> done;
    while (!todo.empty()) {
      std::cerr << "." << std::flush;
      auto min_element = std::min_element(todo.begin(), todo.end(), [this] (uint a, uint b) { return signature(a) < signature(b); });
      auto k = *min_element;
      todo.erase(min_element);
      auto h = normalForm(poly(k), i);
      D("poly(k) = " << poly(k) << " reduces to h = " << h);
      L[k] = std::make_pair(signature(k), h);
      std::vector<uint> G_i_and_done(G[i]);
      G_i_and_done.insert(G_i_and_done.end(), done.begin(), done.end());
      auto k1_todo1 = TopReduction(k, G_i_and_done);
      if (k1_todo1.first) done.push_back(*(k1_todo1.first));
      todo.insert(todo.end(), k1_todo1.second.begin(), k1_todo1.second.end());
    }
    DD("L = ", L);
    return done;
  }

  void AlgorithmF5(uint i) {
    DD("i = " << i << ", G[" << i + 1 << "] = ", G[i + 1]);
    L[i] = std::make_pair(S::e(i), f[i]);
    DD("L = ", L);
    G[i] = G[i + 1];
    G[i].push_back(i);
    std::vector<CriticalPair> Ps;
    for (auto j : G[i + 1]) {
      auto cp = CritPair(i, j, i + 1);
      if (cp) Ps.push_back(*cp);
    }
    DD("critical pairs = ", Ps);
    while (!Ps.empty()) {
      std::sort(Ps.begin(), Ps.end());
      uint d = Ps.front().t.degree();
      D("d = " << d);
      auto d_end = find_if(Ps.begin(), Ps.end(), [d](const CriticalPair& cp) { return cp.t.degree() != d; });
      std::vector<CriticalPair> Pd(Ps.begin(), d_end);
      Ps.erase(Ps.begin(), d_end);
      DD("critical pairs of degree " << d << " = ", Pd);
      std::cerr << "[" << d << ":" << Pd.size() << "]" << std::flush;
      auto S_d = SPols(Pd);
      auto R_d = Reduction(S_d, i);
      for (auto k : R_d) {
        for (auto l : G[i]) {
          auto cp = CritPair(k, l, i + 1);
          if (cp) Ps.push_back(*cp);
        }
        G[i].push_back(k);
      }
    }
  }

  void homogenize(std::vector<P>& f) {
    bool x0_used = false;
    auto x0 = M::x(M::VAR_COUNT - 1);
    for (auto& p : f) {
      for (const auto& term : p) {
        if (x0.divides(term.m())) {
          x0_used = true;
        }
      }
      if (!p.isHomogeneous()) {
        if (x0_used) {
          throw std::invalid_argument("input not homogeneous and last variable cannot be used to homogenize");
        }
        I(p << " is not homogeneous");
        p = p.homogenize(x0);
      }
    }
  }

  std::vector<P> f5(const std::vector<P>& input) {
    f = input;
    homogenize(f);
    std::sort(f.begin(), f.end());
    DD("f = ", f);
    auto m = f.size();
    ResetSimplificationRules(m);
    L = std::vector<R>();
    for (uint i = 0; i < m; ++i) L.push_back(std::make_pair(S(), P()));
    L.back() = std::make_pair(S::e(m - 1), f.back());
    DD("L = ", L);
    G = std::vector<std::vector<uint> >(m);
    G.back().push_back(m - 1);
    auto i = m - 1;
    do {
      --i;
      I("Starting iteration " << i);
      std::cerr << "{" << i << "}" << std::flush;
      AlgorithmF5(i);
      for (auto r : G[i]) {
        if (poly(r).isConstant()) {
          return std::vector<P>({P(C(1))});
        }
      }
    } while (i != 0);
    I("Finished all iterations");
    std::vector<P> result;
    for (auto r : G[0]) {
      result.push_back(poly(r));
    }
    I("Calling interreduce");
    interreduce(result);
    I("Sorting result");
    std::sort(result.begin(), result.end());
    return result;
  }

  std::vector<std::vector<uint> > G;
  std::vector<P> f;
  std::vector<R> L;
  std::vector<std::vector<std::pair<M, uint> > > Rules;
};

template<class A, class B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B>& pair) {
  return out << "(" << pair.first << ", " << pair.second << ")";
}

#endif // F5_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
