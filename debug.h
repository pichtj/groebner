#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include "style.h"

#define W(statement) std::cerr << __func__ << "[" << __LINE__ << "]: " << statement << std::endl;
#define WW(statement, container) std::cerr << __func__ << "[" << __LINE__ << "]: " << statement; print(std::string(__func__) + "[" + to_string(__LINE__) + "]: ", container);

#if (defined DEBUG || defined INFO)
#define I(statement) W(statement)
#define II(statement, container) WW(statement, container)
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
void print(const std::string& prefix, const C& container) {
  std::cerr << "{";
  bool itemPrinted = false;
  for (const auto& item : container) {
    if (itemPrinted) std::cerr << ",";
    std::cerr << std::endl << prefix << "  " << item;
    itemPrinted = true;
  }
  if (itemPrinted) std::cerr << std::endl << prefix;
  std::cerr << "}" << std::endl;
}

#endif // DEBUG_H
