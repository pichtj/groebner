#ifndef MOGVW_H
#define MOGVW_H

#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <vector>
#include <algorithm>

#include "style.h"
#include "debug.h"
#include "integral.h"
#include "Polynomial.h"
#include "LabelledMonomial.h"

template<class P = Polynomial<Term<int, Monomial<char> > > >
class moGVWRunner {
public:
  typedef typename P::MonomialType MonomialType;
  typedef typename P::CoefficientType CoefficientType;
  typedef MM<P> MMType;
  typedef lm_R_l<P> lm_R_lType;
  typedef LabelledMonomial<P> LM;
  typedef unordered_map<MonomialType, MMType> LMSet;
  typedef unordered_set<MMType> MMSet;
  typedef set<MonomialType> MSet;

  bool rejectedByLCMCriterion(const LM& muf, const LMSet& GG) {
    auto m = muf.m();
    auto lmf = muf.f().lm();
    auto it = GG.find(m);
    if (it != GG.end()) {
      auto vg = it->second;
      auto lmg = vg.f().lm();
      if (m == lcm(lmf, lmg)) {
        return false;
      }
      if (muf.signature() > LM(it->first, it->second).signature()) {
        D("rejecting " << muf << " because " << *it << " has smaller signature");
        return true;
      }
    }
    return false;
  }

  bool rejectedBySyzygyCriterion(const LM& muf, const LMSet& GG) {
    auto lmu = muf.u().lm();
    auto t_f = muf.m() / muf.f().lm();
    auto it = GG.find(t_f*lmu.m());
    if (it == GG.end()) return false;
    auto lmv = it->second.u().lm();
    if (lmu.index() > lmv.index()) {
      D("rejecting " << muf << " because " << *it << " has smaller index");
      return true;
    }
    return false;
  }

  bool rejectedByRewrittenCriterion(const LM& uf, const LMSet& GG) {
    for (auto vg = GG.begin(); vg != GG.end(); ++vg) {
      auto lmu = uf.u().lm();
      auto v = vg->second.u();
      auto lmv = v.lm();
      if (!lmv.divides(lmu)) continue;
      auto t = lmu / lmv;
      if (t*vg->second.f().lm() < uf.f().lm()) {
        D("rejecting " << uf);
        return true;
      }
    }
    return false;
  }

  MMSet lift(const LMSet& todo, LMSet& GG) {
    DD("todo = ", todo);

    MMSet HH;
    for (auto it = todo.begin(); it != todo.end(); ++it) {
      LM muf = LM(it->first, it->second);
      D("chose " << muf << " to lift");
      for (uint i = 0; i < MonomialType::VAR_COUNT; ++i ) {
        LM x_i_m = MonomialType::x(i) * muf;
        D("lifted to " << x_i_m);
        auto coll = GG.find(x_i_m.m());
        if (coll != GG.end()) {
          LM nvg = LM(coll->first, coll->second);
          D("collides with " << nvg);
          MonomialType t_f = x_i_m.m() / muf.f().lm();
          MonomialType t_g = x_i_m.m() / nvg.f().lm();
          if (muf.signature() > nvg.signature()
              && !rejectedByLCMCriterion(x_i_m, GG)
              && !rejectedBySyzygyCriterion(x_i_m, GG)
              && !rejectedByRewrittenCriterion(x_i_m, GG)) {
            D("inserting " << t_f * MMType(muf.u(), muf.f()) << " into HH");
            HH.insert(t_f * MMType(muf.u(), muf.f()));
            D("inserting " << t_g * MMType(nvg.u(), nvg.f()) << " into HH");
            HH.insert(t_g * MMType(nvg.u(), nvg.f()));
          }
          if (muf.signature() < nvg.signature()) {
            D("replacing " << nvg << " by " << x_i_m);
            GG.erase(coll);
            GG[x_i_m.m()] = MMType(x_i_m.u(), x_i_m.f());
            if (!rejectedByLCMCriterion(nvg, GG)
                && !rejectedBySyzygyCriterion(x_i_m, GG)
                && !rejectedByRewrittenCriterion(nvg, GG)) {
              D("inserting " << t_f * MMType(muf.u(), muf.f()) << " into HH");
              HH.insert(t_f * MMType(muf.u(), muf.f()));
              D("inserting " << t_g * MMType(nvg.u(), nvg.f()) << " into HH");
              HH.insert(t_g * MMType(nvg.u(), nvg.f()));
            }
          }
        } else {
          GG[x_i_m.m()] = MMType(x_i_m.u(), x_i_m.f());
        }
      }
      wasLifted[muf.m()] = true;
    }
    DD("returning HH = ", HH);
    return HH;
  }

