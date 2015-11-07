#include <iostream>

#include <boost/algorithm/string.hpp>

#include "moGVW.h"
#include "CachedMonomial.h"
#include "Ideal.h"

int main(int argc, char* argv[]) {
  get_var_name = var_name;

  typedef Polynomial<Term<int, Monomial<char, 3, lex> > > P;
  auto input = read_input<P>(std::cin);
  moGVWRunner<P> runner;
  auto output = runner.moGVW(input);
  for (auto it = output.begin(); it != output.end(); ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
