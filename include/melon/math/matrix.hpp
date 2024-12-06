/*
 * Matrix provides 2D array functionality, not really a matrix in the linear algebra sense
 */
#ifndef MELON_MATH_MATRIX_HPP_
#define MELON_MATH_MATRIX_HPP_

#include <cassert>
#include <functional>
#include <optional>
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
  explicit Matrix(
    std::tuple<size_t, size_t> shape,  // (m, n)
    T element = T{}                    // initial value of all elements
  ) noexcept
      : elements{std::get<0>(shape), std::vector<T>(std::get<1>(shape), element)} {}

  // apparently vector<bool> causes this operator to return a reference to a temporary
  [[nodiscard]] T& operator[](std::size_t i, std::size_t j) noexcept { return elements[i][j]; }
  [[nodiscard]] const T& operator[](std::size_t i, std::size_t j) const noexcept { return elements[i][j]; }
  /*
   * bounds checked indexing, returns optional copy
   * needs to take a signed integer to bounds check negative values
   */
  std::optional<T> at(int i, int j) const noexcept {
    if (i < 0 or j < 0) return std::nullopt;
    // x and y are now guaranteed nonnegative
    auto [m, n] = shape();
    auto iu = static_cast<std::size_t>(i);
    auto ju = static_cast<std::size_t>(j);
    if (iu > n or ju > m) return std::nullopt;
    return (*this)[iu, ju];
  }

  /*
   * returns (m, n)
   */
  [[nodiscard]] auto shape() const noexcept -> std::tuple<std::size_t, std::size_t> {
    if (elements.empty()) return {0, 0};  // prevents UB on the next line
    return {elements.size(), elements[0].size()};
  }

  [[nodiscard]] bool operator==(const Matrix& other) const {
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

template <typename T, BinaryOp<T> Fn>
Matrix<T> elementwise(const Matrix<T>& left, const Matrix<T> right, Fn f) {
  assert(left.shape() == right.shape());  // potential UB if left and right are different shape
  auto [m, n] = left.shape();
  Matrix<T> result{{m, n}};
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      result[i, j] = f(left[i, j], right[i, j]);
    }
  }
  return result;
}

}  // namespace melon::math

#endif
