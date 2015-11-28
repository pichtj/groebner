#include <iostream>
#include <fstream>

#include <flint/fmpzxx.h>

#include "GbRunner.h"
#include "Ideal.h"

int main(int argc, char* argv[]) {
  get_var_name = var_name;
  max_print_length = std::numeric_limits<uint>::max();

  typedef Polynomial<Term<flint::fmpzxx, Monomial<char, 64, degrevlex> > > P;

  std::vector<P> input;
  if (argc > 1) {
    D("reading input from file " << argv[1]);
    std::ifstream in_file(argv[1]);
    input = read_input<P>(in_file);
  } else {
    D("reading input from stdin");
    input = read_input<P>(std::cin);
  }
  DD("input = ", input);
  GbRunner::interreduce(input);
  std::sort(input.begin(), input.end());
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
