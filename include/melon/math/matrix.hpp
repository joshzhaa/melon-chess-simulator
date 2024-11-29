/*
 * Matrix provides 2D array functionality, not really a matrix in the linear algebra sense
 */
#ifndef MELON_MATH_MATRIX_H_
#define MELON_MATH_MATRIX_H_

#include <cassert>
#include <concepts>
#include <functional>
#include <vector>

namespace melon::math {

template <typename T>
class Matrix {
  std::vector<std::vector<T>> elements;

public:
  /*
   * passing element by value, because you're about to copy T at least m*n times anyway
   * using explicit to prevent Matrix<T> m = {1, 2} which looks misleading for a Matrix
   */
  explicit Matrix(std::size_t m, std::size_t n, T element = T{}) noexcept : elements{m, std::vector<T>(n, element)} {}
  Matrix(const Matrix& other) noexcept : elements{other.elements} {}
  Matrix(Matrix&& other) noexcept : elements{std::move(other.elements)} {}

  T& operator[](std::size_t i, std::size_t j) noexcept { return elements[i][j]; }
  const T& operator[](std::size_t i, std::size_t j) const noexcept { return elements[i][j]; }

  /*
   * returns (m, n)
   */
  auto shape() const noexcept -> std::tuple<std::size_t, std::size_t> {
    if (elements.empty()) return {0, 0};  // prevents UB on the next line
    return {elements.size(), elements[0].size()};
  }

  bool operator==(const Matrix& other) const {
    if (this->shape() != other.shape()) return false;
    auto [m, n] = this->shape();
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        if ((*this)[i, j] != other[i, j]) return false;
      }
    }
    return true;
  }
};

// trivial deduction guide silences compiler warning
template <typename T>
Matrix(Matrix<T>) -> Matrix<T>;

template <typename Fn, typename T>
concept BinaryOp = requires(Fn f, T a, T b) {
  { std::invoke(f, a, b) } -> std::same_as<T>;
};

// potential UB if left and right are different shape
template <typename T, BinaryOp<T> Fn>
Matrix<T> elementwise(const Matrix<T>& left, const Matrix<T> right, Fn f) {
  assert(left.shape() == right.shape());
  auto [m, n] = left.shape();
  Matrix<T> result{m, n};
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      result[i, j] = f(left[i, j], right[i, j]);
    }
  }
  return result;
}

}  // namespace melon::math

#endif
