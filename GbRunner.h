#ifndef GBRUNNER_H
#define GBRUNNER_H

#include "Polynomial.h"
#include "Signature.h"

struct GbRunner {
  template<class P>
  static void interreduce(std::vector<P>& polynomials) {
    typedef typename P::CoefficientType C;

    DD("polynomials = ", polynomials);
    bool stable;
    do {
      stable = true;
      for (auto p = polynomials.begin(); p != polynomials.end(); ++p) {
        auto term = p->begin();
        while (term != p->end()) {
          auto r = polynomials.begin();
          while (r != polynomials.end() && (r->isZero() || r == p || !r->lm().divides(term->m()))) {
            ++r;
          }
          if (r != polynomials.end()) {
            D("reducing " << *p << " with " << *r);
            stable = false;
            auto t = term->m() / r->lm();
            auto rt = *r * t;
            C c = term->c();
            c *= -1;
            *p = P::combineAndRenormalize(*p, r->lc(), rt, c);
            D("to " << *p);
            term = p->begin();
            DD("polynomials = ", polynomials);
          } else {
            ++term;
          }
        }
      }
    } while (!stable);

    auto p = polynomials.begin();
    while (p != polynomials.end()) {
      if (p->isZero()) {
        polynomials.erase(p);
        continue;
      }
      if (p->lc() < 0) *p = *p * C(-1);
      ++p;
    }
    DD("polynomials = ", polynomials);
  }

};

#endif // GBRUNNER_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
