#ifndef IDEAL_H
#define IDEAL_H

#include <iostream>
#include <vector>

#include "debug.h"

void read_var_names(std::istream& in);
std::string var_name(uint index);

template<class P>
std::vector<P> read_ideal(std::istream& in) {
  std::vector<P> result;
  auto next = in.peek();
  while (!in.eof()) {
    if (next == ',' || next == ' ' || next == '\n') {
      in.get();
      next = in.peek();
      continue;
    }
    P p;
    in >> p;
    result.push_back(p);
    next = in.peek();
  }
  DD("read ideal = ", result);
  return result;
}

template<class P>
std::vector<P> read_input(std::istream& in) {
  read_var_names(in);
  return read_ideal<P>(in);
}

#endif // IDEAL_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
