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

vector<string> var_names = {
  "df",
  "dx",
  "rA",
  "iA",
  "rBp",
  "iBp",
  "rC",
  "iC",
  "rmut",
  "imut",
  "rmutmut",
  "imutmut",
  "rnutnut",
  "inutnut",
  "rmutnut",
  "imutnut",
  "rnut",
  "inut",
  "fftt",
  "xxtt",
  "rky1",
  "iky1",
  "rky2",
  "iky2",
  "rk1y",
  "ik1y",
  "rk11",
  "ik11",
  "rk12",
  "ik12",
  "rk2y",
  "ik2y",
  "rk21",
  "ik21",
  "rk22",
  "ik22",
  "rkyy",
  "ikyy",
  "poly1cs",
  "v1",
  "w1",
  "v5",
  "w5",
  "v9",
  "w9",
  "v2",
  "w2",
  "v8",
  "w8",
  "v6",
  "w6",
  "v7",
  "w7",
  "v3",
  "w3",
  "v4",
  "w4",
  "y",
  "i"
};

string var_name(uint index) {
  if (index < var_names.size()) return var_names[index];
  return default_get_var_name(index);
}

int main(int argc, char* argv[]) {
  get_var_name = var_name;

  typedef Polynomial<Term<fmpzxx, Monomial<char, 64, lex> > > P;
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
