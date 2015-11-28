#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <ctime>
#include "style.h"

//#define PRINT_LINE_NUMBERS

#ifdef PRINT_LINE_NUMBERS
#define W(statement) std::cerr << now << __func__ << "[" << __LINE__ << "]: " << statement << std::endl;
#define WW(statement, container) std::cerr << now << __func__ << "[" << __LINE__ << "]: " << statement; print(std::string(__func__) + "[" + to_string(__LINE__) + "]: ", container);
#else
#define W(statement) std::cerr << now << __func__ << ": " << statement << std::endl;
#define WW(statement, container) std::cerr << now << __func__ << ": " << statement; print(std::string(__func__) + ": ", container);
#endif

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
  uint itemsPrinted = 0;
  for (const auto& item : container) {
    if (itemsPrinted) std::cerr << ",";
    std::cerr << std::endl << prefix << "  " << item;
    ++itemsPrinted;
    if (itemsPrinted > 50) {
      std::cerr << std::endl << prefix << "  " << "...";
      break;
    }
  }
  if (itemsPrinted) std::cerr << std::endl << prefix;
  std::cerr << "}" << std::endl;
}

std::ostream& now(std::ostream& out);

#endif // DEBUG_H
// vim:ruler:cindent:shiftwidth=2:expandtab:
