#ifndef MONOMIAL_H
#define MONOMIAL_H

#include "style.h"
#include "Order.h"

#include <functional>
#include <ostream>
#include <string>

extern std::string (*get_var_name)(uint);
extern std::string (*default_get_var_name)(uint);

template<class E = char, uint VC = 5, class O = lex>
class Monomial {
public:
  typedef E ExponentType;
  typedef Monomial<E, VC, O> This;
  static const uint VAR_COUNT = VC;

  Monomial() : mon() {}
  E& operator[](uint index) { return mon[index]; }
  E operator[](uint index) const { return mon[index]; }

  uint degree() const {
    uint result = 0;
    uint i;
    for (i = 0; i < VAR_COUNT; ++i) {
      result += mon[i];
    }
    return result;
  }

  bool isConstant() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] > 0) {
        return false;
      }
    }
    return true;
  }

  This& operator*=(const This& other) {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      mon[i] += other.mon[i];
    }
    return *this;
  }

  This operator*(const This& other) const {
    This result;
    for (uint i = 0; i < VAR_COUNT; ++i) {
      result.mon[i] = mon[i] + other.mon[i];
    }
    return result;
  }

  This& operator/=(const This& other) {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      mon[i] = mon[i] - other.mon[i];
    }
    return *this;
  }

  This operator/(const This& other) const {
    This result = *this;;
    result /= other;
    return result;
  }

  bool divides(const This& other) const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] > other.mon[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator<(const This& other) const {
    return O()(*this, other);
  }
  bool operator>(const This& other) const { return other < *this; }

  bool operator==(const This& other) const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] != other[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const This& other) const { return !(operator==(other)); }
  bool isZero() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] != 0) {
        return false;
      }
    }
    return true;
  }

  static This x(uint i) {
    This e;
    e[i] = 1;
    return e;
  }
private:
  E mon[VAR_COUNT];
};

namespace std {
  template<typename E, uint VC, class O>
  struct hash<Monomial<E, VC, O> > {
    size_t operator()(const Monomial<E, VC, O>& e) const {
      size_t result = 0;
      for (uint i = 0; i < Monomial<E, VC, O>::VAR_COUNT; ++i) {
        result *= 2147483647;
        result += e[i];
      }
      return result;
    }
  };
}

template<class E, uint VC, class O>
Monomial<E, VC, O> pow(const Monomial<E, VC, O>& m, uint e) {
  Monomial<E, VC, O> result;
  for (uint i = 0; i < Monomial<E, VC, O>::VAR_COUNT; ++i) {
    result[i] = m[i] * e;
  }
  return result;
}

template<class E, uint VC, class O>
Monomial<E, VC, O> lcm(const Monomial<E, VC, O>& a, const Monomial<E, VC, O>& b) {
  Monomial<E, VC, O> result;
  for (uint i = 0; i < Monomial<E, VC, O>::VAR_COUNT; ++i) {
    result[i] = std::max(a[i], b[i]);
  }
  return result;
}

template<class E, uint VC, class O>
std::ostream& operator<<(std::ostream& out, const Monomial<E, VC, O>& mon) {
  out << "{";
  for (uint i = 0; i < Monomial<E, VC, O>::VAR_COUNT; ++i) {
    out << mon[i];
    if (i < Monomial<E, VC, O>::VAR_COUNT - 1) out << " ";
  }
  out << "}";
  return out;
}

template<uint VC, class O>
inline std::ostream& operator<<(std::ostream& out, const Monomial<char, VC, O>& mon) {
  bool termPrinted = false;
  for (uint i = 0; i < Monomial<char, VC, O>::VAR_COUNT; ++i) {
    int e = (int)mon[i];
    if (e) {
      if (termPrinted) out << "*";
      out << (*get_var_name)(i);
      if (e > 1) out << "^" << e;
      termPrinted = true;
    }
  }
  if (!termPrinted) out << "1";
  return out;
}

struct use_abc_var_names {
  use_abc_var_names();
  ~use_abc_var_names();
};

#endif // MONOMIAL_H
