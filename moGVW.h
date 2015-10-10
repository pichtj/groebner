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
  typedef MM<P> MMType;
  typedef lm_R_l<P> lm_R_lType;
  typedef LabelledMonomial<P> LM;
  typedef unordered_set<LM> LMSet;
  typedef set<MMType> MMSet;

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

  bool rejectedBySyzygyCriterion(const LM& m, const LMSet& GG) {
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
    MMSet HH;
    for (auto m = todo.begin(); m != todo.end(); ++m) {
      for (uint i = 0; i < VAR_COUNT; ++i ) {
        LM x_i_m = MonomialType::x(i) * *m;
        bool collides = false;
        for (auto n = GG.begin(); n != GG.end(); ++n) {
          if (x_i_m.collidesWith(*n)) {
            collides = true;
            MonomialType t_f = x_i_m.m() / m->f().lm();
            MonomialType t_g = x_i_m.m() / n->f().lm();
            if (t_f * m->u().lm() > t_g * n->u().lm()
                && !rejectedByLCMCriterion(x_i_m, GG)
                && !rejectedBySyzygyCriterion(x_i_m, GG)
                && !rejectedByRewrittenCriterion(x_i_m, GG)) {
              HH.insert(t_f * MMType(m->u(), m->f()));
              HH.insert(t_g * MMType(n->u(), n->f()));
            }
            if (t_f * m->u().lm() < t_g * n->u().lm()) {
              GG.erase(n);
              GG.insert(x_i_m);
              if (!rejectedByLCMCriterion(*n, GG)
                  && !rejectedBySyzygyCriterion(*n, GG)
                  && !rejectedByRewrittenCriterion(*n, GG)) {
                HH.insert(t_f * MMType(m->u(), m->f()));
                HH.insert(t_g * MMType(n->u(), n->f()));
              }
            }
          }
        }
        if (!collides) {
          GG.insert(x_i_m);
        }
      }
    }
    return HH;
  }

  void append(MMSet& HH, const LMSet& GG) {
  }

  MMSet eliminate(const MMSet& HH) {
    return HH;
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

    while (mindeg <= liftdeg) {
      unordered_set<LabelledMonomial<P> > todo;
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (it->degree() == mindeg && !it->wasLifted()) {
          todo.insert(*it);
        }
      }
      MMSet HH = lift(todo, GG);
      append(HH, GG);
      MMSet PP = eliminate(HH);
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
  }
};

#endif // MOGVW_H
