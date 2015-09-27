#define VAR_COUNT 2

template<class E = char>
class Exponent {
public:
  Exponent() : exp() {}
  E& operator[](const unsigned int& index) { return exp[index]; }
  const E& operator[](const unsigned int& index) const { return exp[index]; }

  unsigned int degree() {
    unsigned int result = 0;
    unsigned int i;
    for (i = 0; i < VAR_COUNT; ++i) {
      result += exp.exp[i];
    }
    return result;
  }

  Exponent<E>& operator*=(const Exponent<E>& other) {
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      exp[i] += other.exp[i];
    }
    return *this;
  }

  Exponent<E> operator*(const Exponent<E>& other) const {
    Exponent<E> result;
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      result.exp[i] = exp[i] + other.exp[i];
    }
    return result;
  }

  Exponent<E> operator/(const Exponent<E>& other) const {
    Exponent<E> result;
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      //assert(a.exp[i] < b.exp[i]);
      result.exp[i] = exp[i] - other.exp[i];
    }
    return result;
  }

  bool operator<(const Exponent<E>& other) const {
    // lex order for simplicity
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != other[i]) {
        return exp[i] < other[i];
      }
    }
    return false;
  }

  bool operator>(const Exponent<E>& other) const {
    return other < *this;
  }

  bool operator==(const Exponent<E>& other) const {
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != other[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const Exponent<E>& other) const { return !(*this == other); }
  bool isZero() const {
    for (unsigned int i = 0; i < VAR_COUNT; ++i) {
      if (exp[i] != 0) {
        return false;
      }
    }
    return true;
  }
private:
  E exp[VAR_COUNT];
};
