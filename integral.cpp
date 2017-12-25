#include "integral.h"

#include <istream>

std::istream& operator>>(std::istream& in, flint::fmpzxx& i) {
  auto next = in.peek();
  std::string s;
  while (!in.eof() && (std::isdigit(next) || next == '+' || next == '-')) {
    s += in.get();
    next = in.peek();
  }
  i = flint::fmpzxx(s.c_str());
  return in;
}

std::istream& operator>>(std::istream& in, mpz_class& i) {
  auto next = in.peek();
  std::string s;
  while (!in.eof() && (std::isdigit(next) || next == '+' || next == '-')) {
    s += in.get();
    next = in.peek();
  }
  i = mpz_class(s.c_str());
  return in;
}

std::ostream& operator<<(std::ostream& out, const mpz_class& i) {
  char* s = mpz_get_str(0, 10, i.get_mpz_t());
  out << s;
  free(s);
  return out;
}

// vim:ruler:cindent:shiftwidth=2:expandtab: