#include <algorithm>

template<class T>
T gcd(T m, T n) {
  if (m == 0) return abs(n);
  m = abs(m);
  n = abs(n);
  while (m != n) {
    if (m < n) {
      std::swap(m, n);
    }
    m -= n;
  }
  return abs(m);
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
