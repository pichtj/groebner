#ifndef ORDER_H
#define ORDER_H

#include <stdexcept>

struct lex {
  template<class M>
  bool operator()(const M& lhs, const M& rhs) const {
    for (uint i = 0; i < M::VAR_COUNT; ++i) {
      if (lhs[i] != rhs[i]) {
        return lhs[i] < rhs[i];
      }
    }
    return false;
  }
};

struct degrevlex {
  template<class M>
  bool operator()(const M& lhs, const M& rhs) const {
    auto ld = lhs.degree();
    auto rd = rhs.degree();
    if (ld != rd) return ld < rd;
    for (uint i = 0; i < M::VAR_COUNT; ++i) {
      if (lhs[M::VC - i - 1] != rhs[M::VC - i -1]) {
        return lhs[M::VC - i - 1] > rhs[M::VC - i - 1];
      }
    }
    throw std::logic_error("this should never happen");
  }
};

#endif // ORDER_H
