#include <iostream>

#include "moGVW.h"

using namespace std;
using namespace flint;

template<class P>
set<P> read_input() {
  set<P> result;
  auto next = cin.peek();
  while (!cin.eof()) {
    D("next = " << next);
    if (next == ',' || next == ' ' || next == '\n') {
      cin.get();
      next = cin.peek();
      continue;
    }
    P p;
    cin >> p;
    result.insert(p);
    next = cin.peek();
  }
  return result;
}

int main(int argc, char* argv[]) {
  use_abc_var_names in_this_scope;

  typedef Polynomial<Term<fmpzxx, Monomial<char, 8, lex> > > P;
  auto input = read_input<P>();
  cout << "Input:" << endl;
  for (auto it = input.begin(); it != input.end(); ++it) {
    cout << *it << endl;
  }
  moGVWRunner<P> runner;
  auto output = runner.moGVW(input);
  cout << "Output:" << endl;
  for (auto it = output.begin(); it != output.end(); ++it) {
    cout << *it << endl;
  }
  return 0;
}