  void append(MMSet& HH, const LMSet& GG) {
    MSet done;
    for (auto wh = HH.begin(); wh != HH.end(); ++wh) {
      done.insert(wh->f().lm());
    }
    DD("initialized done = ", done);
    MSet monomialsInHH;
    for (auto wh = HH.begin(); wh != HH.end(); ++wh) {
      MSet monomialsInWH = wh->f().monomials();
      monomialsInHH.insert(monomialsInWH.begin(), monomialsInWH.end());
    }
    for (auto it = done.begin(); it != done.end(); ++it) {
      monomialsInHH.erase(*it);
    }
    DD("monomials in HH that are not done = ", monomialsInHH);
    while (!monomialsInHH.empty()) {
      MonomialType m = *(monomialsInHH.begin());
      done.insert(m);
      auto it = GG.find(m);
      if (it != GG.end()) {
        MMType vg = it->second;
        MonomialType t = m / vg.f().lm();
        MMType newMM = t*vg;
        HH.insert(newMM);
        auto newMMMonomials = newMM.f().monomials();
        monomialsInHH.insert(newMMMonomials.begin(), newMMMonomials.end());
      }
      for (auto it = done.begin(); it != done.end(); ++it) {
        auto found = monomialsInHH.find(*it);
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
        if (it->f().lm() == uf.f().lm()) {
          found = true;
        } else {
          ++it;
        }
      }
      if (!found) {
        it = PP.begin();
      }
      while (it != PP.end() && !found) {
        if (it->f().lm() == uf.f().lm()) {
          found = true;
        } else {
          ++it;
        }
      }
      if (found) {
        auto vg = *it;
        D("reducing " << uf << " with " << vg);
        auto u = uf.u();
        auto v = vg.u();
        auto f = uf.f();
        auto g = vg.f();
        auto lcg = g.lc();
        auto lcf = f.lc();

        if (u.lm() > v.lm()) {
          auto r = P::combine(f, lcg, g, -lcf);
          if (r.isZero()) {
            D("a row in HH reduced to zero!");
          } else {
            r.renormalize();
            D("inserting " << MMType(lcg*u-lcf*v, r) << " into HH");
            HH.insert(MMType(lcg*u-lcf*v, r));
            todoInHH.insert(MMType(lcg*u-lcf*v, r));
          }
        }
        if (u.lm() < v.lm()) {
          D("removing " << vg << " from HH");
          HH.erase(vg);
          todoInHH.erase(vg);
          auto r = P::combine(f, -lcg, g, lcf);
          if (r.isZero()) {
            D("a row in HH reduced to zero!");
          } else {
            r.renormalize();
            D("inserting " << MMType(lcf*v-lcg*u, r) << " into HH");
            HH.insert(MMType(lcf*v-lcg*u, r));
            todoInHH.insert(MMType(lcf*v-lcg*u, r));
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
    for (auto wh = PP.begin(); wh != PP.end(); ++wh) {
      if (wh->f().isZero()) continue;
      auto m = wh->f().lm();
      D("looking for lm(h) = " << m);
      auto mvg = GG.find(m);
      if (mvg != GG.end()) {
        D("found (" << mvg->first << ", " << mvg->second << ")");
        auto v = mvg->second.u();
        auto w = wh->u();
        auto g = mvg->second.f();
        if (((m / g.lm()) * v).lm() > w.lm()) {
          D("replacing " << mvg->second << " by " << *wh);
          GG[m] = *wh;
        }
      } else {
        D("not found, adding (" << m << ", " << *wh << ") to GG");
        GG[m] = *wh;
      }
    }
    DD("returning, GG = ", GG);
    D("GG has " << GG.size() << " elements in " << GG.bucket_count() << " buckets");
  }

  std::set<P> interreduce(const std::set<P>& input) {
    DD("input = ", input);
    vector<P> intermediate(input.begin(), input.end());
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
            *p *= r->lc();
            *p -= *r * t * term->c();
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

  bool isPrimitive(const std::pair<MonomialType, MMType>& lm) const {
    return lm.first == lm.second.f().lm();
  }

  std::set<P> moGVW(const std::set<P>& input) {
    LMSet GG;
    wasLifted.clear();
    typename std::set<P>::size_type i = 0;
    for (const auto& p : input) {
      GG[p.lm()] = MMType(lm_R_l<P>::e(i), p);
      ++i;
    }

    DD("prefilled GG = ", GG);

    uint liftdeg = 0;
    uint mindeg = numeric_limits<uint>::max();
    for (const auto& lm : GG) {
      if (isPrimitive(lm)) {
        liftdeg = max(liftdeg, lm.first.degree());
      }
      if (!wasLifted[lm.first]) {
        mindeg = min(mindeg, lm.first.degree());
      }
    }
    I("liftdeg = " << liftdeg);
    I("mindeg = " << mindeg);

    while (mindeg <= liftdeg) {
      unordered_map<MonomialType, MMType> todo;
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
      mindeg = numeric_limits<uint>::max();
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (isPrimitive(*it)) {
          liftdeg = max(liftdeg, it->first.degree());
        }
        if (!wasLifted[it->first]) {
          mindeg = min(mindeg, it->first.degree());
        }
      }
      I("liftdeg = " << liftdeg);
      I("mindeg = " << mindeg);
    }

    std::set<P> result;
    for (const auto& p : GG) {
      if (isPrimitive(p)) {
        result.insert(p.second.f());
      }
    }
    I("calling interreduce");
    result = interreduce(result);
    II("returning gb = ", result);
    return result;
  }
private:
  unordered_map<MonomialType, bool> wasLifted;
};

template<class A, class B>
std::ostream& operator<<(ostream& out, const std::pair<A, B>& ab) {
  return out << "(" << ab.first << ", " << ab.second << ")";
}

template<class C>
void print(const string& prefix, const C& c) {
  cout << "{";
  bool itemPrinted = false;
  for (const auto& item : c) {
    if (itemPrinted) cout << ",";
    cout << endl << prefix << "  " << item;
    itemPrinted = true;
  }
  if (itemPrinted) cout << endl << prefix;
  cout << "}" << endl;
}

#endif // MOGVW_H
