#include "polynomial.h"

#include <set>
using namespace std;

typedef struct lm_R_l {
  exp_t lm_u[INPUT_COUNT];
} lm_R_l_t;

exp_t lm(lm_R_l_t lm_R_l);

typedef struct M {
  lm_R_l_t u;
  poly_t f;
} M_t;

M_t jPair(M_t uf, M_t vg);

typedef struct lmon {
  exp_t m;
  M_t generator;
} lmon_t;

inline exp_t monomial(lmon_t lmon) { return lmon.m; }
inline unsigned int degree(lmon_t lmon) { return degree(monomial(lmon)); }
inline exp_t signature(lmon_t lmon) {
  exp_t t = div(lmon.m, lm(lmon.generator.f));
  return mul(t, lm(lmon.generator.u));
}

lmon_t mul(exp_t exp, lmon_t lmon);

inline bool collide(lmon_t a, lmon_t b) {
  for (unsigned int i = 0; i < VAR_COUNT; ++i) {
    if (a.m.exp[i] != b.m.exp[i]) {
      return false;
    }
  }
  for (unsigned int i = 0; i < VAR_COUNT; ++i) {
    if (a.m.exp[i] != 0) {
      return true;
    }
  }
  return false;
}

bool reducible(lmon_t a, set<lmon_t> B);
lmon_t reduce(lmon_t a, set<lmon_t> B);
lmon_t mutual_reduce(lmon_t a, set<lmon_t> B);

set<M_t> lift(set<lmon_t> todo, set<lmon_t> G);
void append(set<M_t> H, set<lmon_t> G);
set<M_t> eliminate(set<M_t> H);
void update(set<M_t> P, set<lmon_t> G);
set<poly_t> moGVW(set<poly_t> input);
