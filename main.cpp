#include <iostream>

#include <boost/algorithm/string.hpp>

#include "moGVW.h"
#include "CachedMonomial.h"

using namespace std;
using namespace flint;
using namespace boost;

vector<string> var_names;

void read_var_names() {
  string line;
  getline(cin, line);
  split(var_names, line, is_any_of(","));
}

template<class P>
set<P> read_ideal() {
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

template<class P>
set<P> read_input() {
  read_var_names();
  return read_ideal<P>();
}

string var_name(uint index) {
  if (index < var_names.size()) return var_names[index];
  return default_get_var_name(index);
}

int main(int argc, char* argv[]) {
  get_var_name = var_name;

  typedef Polynomial<Term<fmpzxx, CachedMonomial<Monomial<char, 64, lex> > > > P;
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
