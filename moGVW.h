#ifndef MOGVW_H
#define MOGVW_H

#include <set>
#include <unordered_set>
#include <limits>
#include <vector>

#include "style.h"
#include "Polynomial.h"
#include "LabelledMonomial.h"

using namespace std;

#define INPUT_COUNT 33

template<class P = Polynomial<Term<int, Monomial<char> > > >
class moGVWRunner {
public:
  typedef typename P::MonomialType MonomialType;
  typedef typename P::CoefficientType CoefficientType;
  typedef MM<P> MMType;
  typedef lm_R_l<P> lm_R_lType;
  typedef LabelledMonomial<P> LM;
  typedef unordered_set<LM> LMSet;
  typedef set<MMType> MMSet;
  typedef set<MonomialType> MSet;

  bool rejectedByLCMCriterion(const LM& muf, const LMSet& GG) {
    MonomialType m = muf.m();
    MonomialType lmu = muf.u().lm();
    MonomialType lmf = muf.f().lm();
    MonomialType t_f = m / lmf;
    for (auto mvg = GG.begin(); mvg != GG.end(); ++mvg) {
      if (mvg->m() != m) continue;
      MonomialType lmv = mvg->u().lm();
      MonomialType lmg = mvg->f().lm();
      MonomialType t_g = m / lmg;
      if (m == lcm(lmf, lmg)) continue;
      if (t_f * lmu > t_g * lmv) {
        return true;
      }
    }
    return false;
  }

  bool rejectedByRewrittenCriterion(const LM& uf, const LMSet& GG) {
    for (auto vg = GG.begin(); vg != GG.end(); ++vg) {
      MonomialType lmu = uf.u().lm();
      MonomialType lmv = vg->u().lm();
      if (!lmv.divides(lmu)) continue;
      MonomialType t = uf.u().lm() / vg->u().lm();
      if (t*vg->f().lm() < uf.f().lm()) {
        return true;
      }
    }
    return false;
  }

  MMSet lift(const LMSet& todo, LMSet& GG) {
    cout << "lift: todo = "; print("lift: ", todo);
    cout << "lift: GG = "; print("lift: ", GG);

    MMSet HH;
    for (auto muf = todo.begin(); muf != todo.end(); ++muf) {
      cout << "lift: chose " << *muf << " to lift" << endl;
      for (uint i = 0; i < VAR_COUNT; ++i ) {
        LM x_i_m = MonomialType::x(i) * *muf;
        cout << "lift: lifted to " << x_i_m << endl;
        bool collides = false;
        for (auto nvg = GG.begin(); nvg != GG.end(); ++nvg) {
          if (x_i_m.collidesWith(*nvg)) {
            cout << "lift: collides with " << *nvg << endl;
            collides = true;
            MonomialType t_f = x_i_m.m() / muf->f().lm();
            MonomialType t_g = x_i_m.m() / nvg->f().lm();
            if (t_f * muf->u().lm() > t_g * nvg->u().lm()
                && !rejectedByLCMCriterion(x_i_m, GG)
                && !rejectedByRewrittenCriterion(x_i_m, GG)) {
              cout << "lift: inserting " << t_f * MMType(muf->u(), muf->f()) << " into HH" << endl;
              HH.insert(t_f * MMType(muf->u(), muf->f()));
              cout << "lift: inserting " << t_g * MMType(nvg->u(), nvg->f()) << " into HH" << endl;
              HH.insert(t_g * MMType(nvg->u(), nvg->f()));
            }
            if (t_f * muf->u().lm() < t_g * nvg->u().lm()) {
              cout << "lift: replacing " << *nvg << " by " << x_i_m << endl;
              GG.erase(nvg);
              GG.insert(x_i_m);
              if (!rejectedByLCMCriterion(*nvg, GG)
                  && !rejectedByRewrittenCriterion(*nvg, GG)) {
                cout << "lift: inserting " << t_f * MMType(muf->u(), muf->f()) << " into HH" << endl;
                HH.insert(t_f * MMType(muf->u(), muf->f()));
                cout << "lift: inserting " << t_g * MMType(nvg->u(), nvg->f()) << " into HH" << endl;
                HH.insert(t_g * MMType(nvg->u(), nvg->f()));
              }
            }
            break;
          }
        }
        if (!collides) {
          cout << "lift: no collision, inserting " << x_i_m << " into GG directly" << endl;
          GG.insert(x_i_m);
        }
      }
      GG.erase(*muf);
      LM lm = *muf;
      lm.setLifted();
      GG.insert(lm);
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
      cout << "append: monomials in " << *wh << " = "; print("append: ", monomialsInWH);
      monomialsInHH.insert(monomialsInWH.begin(), monomialsInWH.end());
    }
    cout << "append: monomials in HH = "; print("append: ", monomialsInHH);
    for (auto it = done.begin(); it != done.end(); ++it) {
      monomialsInHH.erase(*it);
    }
    cout << "append: monomials in HH that are not done = "; print("append: ", monomialsInHH);
    while (!monomialsInHH.empty()) {
      MonomialType m = *(monomialsInHH.begin());
      done.insert(m);
      for (auto mvg = GG.begin(); mvg != GG.end(); ++mvg) {
        if (mvg->m() != m) continue;
        MonomialType t = m / mvg->f().lm();
        MMType newMM = t*MMType(mvg->u(), mvg->f());
        HH.insert(newMM);
        auto newMMMonomials = newMM.f().monomials();
        monomialsInHH.insert(newMMMonomials.begin(), newMMMonomials.end());
        break;
      }
      monomialsInHH.erase(done.begin(), done.end());
    }
    cout << "append: returning HH = "; print("append: ", HH);
  }

