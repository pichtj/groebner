#include <iostream>

#include "FGb.h"
#include "Ideal.h"

int main(int argc, char* argv[]) {
  get_var_name = var_name;

  typedef Polynomial<Term<int, Monomial<char, 59, degrevlex> > > P;
  auto input = read_input<P>(std::cin);
  FGbRunner<typename P::MonomialType::ExponentType, P::MonomialType::VAR_COUNT> runner;
  auto output = runner.fgb(input);
  for (auto it = output.begin(); it != output.end(); ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
