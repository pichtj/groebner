#ifndef EXPONENT_H
#define EXPONENT_H

#include "style.h"
#include "operators.h"

#include <functional>
#include <ostream>

#define VAR_COUNT 2

template<class E = char>
class Exponent {
public:
  typedef E ExponentType;
  Exponent() : exp() {}
  E& operator[](const uint& index) { return exp[index]; }
  const E& operator[](const uint& index) const { return exp[index]; }

  uint degree() const {
    uint result = 0;
    uint i;
    for (i = 0; i < VAR_COUNT; ++i) {
      result += exp[i];
    }
    return result;
  }

  Exponent<E>& operator*=(const Exponent<E>& other) {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      exp[i] += other.exp[i];
    }
    return *this;
  }

  Exponent<E> operator*(const Exponent<E>& other) const {
    Exponent<E> result;
    for (uint i = 0; i < VAR_COUNT; ++i) {
      result.exp[i] = exp[i] + other.exp[i];
    }
    return result;
  }

  Exponent<E> operator/(const Exponent<E>& other) const {
    Exponent<E> result;
    for (uint i = 0; i < VAR_COUNT; ++i) {
      //assert(a.exp[i] < b.exp[i]);
      result.exp[i] = exp[i] - other.exp[i];
    }
    return result;
  }

  bool operator<(const Exponent<E>& other) const {
    // lex order for simplicity
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != other[i]) {
        return exp[i] < other[i];
      }
    }
    return false;
  }

  bool operator==(const Exponent<E>& other) const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != other[i]) {
        return false;
      }
    }
    return true;
  }
  bool isZero() const {
    for (uint i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != 0) {
        return false;
      }
    }
    return true;
  }

  static Exponent<E> x(uint i) {
    Exponent<E> e;
    e[i] = 1;
    return e;
  }
private:
  E exp[VAR_COUNT];
};

namespace std {
  template<typename E>
  struct hash {
      size_t operator()(const Exponent<typename E::ExponentType>& e) const {
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
std::ostream& operator<<(std::ostream& out, const Exponent<E>& exp) {
  out << "{";
  for (uint i = 0; i < VAR_COUNT; ++i) {
    out << exp[i];
    if (i < VAR_COUNT - 1) out << " ";
  }
  out << "}";
  return out;
}

template<>
inline std::ostream& operator<<(std::ostream& out, const Exponent<char>& exp) {
  out << "{";
  for (uint i = 0; i < VAR_COUNT; ++i) {
    out << (int)exp[i];
    if (i < VAR_COUNT - 1) out << " ";
  }
  out << "}";
  return out;
}

#endif // EXPONENT_H
