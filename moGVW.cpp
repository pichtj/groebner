#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include "integral.h"
#include "moGVW.h"
#include "CachedMonomial.h"
#include "Ideal.h"

int main(int argc, char* argv[]) {
  get_var_name = var_name;

  typedef Polynomial<Term<flint::fmpzxx, Monomial<char, 64, degrevlex> > > P;

  std::vector<P> input;
  if (argc > 1) {
    std::ifstream in_file(argv[1]);
    input = read_input<P>(in_file);
  } else {
    input = read_input<P>(std::cin);
  }
  moGVWRunner<P> runner;
  auto output = runner.moGVW(input);
  for (auto it = output.begin(); it != output.end(); ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
