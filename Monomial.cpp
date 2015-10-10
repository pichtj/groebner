#include <sstream>

#include "Monomial.h"

std::string get_var_name_impl(uint index) {
  std::stringstream s;
  s << "x" << index;
  return s.str();
}

std::string (*get_var_name)(uint) = get_var_name_impl;
std::string (*default_get_var_name)(uint) = get_var_name_impl;
