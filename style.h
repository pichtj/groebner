#ifndef STYLE_H
#define STYLE_H

#include <string>
#include <sstream>

typedef unsigned int uint;
typedef unsigned long ulong;

template<class T>
std::string to_string(const T& t) {
  std::stringstream s;
  s << t;
  return s.str();
}

template<class T>
T from_string(const std::string& s) {
  T t;
  std::stringstream ss(s);
  ss >> t;
  return t;
}

#endif // STYLE_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
