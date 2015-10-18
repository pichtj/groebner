#ifndef MOGVW_H
#define MOGVW_H

#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <vector>
#include <algorithm>

#define DEBUG
#include "debug.h"

#include "style.h"
#include "debug.h"
#include "integral.h"
#include "Polynomial.h"
#include "LabelledMonomial.h"

using namespace std;

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
        cout << "rejectedByLCMCriterion: rejecting " << muf << " because " << *it << " has smaller signature" << endl;
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
      cout << "rejectedBySyzygyCriterion: rejecting " << muf << endl;
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
        cout << "rejectedByRewrittenCriterion: rejecting " << uf << endl;
        return true;
      }
    }
    return false;
  }

  MMSet lift(const LMSet& todo, LMSet& GG) {
    cout << "lift: todo = "; print("lift: ", todo);

    MMSet HH;
    for (auto it = todo.begin(); it != todo.end(); ++it) {
      LM muf = LM(it->first, it->second);
      cout << "lift: chose " << muf << " to lift" << endl;
      for (uint i = 0; i < MonomialType::VAR_COUNT; ++i ) {
        LM x_i_m = MonomialType::x(i) * muf;
        cout << "lift: lifted to " << x_i_m << endl;
        auto coll = GG.find(x_i_m.m());
        if (coll != GG.end()) {
          LM nvg = LM(coll->first, coll->second);
          cout << "lift: collides with " << nvg << endl;
          MonomialType t_f = x_i_m.m() / muf.f().lm();
          MonomialType t_g = x_i_m.m() / nvg.f().lm();
          if (muf.signature() > nvg.signature()
              && !rejectedByLCMCriterion(x_i_m, GG)
              && !rejectedBySyzygyCriterion(x_i_m, GG)
              && !rejectedByRewrittenCriterion(x_i_m, GG)) {
            cout << "lift: inserting " << t_f * MMType(muf.u(), muf.f()) << " into HH" << endl;
            HH.insert(t_f * MMType(muf.u(), muf.f()));
            cout << "lift: inserting " << t_g * MMType(nvg.u(), nvg.f()) << " into HH" << endl;
            HH.insert(t_g * MMType(nvg.u(), nvg.f()));
          }
          if (muf.signature() < nvg.signature()) {
            cout << "lift: replacing " << nvg << " by " << x_i_m << endl;
            GG.erase(coll);
            GG[x_i_m.m()] = MMType(x_i_m.u(), x_i_m.f());
            if (!rejectedByLCMCriterion(nvg, GG)
                && !rejectedBySyzygyCriterion(x_i_m, GG)
                && !rejectedByRewrittenCriterion(nvg, GG)) {
              cout << "lift: inserting " << t_f * MMType(muf.u(), muf.f()) << " into HH" << endl;
              HH.insert(t_f * MMType(muf.u(), muf.f()));
              cout << "lift: inserting " << t_g * MMType(nvg.u(), nvg.f()) << " into HH" << endl;
              HH.insert(t_g * MMType(nvg.u(), nvg.f()));
            }
          }
        } else {
          //cout << "lift: no collision, inserting " << x_i_m << " into GG directly" << endl;
          GG[x_i_m.m()] = MMType(x_i_m.u(), x_i_m.f());
        }
      }
      wasLifted[muf.m()] = true;
    }
    cout << "lift: returning HH = "; print("lift: ", HH);
    return HH;
  }

  void append(MMSet& HH, const LMSet& GG) {
    MSet done;
    for (auto wh = HH.begin(); wh != HH.end(); ++wh) {
      done.insert(wh->f().lm());
    }
    cout << "append: initialized done = "; print("append: ", done);
    MSet monomialsInHH;
    for (auto wh = HH.begin(); wh != HH.end(); ++wh) {
      MSet monomialsInWH = wh->f().monomials();
      monomialsInHH.insert(monomialsInWH.begin(), monomialsInWH.end());
    }
    for (auto it = done.begin(); it != done.end(); ++it) {
      monomialsInHH.erase(*it);
    }
    cout << "append: monomials in HH that are not done = "; print("append: ", monomialsInHH);
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
    cout << "append: returning HH = "; print("append: ", HH);
  }

  MMSet eliminate(MMSet& HH) {
    MMSet PP;
    MMSet todoInHH = HH;
    while (!todoInHH.empty()) {
      auto uf = *(todoInHH.begin());
      cout << "eliminate: working on " << uf << endl;
      HH.erase(uf);
      todoInHH.erase(uf);
      bool found = false;
      auto vg = HH.begin();
      while (vg != HH.end() && !found) {
        if (vg->f().lm() == uf.f().lm()) {
          found = true;
        } else {
          ++vg;
        }
      }
      if (!found) {
        vg = PP.begin();
      }
      while (vg != PP.end() && !found) {
        if (vg->f().lm() == uf.f().lm()) {
          found = true;
        } else {
          ++vg;
        }
      }
      if (found) {
        cout << "eliminate: reducing " << uf << " with " << *vg << endl;
        auto u = uf.u();
        auto v = vg->u();
        auto f = uf.f();
        auto g = vg->f();
        auto lcg = g.lc();
        auto lcf = f.lc();

        if (u.lm() > v.lm()) {
          auto lcgflcfg = lcg*f-lcf*g;
          if (lcgflcfg.isZero()) {
            cout << "eliminate: a row in HH reduced to zero!" << endl;
          } else {
            auto divisor = gcd(lcgflcfg.coefficients());
            if (divisor != 1) {
              cout << "eliminate: dividing " << lcgflcfg << " by gcd = " << divisor << endl;
              lcgflcfg /= divisor;
            }
            cout << "eliminate: inserting " << MMType(lcg*u-lcf*v, lcgflcfg) << " into HH" << endl;
            HH.insert(MMType(lcg*u-lcf*v, lcgflcfg));
            todoInHH.insert(MMType(lcg*u-lcf*v, lcgflcfg));
          }
        }
        if (u.lm() < v.lm()) {
          cout << "eliminate: removing " << *vg << " from HH" << endl;
          HH.erase(*vg);
          todoInHH.erase(*vg);
          auto lcfglcgf = lcf*g-lcg*f;
          if (lcfglcgf.isZero()) {
            cout << "eliminate: a row in HH reduced to zero!" << endl;
          } else {
            auto divisor = gcd(lcfglcgf.coefficients());
            if (divisor != 1) {
              cout << "eliminate: dividing " << lcfglcgf << " by gcd = " << divisor << endl;
              lcfglcgf /= divisor;
            }
            cout << "eliminate: inserting " << MMType(lcf*v-lcg*u, lcfglcgf) << " into HH" << endl;
            HH.insert(MMType(lcf*v-lcg*u, lcfglcgf));
            todoInHH.insert(MMType(lcf*v-lcg*u, lcfglcgf));
          }
          cout << "eliminate: inserting " << uf << " into HH" << endl;
          HH.insert(uf);
        }
      } else {
        cout << "eliminate: no reduction found for " << uf << endl;
        PP.insert(uf);
      }
    }
    cout << "eliminate: returning PP = "; print("eliminate: ", PP);
    return PP;
  }

  void update(const MMSet& PP, LMSet& GG) {
    for (auto wh = PP.begin(); wh != PP.end(); ++wh) {
      if (wh->f().isZero()) continue;
      auto m = wh->f().lm();
      cout << "update: looking for lm(h) = " << m << endl;
      auto mvg = GG.find(m);
      if (mvg != GG.end()) {
        cout << "update: found (" << mvg->first << ", " << mvg->second << ")" << endl;
        auto v = mvg->second.u();
        auto w = wh->u();
        auto g = mvg->second.f();
        if (((m / g.lm()) * v).lm() > w.lm()) {
          cout << "update: replacing " << mvg->second << " by " << *wh << endl;
          GG[m] = *wh;
        }
      } else {
        cout << "update: not found, adding (" << m << ", " << *wh << ") to GG" << endl;
        GG[m] = *wh;
      }
    }
    cout << "update: returning, GG = "; print("update: ", GG);
    cout << "update: GG has " << GG.size() << " elements in " << GG.bucket_count() << " buckets" << endl;
  }

  std::set<P> interreduce(const std::set<P>& input) {
    cout << "interreduce: input = "; print("interreduce: ", input);
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
            cout << "interreduce: reducing " << *p << " with " << *r;
            stable = false;
            auto t = term->m() / r->lm();
            *p *= r->lc();
            *p -= *r * t * term->c();
            cout << " to " << *p << endl;
            terms = p->terms();
            term = terms.begin();
            cout << "interreduce: intermediate = "; print("interreduce: ", intermediate);
          } else {
            ++term;
          }
        }
      }
    } while (!stable);

    for (auto& p : intermediate) {
      if (p.isZero()) continue;
      if (p.lc() < 0) p *= -1;
      auto divisor = gcd(p.coefficients());
      if (divisor != 1) {
        cout << "interreduce: dividing " << p << " by gcd = " << divisor << endl;
        p /= divisor;
      }
    }
    std::set<P> result(intermediate.begin(), intermediate.end());
    result.erase(P());
    cout << "interreduce: returning reduced gb = "; print("interreduce: ", result);
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

    cout << "moGVW: prefilled GG = "; print("moGVW: ", GG);

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
    cout << "moGVW: liftdeg = " << liftdeg << endl;
    cout << "moGVW: mindeg = " << mindeg << endl;

    while (mindeg <= liftdeg) {
      unordered_map<MonomialType, MMType> todo;
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (it->first.degree() == mindeg && !wasLifted[it->first]) {
          todo[it->first] = it->second;
        }
      }
      cout << "moGVW: calling lift" << endl;
      MMSet HH = lift(todo, GG);
      cout << "moGVW: calling append" << endl;
      append(HH, GG);
      cout << "moGVW: calling eliminate" << endl;
      MMSet PP = eliminate(HH);
      cout << "moGVW: calling update" << endl;
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
      cout << "moGVW: liftdeg = " << liftdeg << endl;
      cout << "moGVW: mindeg = " << mindeg << endl;
    }

    std::set<P> result;
    for (const auto& p : GG) {
      if (isPrimitive(p)) {
        result.insert(p.second.f());
      }
    }
    cout << "moGVW: calling interreduce" << endl;
    result = interreduce(result);
    cout << "moGVW: returning gb = "; print("moGVW: ", result);
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
