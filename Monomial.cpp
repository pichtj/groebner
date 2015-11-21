#include <sstream>

#include "Monomial.h"

std::string get_var_name_impl(uint index) {
  std::stringstream s;
  s << "x" << index;
  return s.str();
}

std::string (*get_var_name)(uint) = get_var_name_impl;
std::string (*default_get_var_name)(uint) = get_var_name_impl;

std::string abc_var_names(uint index) {
  const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
  return std::string(alphabet + index, alphabet + index + 1);
}

use_abc_var_names::use_abc_var_names() {
  get_var_name = abc_var_names;
}

use_abc_var_names::~use_abc_var_names() {
  get_var_name = default_get_var_name;
}
// vim:ruler:cindent:shiftwidth=2:expandtab:
