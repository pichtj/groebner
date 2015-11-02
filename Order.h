#ifndef ORDER_H
#define ORDER_H

#include <stdexcept>
#include <iostream>

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
      if (lhs[M::VAR_COUNT - i - 1] != rhs[M::VAR_COUNT - i - 1]) {
        return lhs[M::VAR_COUNT - i - 1] > rhs[M::VAR_COUNT - i - 1];
      }
    }
    return false;
  }
};

#endif // ORDER_H
