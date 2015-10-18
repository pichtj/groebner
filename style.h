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

#endif // STYLE_H
