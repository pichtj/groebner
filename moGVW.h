#ifndef MOGVW_H
#define MOGVW_H

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <limits>
#include <vector>
#include <algorithm>
#include <memory>

#include "fmpz.h"
#include "fmpz_mat.h"

#include "style.h"
#include "debug.h"
#include "GbRunner.h"
#include "integral.h"
#include "MM.h"

#ifdef PNG_OUTPUT
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/io/png_io.hpp>
using namespace boost::gil;
#endif // PNG_OUTPUT

template<class P = Polynomial<Term<int, Monomial<char> > > >
struct moGVWRunner : public GbRunner {
  typedef typename P::TermType T;
  typedef typename P::MonomialType M;
  typedef typename P::CoefficientType C;
  typedef Signature<P> S;
  typedef MM<P> MMP;
  typedef std::unordered_map<M, MMP> LMSet;
  typedef std::unordered_set<MMP> MMSet;
  typedef std::set<M> MSet;

  MMP signature(const M& m, MMP uf) const {
    M t = m / uf.f().lm();
    return MMP(uf.u(), T(C(1), t));
  }
  MMP signature(const std::pair<M, MMP>& muf) const { return signature(muf.first, muf.second); }

  bool rejectedByLCMCriterion(const M& m, MMP uf, const LMSet& GG) {
    auto lmf = uf.f().lm();
    auto nvg = GG.find(m);
    if (nvg == GG.end()) return false;
    auto lmg = nvg->second.f().lm();
    if (m == lcm(lmf, lmg)) {
      return false;
    }
    if (signature(m, uf) > signature(*nvg)) {
      D("rejecting (" << m << ", " << uf << ") because " << *nvg << " has smaller signature");
      return true;
    }
    return false;
  }

  bool rejectedBySyzygyCriterion(const M& m, MMP uf, const LMSet& GG) {
    auto lmu = uf.u();
    auto t_f = m / uf.f().lm();
    auto nvg = GG.find(t_f*lmu.m);
    if (nvg == GG.end()) return false;
    auto lmv = nvg->second.u();
    if (lmu.index > lmv.index) {
      D("rejecting (" << m << ", " << uf << ") because " << *nvg << " has smaller index");
      return true;
    }
    return false;
  }

  bool rejectedByRewrittenCriterion(const M& m, MMP uf, const LMSet& GG) {
    auto lmu = uf.u();
    auto lmf = uf.f().lm();
    for (auto nvg = GG.begin(); nvg != GG.end(); ++nvg) {
      auto lmv = nvg->second.u();
      if (!lmv.divides(lmu)) continue;
      auto t = lmu / lmv;
      if (t*nvg->second.f().lm() < lmf) {
        D("rejecting " << uf);
        return true;
      }
    }
    return false;
  }

