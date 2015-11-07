#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include "style.h"

#if (defined DEBUG || defined INFO)
#define I(statement) std::cerr << __func__ << "[" << __LINE__ << "]: " << statement << std::endl;
#define II(statement, container) std::cerr << __func__ << "[" << __LINE__ << "]: " << statement; print(std::string(__func__) + "[" + to_string(__LINE__) + "]: ", container);
#else
#define I(statement) ;
#define II(statement, container) ;
#endif // (defined DEBUG || defined INFO)

#ifdef DEBUG
#define D(statement) I(statement)
#define DD(statement, container) II(statement, container)
#else
#define D(statement) ;
#define DD(statement, container) ;
#endif // DEBUG

template<class C>
void print(const std::string& prefix, const C& c) {
  std::cerr << "{";
  bool itemPrinted = false;
  for (const auto& item : c) {
    if (itemPrinted) std::cerr << ",";
    std::cerr << std::endl << prefix << "  " << item;
    itemPrinted = true;
  }
  if (itemPrinted) std::cerr << std::endl << prefix;
  std::cerr << "}" << std::endl;
}

#endif // DEBUG_H
