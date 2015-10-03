#ifndef OPERATORS_H
#define OPERATORS_H

template<class A, class B>
bool operator!=(const A& a, const B& b) {
    return !(a == b);
}

template<class A, class B>
bool operator>(const A& a, const B& b) {
  return b < a;
}

template<class A, class B>
bool operator<=(const A& a, const B& b) {
  return !(a > b);
}

template<class A, class B>
bool operator>=(const A& a, const B& b) {
  return !(a < b);
}

#endif // OPERATORS_H