  MMSet lift(const LMSet& todo, LMSet& GG, uint max_var) {
    DD("todo = ", todo);

    MMSet HH;
    static uint count = 0;
    for (const auto& muf : todo) {
      D("chose " << muf << " to lift");
      if (count > 0 && count % 1000 == 0) {
        std::cerr << "l" << std::flush;
        ++count;
      }
      for (uint i = 0; i <= max_var; ++i) {
        auto xim_m = muf.first;
        xim_m *= M::x(i);
        auto uf = muf.second;
        auto nvg = GG.find(xim_m);
        if (nvg != GG.end()) {
          auto vg = nvg->second;
          M t_f = xim_m / uf.f().lm();
          M t_g = xim_m / vg.f().lm();
          auto tf_lmu = t_f * uf.u();
          auto tg_lmv = t_g * vg.u();
          if (tf_lmu > tg_lmv
              && !rejectedByLCMCriterion(xim_m, uf, GG)
              && !rejectedBySyzygyCriterion(xim_m, uf, GG)
              && !rejectedByRewrittenCriterion(xim_m, uf, GG)) {
            auto tf_uf = t_f * uf;
            auto tg_vg = t_g * vg;
            D("inserting " << tf_uf << " into HH");
            HH.insert(tf_uf);
            D("inserting " << tg_vg << " into HH");
            HH.insert(tg_vg);
          }
          if (tf_lmu < tg_lmv) {
            D("replacing GG[" << xim_m << "] = " << vg << " by " << uf);
            GG[xim_m] = uf;
            if (!rejectedByLCMCriterion(xim_m, vg, GG)
                && !rejectedBySyzygyCriterion(xim_m, vg, GG)
                && !rejectedByRewrittenCriterion(xim_m, vg, GG)) {
              auto tf_uf = t_f * uf;
              auto tg_vg = t_g * vg;
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

  void append(MMSet& HH, const LMSet& GG, MMP vg) {
    if (HH.size() % 1000 == 0) {
      D(HH.size() << " elements in HH");
      std::cerr << "a" << std::flush;
    }
    HH.insert(vg);
    for (const auto& term : vg.f()) {
      auto m = term.m();
      auto it = GG.find(m);
      if (it != GG.end()) {
        auto vg = it->second;
        M t = m / vg.f().lm();
        MMP newMM = t * vg;
        if (HH.find(newMM) == HH.end()) {
          append(HH, GG, newMM);
        }
      }
    }
  }

  void append(MMSet& HH, const LMSet& GG) {
    MMSet HH2;
    for (const auto& wh : HH) {
      append(HH2, GG, wh);
    }
    HH.insert(HH2.begin(), HH2.end());
    DD("returning HH = ", HH);
  }

  struct row {
    row(MMP vg) : uf(vg), done(vg.f().isZero()) {}

    bool operator<(row other) const {
      return uf < other.uf;
    }
    bool operator>(row other) const { return other < *this; }

    S u() const { return uf.u(); }
    const P& f() const { return uf.f(); }

    MMP uf;
    bool done;
  };

  struct PolynomialMatrix {
    PolynomialMatrix(const MMSet& HH) {
      for (auto uf : HH) {
        rows.push_back(row(uf));
        for (const auto& term : uf.f()) {
          monomials.insert(term.m());
        }
      }

      std::stable_sort(rows.begin(), rows.end(), std::greater<row>());
    }

    friend std::ostream& operator<<(std::ostream& out, const typename moGVWRunner<P>::PolynomialMatrix& m) {
      out << "[" << m.rows.size() << "x" << m.monomials.size() << "]";
#ifdef DEBUG
      if (m.monomials.size() > 30) return out;
      out << std::endl;
      for (const auto& monomial : m.monomials) {
        out << "\t" << monomial;
      }
      for (const auto& row : m.rows) {
        out << std::endl;
        for (const auto& monomial : m.monomials) {
          auto c = row.f()[monomial];
          out << "\t";
          if (c != C())
            out << c;
        }
        out << "\t" << row.uf.u();
      }
#endif // DEBUG
      return out;
    }

#ifdef PNG_OUTPUT
    typedef point2<ptrdiff_t>   point_t;

    typedef PolynomialMatrix    const_t;
    typedef gray8_pixel_t       value_type;
    typedef value_type          reference;
    typedef value_type          const_reference;
    typedef point_t             argument_type;
    typedef reference           result_type;
    BOOST_STATIC_CONSTANT(bool, is_mutable=false);

    result_type operator()(const point_t& p) const {
      auto monomial = monomials.begin();
      for (uint i = 0; i < p.x; ++i) ++monomial;
      value_type result;
      if (rows[p.y].f()[*monomial] == C(0)) {
        for (int k = 0; k < num_channels<value_type>::value; ++k)
          result[k] = (typename channel_type<value_type>::type)(0);
      } else {
        for (int k = 0; k < num_channels<value_type>::value; ++k)
          result[k] = (typename channel_type<value_type>::type)(255);
      }
      return result;
    }
#endif // PNG_OUTPUT

    void save(const std::string& filename) {
#ifdef PNG_OUTPUT
      if (rows.size() == 0 || monomials.size() == 0) return;

      typedef virtual_2d_locator<PolynomialMatrix, false> locator_t;
      typedef image_view<locator_t> my_virt_view_t;

      auto png_filename = filename + ".png";
      I("saving " + png_filename + "...");
      point_t dims(monomials.size(), rows.size());

      my_virt_view_t view(dims, locator_t(point_t(0, 0), point_t(1, 1), *this));
      png_write_view(png_filename.c_str(), view);
#endif // PNG_OUTPUT
    }

    std::vector<row> rows;
    std::set<M, std::greater<M> > monomials;
  };

  MMSet eliminate2(const MMSet& HH) {
    PolynomialMatrix m(HH);

    std::cerr << "[" << m.rows.size() << "x" << m.monomials.size() << "]" << std::flush;

    std::vector<M> pivot_columns;
    std::vector<typename std::vector<row>::iterator> pivot_rows;
    for (const auto& monomial : m.monomials) {
      for (auto it = m.rows.begin(); it != m.rows.end(); ++it) {
        if (it->f().lm() == monomial) {
          pivot_columns.push_back(monomial);
          pivot_rows.push_back(it);
          break;
        }
      }
    }

    I(m);
    static uint step;

    m.save("matrix" + to_string(step++, 3));

    auto end = m.rows.end();

    auto size = pivot_rows.size();
    for (uint i = 0; i < size; ++i) {
      if (i > 0 && i % 1000 == 0) {
        std::cerr << (100*(double)i/size) << "%" << std::flush;
      }
      D("reducing column " << pivot_columns[i]);

      const P& f = pivot_rows[i]->f();
      const C fc = f.lc();

      auto j = pivot_rows[i];
      for (++j; j != end; ++j) {
        const P& g = j->f();
        if (g.lm() != pivot_columns[i]) continue;
        C gc = g.lc();
        gc *= -1;

        D("reducing " << g << " with " << f);
        std::cerr << "." << std::flush;
        j->uf.combineAndRenormalize(fc, pivot_rows[i]->uf, gc);
        auto colbegin = pivot_columns.begin();
        auto colend = pivot_columns.end();
        auto lmf = j->f().lm();
        auto it = std::lower_bound(colbegin, colend, lmf, std::greater<M>());
        if (it == colend) {
          pivot_rows.push_back(j);
          pivot_columns.push_back(lmf);
          continue;
        }
        if (it != colend && *it == lmf && pivot_rows[it - colbegin] > j) {
          pivot_rows[it - colbegin] = j;
        }
      }
      D("m = " << m);
      m.save("matrix" + to_string(step, 3) + "-" + to_string(i, 5));
    }

    MMSet PP;
    for (const auto& row : m.rows) {
      if (!row.f().isZero())
        PP.insert(row.uf);
    }
    DD("returning PP = ", PP);
    return PP;
  }

  MMSet eliminate(const MMSet& HH) {
    PolynomialMatrix m(HH);

    std::cerr << "[" << m.rows.size() << "x" << m.monomials.size() << "]" << std::flush;

    I(m);
    static uint step;

    m.save("matrix" + to_string(step++, 3));

    auto begin = m.rows.begin();
    auto end = m.rows.end();

    auto size = m.monomials.size();
    uint k = 0;
    for (const auto& monomial : m.monomials) {
      if (k > 0 && k % 1000 == 0) {
        std::cerr << (100*(double)k/size) << "%" << std::flush;
      }
      ++k;
      D("reducing column " << monomial);

      auto i = begin;

      while (i != end && (i->done || i->f().lm() != monomial)) ++i;
      if (i == end) continue;

      const P& f = i->f();
      const C fc = f.lc();

      auto j = i;
      for (++j; j != end; ++j) {
        if (j->done) continue;
        const P& g = j->f();
        if (g.lm() != monomial) continue;
        C gc = g.lc();
        gc *= -1;

        D("reducing " << g << " with " << f);
        std::cerr << "." << std::flush;
        j->uf.combineAndRenormalize(fc, i->uf, gc);
      }
      i->done = true;
      D("m = " << m);
      m.save("matrix" + to_string(step, 3) + "-" + to_string(k, 5));
    }

    MMSet PP;
    for (const auto& row : m.rows) {
      if (!row.f().isZero())
        PP.insert(row.uf);
    }
    DD("returning PP = ", PP);
    return PP;
  }

  void update(const MMSet& PP, LMSet& GG) {
    for (const auto& wh : PP) {
      if (wh.f().isZero()) continue;
      auto m = wh.f().lm();
      D("looking for lm(h) = " << m);
      auto mvg = GG.find(m);
      if (mvg != GG.end()) {
        auto v = mvg->second.u();
        auto w = wh.u();
        auto g = mvg->second.f();
        if (((m / g.lm()) * v) > w) {
          D("replacing " << mvg->second << " by " << wh);
          GG[m] = wh;
        }
      } else {
        D("not found, adding (" << m << ", " << wh << ") to GG");
        GG[m] = wh;
      }
    }
    DD("returning, GG = ", GG);
    D("GG has " << GG.size() << " elements in " << GG.bucket_count() << " buckets");
  }

  bool isPrimitive(const std::pair<M, MMP >& lm) const {
    return lm.first == lm.second.f().lm();
  }

  std::vector<P> moGVW(std::vector<P>& input) {
    interreduce(input);

    M m;
    uint max_var = 0;
    for (const auto& p : input) {
      for (const auto& t : p) {
        m |= t.m();
      }
    }
    for (uint i = 0; i < M::VAR_COUNT; ++i) {
      if (m[i] > 0) max_var = i;
    }

    LMSet GG;
    wasLifted.clear();
    for (typename std::vector<P>::size_type i = 0; i < input.size(); ++i) {
      GG[input[i].lm()] = MMP(S::e(i), input[i]);
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
    std::cerr << "[" << mindeg << ":" << liftdeg << "]" << std::flush;

    while (mindeg <= liftdeg) {
      LMSet todo;
      for (auto it = GG.begin(); it != GG.end(); ++it) {
        if (it->first.degree() == mindeg && !wasLifted[it->first]) {
          todo[it->first] = it->second;
        }
      }
      I("calling lift");
      MMSet HH = lift(todo, GG, max_var);
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
      std::cerr << "[" << mindeg << ":" << liftdeg << "]" << std::flush;
    }

    std::vector<P> result;
    for (const auto& p : GG) {
      if (isPrimitive(p)) {
        result.push_back(p.second.f());
      }
    }
    std::sort(result.begin(), result.end());
    I("calling interreduce");
    interreduce(result);
    std::sort(result.begin(), result.end());
    II("returning gb = ", result);
    return result;
  }
private:
  std::unordered_map<M, bool> wasLifted;
};

template<class A, class B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B>& ab) {
  return out << "(" << ab.first << ", " << ab.second << ")";
}

#endif // MOGVW_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
