/*
 * Defines Scalar concept and Vector struct supporting the usual matrix algebra
 * operations
 */
#ifndef MELON_MATH_VECTOR_HPP_
#define MELON_MATH_VECTOR_HPP_

#include <concepts>

namespace melon::math {

// clang-format off
// for some reason, clang-format does { a* b }, which is horrific
template <typename T>
concept Scalar = requires(T a, T b) {
  { a + b } -> std::same_as<T>;
  { a - b } -> std::same_as<T>;
  { a * b } -> std::same_as<T>;
  { a / b } -> std::same_as<T>;
};
// clang-format on

template <Scalar T>
struct Vector {
  T x;
  T y;
};

template <Scalar T>
Vector<T> operator+(const Vector<T>& left, const Vector<T>& right) {
  return {left.x + right.x, left.y + right.y};
}

template <Scalar T>
Vector<T> operator-(const Vector<T>& left, const Vector<T>& right) {
  return {left.x - right.x, left.y - right.y};
}

template <Scalar T>
Vector<T> operator*(T scalar, const Vector<T>& vector) {
  return {scalar * vector.x, scalar * vector.y};
}

template <Scalar T>
Vector<T> operator*(const Vector<T>& vector, T scalar) {
  return scalar * vector;
}

template <Scalar T>
Vector<T>& operator+=(Vector<T>& left, const Vector<T>& right) {
  left.x += right.x;
  left.y += right.y;
  return left;
}

template <Scalar T>
bool operator==(const Vector<T>& left, const Vector<T>& right) {
  return left.x == right.x and left.y == right.y;
}

}  // namespace melon::math

#endif
