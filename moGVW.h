#include <set>
#include <unordered_set>
#include <limits>
#include <vector>

#include "style.h"
#include "Polynomial.h"
#include "LabelledMonomial.h"

using namespace std;

#define INPUT_COUNT 33

template<class C, class E>
bool rejectedByLCMCriterion(const LabelledMonomial<C, E>& m) {
  return false;
}

template<class C, class E>
bool rejectedBySyzygyCriterion(const LabelledMonomial<C, E>& m) {
  return false;
}

template<class C, class E>
bool rejectedByRewrittenCriterion(const LabelledMonomial<C, E>& m) {
  return false;
}

template<class C, class E>
set<MM<C, E> > lift(const unordered_set<LabelledMonomial<C, E> >& todo, unordered_set<LabelledMonomial<C, E> >& GG) {
  set<MM<C, E> > HH;
  for (auto m = todo.begin(); m != todo.end(); ++m) {
    for (uint i = 0; i < VAR_COUNT; ++i ) {
      LabelledMonomial<C, E> x_i_m = Monomial<E>::x(i) * *m;
      bool collides = false;
      for (auto n = GG.begin(); n != GG.end(); ++n) {
        if (x_i_m.collidesWith(*n)) {
          collides = true;
          Monomial<E> t_f = x_i_m.m() / m->f().lm();
          Monomial<E> t_g = x_i_m.m() / n->f().lm();
          if (t_f * m->u().lm() > t_g * n->u().lm()
              && !rejectedByLCMCriterion(x_i_m)
              && !rejectedBySyzygyCriterion(x_i_m)
              && !rejectedByRewrittenCriterion(x_i_m)) {
            HH.insert(t_f * MM<C, E>(m->u(), m->f()));
            HH.insert(t_g * MM<C, E>(n->u(), n->f()));
          }
          if (t_f * m->u().lm() < t_g * n->u().lm()) {
            GG.erase(n);
            GG.insert(x_i_m);
            if (!rejectedByLCMCriterion(*n)
                && !rejectedBySyzygyCriterion(*n)
                && !rejectedByRewrittenCriterion(*n)) {
              HH.insert(t_f * MM<C, E>(m->u(), m->f()));
              HH.insert(t_g * MM<C, E>(n->u(), n->f()));
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

template<class C, class E>
void append(set<MM<C, E> >& HH, const unordered_set<LabelledMonomial<C, E> >& GG) {
}

template<class C, class E>
set<MM<C, E> > eliminate(const set<MM<C, E> >& HH) {
  return HH;
}

template<class C, class E>
void update(set<MM<C, E> >& PP, const unordered_set<LabelledMonomial<C, E> >& GG) {
  for (auto wh = PP.begin(); wh != PP.end(); ++wh) {
    if (wh->f.isZero()) continue;
  }
}

template<class C = int, class E = char, class PSet>
PSet moGVW(const PSet& input) {
  unordered_set<LabelledMonomial<C, E> > GG;
  for (typename PSet::size_type i = 0; i < input.size(); ++i) {
    GG.insert(LabelledMonomial<C, E>(input[i].lm(), lm_R_l<C, E>::e(i), input[i]));
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
    unordered_set<LabelledMonomial<C, E> > todo;
    for (auto it = GG.begin(); it != GG.end(); ++it) {
      if (it->degree() == mindeg && !it->wasLifted()) {
        todo.insert(*it);
      }
    }
    set<MM<C, E> > HH = lift(todo, GG);
    append(HH, GG);
    set<MM<C, E> > PP = eliminate(HH);
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
