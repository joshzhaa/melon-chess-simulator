/*
 * Matrix provides 2D array functionality, not really a matrix in the linear algebra sense
 */
#ifndef MELON_MATH_MATRIX_HPP_
#define MELON_MATH_MATRIX_HPP_

#include <cassert>
#include <functional>
#include <initializer_list>
#include <optional>
#include <vector>

#include "vector.hpp"

namespace melon::math {

template <typename T>
class Matrix {
  std::vector<std::vector<T>> elements;

public:
  /*
   * passing element by value, because you're about to copy T at least m*n times anyway
   * using explicit to prevent Matrix<T> m = {1, 2} which looks misleading for a Matrix
   */
  // clang-format off
  explicit Matrix(
    std::tuple<size_t, size_t> shape,  // (m, n)
    T element = T{}                    // initial value of all elements
  ) noexcept : elements{std::get<0>(shape), std::vector<T>(std::get<1>(shape), element)} {}
  // clang-format on

  Matrix() = default;
  Matrix(const Matrix& other) = default;
  Matrix& operator=(const Matrix& other) = default;
  Matrix(Matrix&& other) = default;
  Matrix& operator=(Matrix&& other) = default;
  ~Matrix() = default;

  /*
   * internally, matrices are stored "upside down" so they can be indexed by [y, x]
   * but, the initializer_list is taken to be "right side up"
   * no ctor for intializer_list is defined because it clobbers the general {} syntax
   * since Matrix will often hold numbers, this is to be avoided
   * the cost in this case is that user code must call operator= on another line from the declaration
   */
  Matrix& operator=(std::initializer_list<std::initializer_list<T>> list) noexcept {
    elements = std::vector<std::vector<T>>();  // clear current elements (and its capacity)
    std::size_t const m = list.size();
    elements.reserve(m);
    // no operator[] on std::initializer_list for some reason, so usage of pointer arithmetic is forced
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    for (const std::initializer_list<T>* it = list.end(); it > list.begin(); --it) {
      const auto* i = it - 1;
      std::vector<T> row;
      row.reserve(i->size());
      for (const T* j = i->begin(); j < i->end(); ++j) {
        row.push_back(*j);
      }
      elements.push_back(std::move(row));
    }
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    return *this;
  }

  // apparently vector<bool> causes this operator to return a reference to a temporary
  [[nodiscard]] T& operator[](std::size_t i, std::size_t j) noexcept { return elements[i][j]; }
  [[nodiscard]] const T& operator[](std::size_t i, std::size_t j) const noexcept { return elements[i][j]; }
  // Vector indexing assumes y == i and x == j
  [[nodiscard]] T& operator[](const Vector<int>& v) noexcept { return elements[static_cast<unsigned>(v.y)][static_cast<unsigned>(v.x)]; }
  [[nodiscard]] const T& operator[](const Vector<int>& v) const noexcept { return elements[static_cast<unsigned>(v.y)][static_cast<unsigned>(v.x)]; }

  /*
   * bounds checked indexing, returns optional copy
   * needs to take a signed integer to bounds check negative values
   */
  [[nodiscard]] std::optional<T> at(const Vector<int> v) const noexcept {
    if (v.x < 0 or v.y < 0) return std::nullopt;
    // x and y are now guaranteed nonnegative
    auto [m, n] = shape();
    auto x = static_cast<std::size_t>(v.x);
    auto y = static_cast<std::size_t>(v.y);
    if (x >= n or y >= m) return std::nullopt;
    return (*this)[y, x];
  }

  /*
   * returns (m, n)
   */
  [[nodiscard]] auto shape() const noexcept -> std::tuple<std::size_t, std::size_t> {
    if (elements.empty()) return {0, 0};  // prevents UB on the next line
    return {elements.size(), elements[0].size()};
  }

  // operator!= auto generated from this
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
Matrix<T> elementwise(const Matrix<T>& left, const Matrix<T>& right, Fn f) {
  assert(left.shape() == right.shape());  // potential UB if left and right are different shape
  auto [m, n] = left.shape();
  Matrix<T> result{
    {m, n}
  };
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      result[i, j] = f(left[i, j], right[i, j]);
    }
  }
  return result;
}

}  // namespace melon::math

#endif