  MMSet eliminate(MMSet& HH) {
    cout << "eliminate: " << endl;
    MMSet PP;
    cout << "eliminate: declared PP" << endl;
    while (!HH.empty()) {
      auto uf = HH.begin();
      cout << "eliminate: working on " << *uf << endl;
      HH.erase(uf);
      bool found = false;
      auto vg = HH.begin();
      while (vg != HH.end()) {
        if (vg->f().lm() == uf->f().lm()) {
          found = true;
          break;
        }
        ++vg;
      }
      if (!found) {
        vg = PP.begin();
        while (vg != PP.end()) {
          if (vg->f().lm() == uf->f().lm()) {
            found = true;
            break;
          }
          ++vg;
        }
      }
      if (found) {
        cout << "eliminate: reducing " << *uf << " with " << *vg << endl;
        auto u = uf->u();
        auto v = vg->u();
        auto f = uf->f();
        auto g = vg->f();
        auto lcg = g.lc();
        auto lcf = f.lc();

        if (u.lm() > v.lm()) {
//          cout << "eliminate: inserting " << MMType(lcg*u-lcf*v, lcg*f-lcf*g) << " into HH" << endl;
//          HH.insert(MMType(lcg*u-lcf*v, lcg*f-lcf*g));
        }
        if (u.lm() < v.lm()) {
          cout << "eliminate: removing " << *vg << " from HH" << endl;
          HH.erase(*vg);
//          cout << "eliminate: inserting " << MMType(lcf*v-lcg*u, lcf*g-lcg*f) << " into HH" << endl;
//          HH.insert(MMType(lcf*v-lcg*u, lcf*g-lcg*f));
          cout << "eliminate: inserting " << *uf << " into HH" << endl;
          HH.insert(*uf);
        }
      } else {
        cout << "eliminate: no reduction found for " << *uf << endl;
        PP.insert(*uf);
      }
    }
    return PP;
  }

  void update(MMSet& PP, const LMSet& GG) {
    for (auto wh = PP.begin(); wh != PP.end(); ++wh) {
      if (wh->f().isZero()) continue;
    }
  }

  template<class PSet>
  PSet moGVW(const PSet& input) {
    LMSet GG;
    for (typename PSet::size_type i = 0; i < input.size(); ++i) {
      GG.insert(LabelledMonomial<P>(input[i].lm(), lm_R_l<P>::e(i), input[i]));
    }

    cout << "moGVW: prefilled GG = "; print("moGVW: ", GG);

    uint liftdeg = 0;
    uint mindeg = numeric_limits<uint>::max();
    for (auto it = GG.begin(); it != GG.end(); ++it) {
      if (it->isPrimitive()) {
        liftdeg = max(liftdeg, it->degree());
      }
      if (!it->wasLifted()) {
        mindeg = min(mindeg, it->degree());
      }
    }
    cout << "moGVW: liftdeg = " << liftdeg << endl;
    cout << "moGVW: mindeg = " << mindeg << endl;

    while (mindeg <= liftdeg) {
      unordered_set<LabelledMonomial<P> > todo;
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (it->degree() == mindeg && !it->wasLifted()) {
          todo.insert(*it);
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
        if (it->isPrimitive()) {
          liftdeg = max(liftdeg, it->degree());
        }
        if (!it->wasLifted()) {
          mindeg = min(mindeg, it->degree());
        }
      }
    }

    PSet result;
    for (auto muf = GG.begin(); muf != GG.end(); ++muf) {
      if (muf->isPrimitive()) {
        result.push_back(muf->f());
      }
    }
    return result;
  }
};

template<class C>
void print(const string& prefix, const C& c) {
  cout << "{" << endl;
  for (auto it = c.begin(); it != c.end(); ++it) {
    cout << prefix << "  " << *it << endl;
  }
  cout << prefix << "}" << endl;

}

#endif // MOGVW_H
