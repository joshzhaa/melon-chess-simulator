/*
 * Matrix provides 2D array functionality, not really a matrix in the linear algebra sense
 */
#ifndef MELON_MATH_MATRIX_H_
#define MELON_MATH_MATRIX_H_

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
};

// trivial deduction guide silences compiler warning
template <typename T>
Matrix(Matrix<T>) -> Matrix<T>;

}  // namespace melon::math

#endif
