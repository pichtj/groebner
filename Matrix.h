#ifndef MATRIX_H
#define MATRIX_H

#include "debug.h"
#include "Polynomial.h"

template<uint ROWS, uint COLS, class P = Polynomial<Term<int, Monomial<char> > > >
struct CoefficientMatrix {
  typedef typename P::MonomialType M;
  typedef typename P::TermType T;
  typedef typename P::CoefficientType C;
};

template<uint ROWS, uint COLS, class P = Polynomial<Term<int, Monomial<char> > > >
struct UpperTriangularSparse : public CoefficientMatrix<ROWS, COLS, P> {
};

template<uint ROWS, uint COLS, class P = Polynomial<Term<int, Monomial<char> > > >
struct Dense : public CoefficientMatrix<ROWS, COLS, P> {
};

#endif // MATRIX_H
