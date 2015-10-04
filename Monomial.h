#ifndef MONOMIAL_H
#define MONOMIAL_H

#include "style.h"
#include "operators.h"

#include <functional>
#include <ostream>

#define VAR_COUNT 2

template<class E = char>
class Monomial {
public:
  typedef E ExponentType;
  typedef Monomial<E> This;
  Monomial() : mon() {}
  E& operator[](const uint& index) { return mon[index]; }
  const E& operator[](const uint& index) const { return mon[index]; }

  uint degree() const {
    uint result = 0;
    uint i;
    for (i = 0; i < VAR_COUNT; ++i) {
      result += mon[i];
    }
    return result;
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

  This operator/(const This& other) const {
    This result;
    for (uint i = 0; i < VAR_COUNT; ++i) {
      //assert(a.mon[i] < b.mon[i]);
      result.mon[i] = mon[i] - other.mon[i];
    }
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
    // lex order for simplicity
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] != other[i]) {
        return mon[i] < other[i];
      }
    }
    return false;
  }

  bool operator==(const This& other) const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (mon[i] != other[i]) {
        return false;
      }
    }
    return true;
  }
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
  template<typename E>
  struct hash {
      size_t operator()(const Monomial<typename E::ExponentType>& e) const {
      size_t result = 0;
      for (uint i = 0; i < VAR_COUNT; ++i) {
        result *= 7;
        result += e[i];
      }
      return result;
    }
  };
}

template<class E>
Monomial<E> pow(const Monomial<E>& m, uint e) {
  Monomial<E> result;
  for (uint i = 0; i < VAR_COUNT; ++i) {
    result[i] = m[i] * e;
  }
  return result;
}

template<class E>
std::ostream& operator<<(std::ostream& out, const Monomial<E>& mon) {
  out << "{";
  for (uint i = 0; i < VAR_COUNT; ++i) {
    out << mon[i];
    if (i < VAR_COUNT - 1) out << " ";
  }
  out << "}";
  return out;
}

template<>
inline std::ostream& operator<<(std::ostream& out, const Monomial<char>& mon) {
  out << "{";
  for (uint i = 0; i < VAR_COUNT; ++i) {
    out << (int)mon[i];
    if (i < VAR_COUNT - 1) out << " ";
  }
  out << "}";
  return out;
}

#endif // MONOMIAL_H
