#ifndef MOGVW_H
#define MOGVW_H

#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <vector>
#include <algorithm>
#include <memory>

#include "style.h"
#include "debug.h"
#include "integral.h"
#include "Polynomial.h"
#include "MM.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct moGVWRunner {
  typedef typename P::MonomialType MonomialType;
  typedef typename P::TermType TermType;
  typedef typename P::TermType T;
  typedef typename P::CoefficientType CoefficientType;
  typedef MM<P> MMType;
  typedef MonRl<P> MonRlType;
  typedef std::unordered_map<MonomialType, std::shared_ptr<MMType> > LMSet;
  typedef std::unordered_set<MMType> MMSet;
  typedef std::set<MonomialType> MSet;

  MMType signature(const MonomialType& m, const MMType& uf) const {
    MonomialType t = m / uf.f.lm();
    return MMType(uf.u, T(1, t));
  }

  MMType signature(const std::pair<MonomialType, std::shared_ptr<MMType> >& muf) const {
    return signature(muf.first, *(muf.second));
  }

  bool rejectedByLCMCriterion(const MonomialType& m, std::shared_ptr<MMType> uf, const LMSet& GG) {
    auto lmf = uf->f.lm();
    auto nvg = GG.find(m);
    if (nvg == GG.end()) return false;
    auto lmg = nvg->second->f.lm();
    if (m == lcm(lmf, lmg)) {
      return false;
    }
    if (signature(m, *uf) > signature(*nvg)) {
      D("rejecting (" << m << ", " << *uf << ") because " << *nvg << " has smaller signature");
      return true;
    }
    return false;
  }

  bool rejectedBySyzygyCriterion(const MonomialType& m, std::shared_ptr<MMType> uf, const LMSet& GG) {
    auto lmu = uf->u;
    auto t_f = m / uf->f.lm();
    auto nvg = GG.find(t_f*lmu.m);
    if (nvg == GG.end()) return false;
    auto lmv = nvg->second->u;
    if (lmu.index > lmv.index) {
      D("rejecting (" << m << ", " << *uf << ") because " << *nvg << " has smaller index");
      return true;
    }
    return false;
  }

  bool rejectedByRewrittenCriterion(const MonomialType& m, std::shared_ptr<MMType> uf, const LMSet& GG) {
    auto lmu = uf->u;
    auto lmf = uf->f.lm();
    for (auto nvg = GG.begin(); nvg != GG.end(); ++nvg) {
      auto lmv = nvg->second->u;
      if (!lmv.divides(lmu)) continue;
      auto t = lmu / lmv;
      if (t*nvg->second->f.lm() < lmf) {
        D("rejecting " << *uf);
        return true;
      }
    }
    return false;
  }

  MMSet lift(const LMSet& todo, LMSet& GG) {
    DD("todo = ", todo);

    MMSet HH;
    for (const auto& muf : todo) {
      D("chose " << muf << " to lift");
      for (uint i = 0; i < MonomialType::VAR_COUNT; ++i) {
        auto xim_m = muf.first;
        xim_m[i]++;
        auto uf = muf.second;
        auto nvg = GG.find(xim_m);
        if (nvg != GG.end()) {
          auto vg = nvg->second;
          MonomialType t_f = xim_m / uf->f.lm();
          MonomialType t_g = xim_m / vg->f.lm();
          auto tf_lmu = t_f * uf->u;
          auto tg_lmv = t_g * vg->u;
          if (tf_lmu > tg_lmv
              && !rejectedByLCMCriterion(xim_m, uf, GG)
              && !rejectedBySyzygyCriterion(xim_m, uf, GG)
              && !rejectedByRewrittenCriterion(xim_m, uf, GG)) {
            auto tf_uf = t_f * *uf;
            auto tg_vg = t_g * *vg;
            D("inserting " << tf_uf << " into HH");
            HH.insert(tf_uf);
            D("inserting " << tg_vg << " into HH");
            HH.insert(tg_vg);
          }
          if (tf_lmu < tg_lmv) {
            D("replacing GG[" << xim_m << "] = " << *vg << " by " << *uf);
            GG[xim_m] = uf;
            if (!rejectedByLCMCriterion(xim_m, vg, GG)
                && !rejectedBySyzygyCriterion(xim_m, vg, GG)
                && !rejectedByRewrittenCriterion(xim_m, vg, GG)) {
              auto tf_uf = t_f * *uf;
              auto tg_vg = t_g * *vg;
              D("inserting " << tf_uf << " into HH");
              HH.insert(tf_uf);
              D("inserting " << tg_vg << " into HH");
              HH.insert(tg_vg);
            }
          }
        } else {
          GG[xim_m] = uf;
        }
      }
      wasLifted[muf.first] = true;
    }
    DD("returning HH = ", HH);
    return HH;
  }

  void append(MMSet& HH, const LMSet& GG) {
    MSet done;
    for (const auto& wh : HH) {
      done.insert(wh.f.lm());
    }
    DD("initialized done = ", done);
    MSet monomialsInHH;
    for (const auto& wh : HH) {
      auto terms = wh.f.terms();
      for (const auto& term : terms) {
        if (done.find(term.m()) == done.end())
          monomialsInHH.insert(term.m());
      }
    }
    DD("monomials in HH that are not done = ", monomialsInHH);
    while (!monomialsInHH.empty()) {
      MonomialType m = *(monomialsInHH.begin());
      done.insert(m);
      auto it = GG.find(m);
      if (it != GG.end()) {
        auto vg = it->second;
        MonomialType t = m / vg->f.lm();
        MMType newMM = t * *vg;
        HH.insert(newMM);
        for (const auto& term : newMM.f.terms()) {
          monomialsInHH.insert(term.m());
        }
      }
      for (const auto& it : done) {
        auto found = monomialsInHH.find(it);
        if (found != monomialsInHH.end()) {
          monomialsInHH.erase(found);
        }
      }
    }
    DD("returning HH = ", HH);
  }

  MMSet eliminate(MMSet& HH) {
    MMSet PP;
    MMSet todoInHH = HH;
    while (!todoInHH.empty()) {
      auto uf = *(todoInHH.begin());
      D("working on " << uf);
      HH.erase(uf);
      todoInHH.erase(uf);
      bool found = false;
      auto it = HH.begin();
      while (it != HH.end() && !found) {
        if (it->f.lm() == uf.f.lm()) {
          found = true;
        } else {
          ++it;
        }
      }
      if (!found) {
        it = PP.begin();
      }
      while (it != PP.end() && !found) {
        if (it->f.lm() == uf.f.lm()) {
          found = true;
        } else {
          ++it;
        }
      }
      if (found) {
        auto vg = *it;
        D("reducing " << uf << " with " << vg);
        auto u = uf.u;
        auto v = vg.u;
        auto f = uf.f;
        auto g = vg.f;
        auto lcg = g.lc();
        auto lcf = f.lc();

        if (u > v) {
          auto r = P::combine(f, lcg, g, -lcf);
          if (r.isZero()) {
            D("a row in HH reduced to zero!");
          } else {
            r.renormalize();
            MonRlType w(std::max(u.m, v.m), std::min(u.index, v.index));
            MMType wr = MMType(w, r);
            D("inserting " << wr << " into HH");
            HH.insert(wr);
            todoInHH.insert(wr);
          }
        }
        if (u < v) {
          D("removing " << vg << " from HH");
          HH.erase(vg);
          todoInHH.erase(vg);
          auto r = P::combine(f, -lcg, g, lcf);
          if (r.isZero()) {
            D("a row in HH reduced to zero!");
          } else {
            r.renormalize();
            MonRlType w(std::max(u.m, v.m), std::min(u.index, v.index));
            MMType wr = MMType(w, r);
            D("inserting " << wr << " into HH");
            HH.insert(wr);
            todoInHH.insert(wr);
          }
          D("inserting " << uf << " into HH");
          HH.insert(uf);
        }
      } else {
        D("no reduction found for " << uf);
        PP.insert(uf);
      }
    }
    DD("returning PP = ", PP);
    return PP;
  }

  void update(const MMSet& PP, LMSet& GG) {
    for (const auto& wh : PP) {
      if (wh.f.isZero()) continue;
      auto m = wh.f.lm();
      D("looking for lm(h) = " << m);
      auto mvg = GG.find(m);
      if (mvg != GG.end()) {
        auto v = mvg->second->u;
        auto w = wh.u;
        auto g = mvg->second->f;
        if (((m / g.lm()) * v) > w) {
          D("replacing " << mvg->second << " by " << wh);
          GG[m] = std::shared_ptr<MMType>(new MMType(wh));
        }
      } else {
        D("not found, adding (" << m << ", " << wh << ") to GG");
        GG[m] = std::shared_ptr<MMType>(new MMType(wh));
      }
    }
    DD("returning, GG = ", GG);
    D("GG has " << GG.size() << " elements in " << GG.bucket_count() << " buckets");
  }

  std::set<P> interreduce(const std::set<P>& input) {
    DD("input = ", input);
    std::vector<P> intermediate(input.begin(), input.end());
    bool stable;
    do {
      stable = true;
      for (auto p = intermediate.begin(); p != intermediate.end(); ++p) {
        auto terms = p->terms();
        auto term = terms.begin();
        while (term != terms.end()) {
          auto r = intermediate.begin();
          while (r != intermediate.end() && (r->isZero() || r == p || !r->lm().divides(term->m()))) {
            ++r;
          }
          if (r != intermediate.end()) {
            D("reducing " << *p << " with " << *r);
            stable = false;
            auto t = term->m() / r->lm();
            auto rt = *r * t;
            p->combine(r->lc(), rt, -term->c());
            p->renormalize();
            D("to " << *p);
            terms = p->terms();
            term = terms.begin();
            DD("intermediate = ", intermediate);
          } else {
            ++term;
          }
        }
      }
    } while (!stable);

    for (auto& p : intermediate) {
      if (p.isZero()) continue;
      if (p.lc() < 0) p *= -1;
      p.renormalize();
    }
    std::set<P> result(intermediate.begin(), intermediate.end());
    result.erase(P());
    DD("returning reduced gb = ", result);
    return result;
  }

  bool isPrimitive(const std::pair<MonomialType, std::shared_ptr<MMType> >& lm) const {
    return lm.first == lm.second->f.lm();
  }

  std::set<P> moGVW(const std::set<P>& input) {
    LMSet GG;
    wasLifted.clear();
    typename std::set<P>::size_type i = 0;
    for (const auto& p : input) {
      GG[p.lm()] = std::shared_ptr<MMType>(new MMType(MonRl<P>::e(i), p));
      ++i;
    }

    DD("prefilled GG = ", GG);

    uint liftdeg = 0;
    uint mindeg = std::numeric_limits<uint>::max();
    for (const auto& lm : GG) {
      if (isPrimitive(lm)) {
        liftdeg = std::max(liftdeg, lm.first.degree());
      }
      if (!wasLifted[lm.first]) {
        mindeg = std::min(mindeg, lm.first.degree());
      }
    }
    I("liftdeg = " << liftdeg);
    I("mindeg = " << mindeg);

    while (mindeg <= liftdeg) {
      LMSet todo;
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (it->first.degree() == mindeg && !wasLifted[it->first]) {
          todo[it->first] = it->second;
        }
      }
      I("calling lift");
      MMSet HH = lift(todo, GG);
      I("calling append");
      append(HH, GG);
      I("calling eliminate");
      MMSet PP = eliminate(HH);
      I("calling update");
      update(PP, GG);

      liftdeg = 0;
      mindeg = std::numeric_limits<uint>::max();
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (isPrimitive(*it)) {
          liftdeg = std::max(liftdeg, it->first.degree());
        }
        if (!wasLifted[it->first]) {
          mindeg = std::min(mindeg, it->first.degree());
        }
      }
      I("liftdeg = " << liftdeg);
      I("mindeg = " << mindeg);
    }

    std::set<P> result;
    for (const auto& p : GG) {
      if (isPrimitive(p)) {
        result.insert(p.second->f);
      }
    }
    I("calling interreduce");
    result = interreduce(result);
    II("returning gb = ", result);
    return result;
  }
private:
  std::unordered_map<MonomialType, bool> wasLifted;
};

template<class A, class B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B>& ab) {
  return out << "(" << ab.first << ", " << ab.second << ")";
}

template<class C>
void print(const std::string& prefix, const C& c) {
  std::cout << "{";
  bool itemPrinted = false;
  for (const auto& item : c) {
    if (itemPrinted) std::cout << ",";
    std::cout << std::endl << prefix << "  " << item;
    itemPrinted = true;
  }
  if (itemPrinted) std::cout << std::endl << prefix;
  std::cout << "}" << std::endl;
}

#endif // MOGVW_H
