#define VAR_COUNT 75

template<class E>
class Exponent {
public:
  inline Exponent() : exp() {}
  inline E& operator[](const unsigned int& index) { return exp[index]; }
  inline const E& operator[](const unsigned int& index) const { return exp[index]; }

  inline unsigned int degree() {
    unsigned int result = 0;
    unsigned int i;
    for (i = 0; i < VAR_COUNT; ++i) {
      result += exp.exp[i];
    }
    return result;
  }

  inline Exponent<E> operator*(const Exponent<E>& other) const {
    Exponent<E> result;
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      result.exp[i] = exp[i] + other.exp[i];
    }
    return result;
  }

  inline Exponent<E> operator/(const Exponent<E>& other) const {
    Exponent<E> result;
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      //assert(a.exp[i] < b.exp[i]);
      result.exp[i] = exp[i] - other.exp[i];
    }
    return result;
  }

  bool operator<(const Exponent<E>& other) const;
  inline bool operator==(const Exponent<E>& other) const {
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != other[i]) {
        return false;
      }
      return true;
    }
  }
  inline bool operator!=(const Exponent<E>& other) const { return !(*this == other); }
  inline bool isZero() const {
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != 0) {
        return false;
      }
      return true;
    }
  }
private:
  E exp[VAR_COUNT];
};

template<class C, class E>
class Term {
public:
  C coeff;
  Exponent<E> exp;
  inline unsigned int degree() { return exp.degree(); }
};

template<class C, class E>
class Polynomial {
public:
  inline Term<C, E> lterm() const { return term; }
  inline C lc() const { return term.coeff; }
  inline E lm() const { return term.exp; }
  Polynomial<C, E> operator+(const Polynomial<C, E>& other) const;
  Polynomial<C, E>& operator+=(const Polynomial<C, E>& other);
  Polynomial<C, E> operator*(const Polynomial<C, E>& other) const;
  Polynomial<C, E>& operator*=(const Polynomial<C, E>& other);
  Polynomial<C, E>& operator*=(const C& c);
  bool operator==(const Polynomial<C, E>& other) const;
private:
  Polynomial* next;
  Term<C, E> term;
};
