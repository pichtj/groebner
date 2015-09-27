#include "Term.h"

template<class C, class E> class Polynomial;
#include "print.h"

template<class C = int, class E = char>
class Polynomial {
public:
  Polynomial() : pd(0) {}
  Polynomial(const C& c) : pd(new PolynomialData(Term<C, E>(c))) {}
  ~Polynomial() {
    deleteAll();
  }
  Term<C, E> lterm() const { return pd->term; }
  C lc() const { if (pd) { return pd->term.coeff; } else { return C(); } }
  Exponent<E> lm() const { if (pd) { return pd->term.exp; } else { return Exponent<E>(); } }
  Polynomial<C, E>& operator+=(const Term<C, E>& t) {
    if (pd == 0 || pd->term.exp < t.exp) {
      PolynomialData* new_pd = new PolynomialData(t);
      new_pd->next = pd;
      pd = new_pd;
      return *this;
    }

    PolynomialData* current = pd;
    PolynomialData* before = 0;

    while (current->term.exp > t.exp && current->next) {
      before = current;
      current = current->next;
    }

    if (current->term.exp == t.exp) {
      current->term.coeff += t.coeff;
      if (current->term.coeff == C()) {
        if (before) {
          before->next = current->next;
          delete current;
        } else {
          PolynomialData* old_pd = pd;
          pd = pd->next;
          delete old_pd;
        }
      }
    } else {
      PolynomialData* after = current->next;
      current->next = new PolynomialData(t);
      current->next->next = after;
    }
    
    return *this;
  }
  Polynomial<C, E> operator+(const Polynomial<C, E>& other) const;
  Polynomial<C, E>& operator+=(const Polynomial<C, E>& other);
  Polynomial<C, E>& operator*=(const C& c) {
    if (c == C()) {
      deleteAll();
      return *this;
    }
    PolynomialData* current = pd;
    while (current) {
      current->term.coeff *= c;
      current = current->next;
    }
    return *this;
  }
  Polynomial<C, E>& operator*=(const Term<C, E>& t) {
    PolynomialData* current = pd;
    while (current) {
      current->term *= t;
      current = current->next;
    }
    return *this;
  }
  Polynomial<C, E> operator*(const Term<C, E>& t) const {
    Polynomial<C, E> result;
    result += *this;
    result *= t;
    return result;
  }
  Polynomial<C, E>& operator*=(const Polynomial<C, E>& other) {
    Polynomial<C, E> newMe = *this * other;
    deleteAll();
    *this += newMe;
  }
  Polynomial<C, E> operator*(const Polynomial<C, E>& other) const {
    Polynomial<C, E> result;
    PolynomialData* current = other.pd;
    while (current) {
      Polynomial<C, E> p = *this * current->term;
      result += p;
      current = current->next;
    }
    return result;
  }
  bool operator==(const Polynomial<C, E>& other) const;
  void doPrint() const { if (pd) pd->doPrint(); else printf("0"); }
private:
  void deleteAll() {
    PolynomialData* last = pd;
    while (last) {
      PolynomialData* p = last->next;
      delete last;
      last = p;
    }
    pd = 0;
  }
  struct PolynomialData {
    PolynomialData(const Term<C, E>& t) : next(0), term(t) {}
    PolynomialData* next;
    Term<C, E> term;
    void doPrint() {
      print(term);
      if (next) {
        printf("+");
        next->doPrint();
      }
    }
  };

  Polynomial(PolynomialData* polynomialData) : pd(polynomialData) {}
  PolynomialData* pd;
};

template<class C, class E>
Polynomial<C, E>& Polynomial<C, E>::operator+=(const Polynomial<C, E>& other) {
  const PolynomialData* b = other.pd;
  while (b) {
    operator+=(b->term);
    b = b->next;
  }
  return *this;
}

template<class C, class E>
Polynomial<C, E> Polynomial<C, E>::operator+(const Polynomial<C, E>& other) const {
  Polynomial<C, E> result = Polynomial<C, E>();
  result += *this;
  result += other;
  return result;
}
