#include <gmp.h>

#define VAR_COUNT 75
#define INPUT_COUNT 33
#define COEFFICIENT_TYPE long
#define EXPONENT_TYPE unsigned short

#define USE_GMP 0

typedef COEFFICIENT_TYPE coeff_t;

typedef struct exp {
  EXPONENT_TYPE exp[VAR_COUNT];
} exp_t;

inline unsigned int degree(exp_t exp) {
  unsigned int result = 0;
  unsigned int i;
  for (i = 0; i < VAR_COUNT; ++i) {
    result += exp.exp[i];
  }
  return result;
}

inline exp_t mul(exp_t a, exp_t b) {
  exp_t result;
  unsigned int i;
  for (i = 0; i < VAR_COUNT; ++i) {
    result.exp[i] = a.exp[i] + b.exp[i];
  }
  return result;
};

inline exp_t div(exp_t a, exp_t b) {
  exp_t result;
  unsigned int i;
  for (i = 0; i < VAR_COUNT; ++i) {
    //assert(a.exp[i] < b.exp[i]);
    result.exp[i] = a.exp[i] - b.exp[i];
  }
  return result;
};

typedef struct term {
  coeff_t coeff;
  exp_t exp;
} term_t;

int compare(term_t* a, term_t* b);
inline unsigned int degree(term_t term) { return degree(term.exp); }

typedef struct poly poly_t;
struct poly {
  poly_t* next;
  term_t term;
};

inline term_t lterm(poly_t* p) { return p->term; }
inline coeff_t lc(poly_t* p) { return p->term.coeff; }
inline exp_t lm(poly_t* p) { return p->term.exp; }
inline exp_t lm(poly_t p) { return p.term.exp; }

void add(poly_t* result, poly_t* a, poly_t* b);
void add_mul_c(poly_t* result, poly_t* a, coeff_t ca, poly_t* b, coeff_t cb);
void mul_c(poly_t* result, coeff_t c);
void mul(poly_t* result, poly_t* a, poly_t* b);
