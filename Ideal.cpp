#include <boost/algorithm/string.hpp>

#include "Ideal.h"
#include "Monomial.h"

std::vector<std::string> var_names;

void read_var_names(std::istream& in) {
  std::string line;
  getline(in, line);
  boost::split(var_names, line, boost::is_any_of(","));
}

std::string var_name(uint index) {
  if (index < var_names.size()) return var_names[index];
  return default_get_var_name(index);
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
