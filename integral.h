#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <algorithm>

#include <mpirxx.h>
#include <flint/fmpz.h>
#include <flint/fmpzxx.h>

template<class T>
T gcd(T m, T n) {
  if (m < 0) return gcd(-m, n);
  if (n < 0) return gcd(m, -n);
  if (m == 0) return n;
  while (m != n) {
    if (m < n) {
      std::swap(m, n);
    }
    m -= n;
  }
  return m;
}

template<>
inline long gcd(long m, long n) {
  if (m == 0 || n == 0) return 0;
  while (1) {
    m = m % n;
    if (m == 0) return n;
    n = n % m;
    if (n == 0) return m;
  }
}

template<>
inline mpz_class gcd(mpz_class m, mpz_class n) {
  mpz_class result;
  mpz_gcd(result.get_mpz_t(), m.get_mpz_t(), n.get_mpz_t());
  return result;
}

template<>
inline flint::fmpzxx gcd(flint::fmpzxx m, flint::fmpzxx n) {
  flint::fmpzxx result;
  fmpz_gcd(result._fmpz(), m._fmpz(), n._fmpz());
  return result;
}

template<class C>
typename C::value_type gcd(const C& c) {
  auto it = c.begin();
  auto end = c.end();
  if (it == end) return typename C::value_type();
  auto result = *it;
  for (++it; it != end; ++it) {
    result = gcd(result, *it);
  }
  return result;
}

template<class C>
size_t log2(const C& c) {
  size_t result = 0;
  auto d = c - 1;
  while (d > 0) {
    d /= 2;
    ++result;
  }
  return result;
}

template<>
inline size_t log2(const mpz_class& c) {
  if (c == 0 || c == 1) return 0;
  mpz_class d = c - 1;
  return mpz_sizeinbase(d.get_mpz_t(), 2);
}

template<>
inline size_t log2(const flint::fmpzxx& c) {
  if (c == 0 || c == 1) return 0;
  flint::fmpzxx d = c;
  d -= flint::fmpzxx(1);
  return fmpz_sizeinbase(d._fmpz(), 2);
}

inline std::istream& operator>>(std::istream& in, flint::fmpzxx& i) {
  auto next = in.peek();
  std::string s;
  while (!in.eof() && (std::isdigit(next) || next == '+' || next == '-')) {
    s += in.get();
    next = in.peek();
  }
  i = flint::fmpzxx(s.c_str());
  return in;
}

#endif // INTEGRAL_H
