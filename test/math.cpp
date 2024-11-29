#include <gtest/gtest.h>

#include <functional>
#include <tuple>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"

using namespace melon::math;

TEST(Math, Basic) {
  Vector<int> u{1, 2};
  Vector<int> v{2, 3};
  Vector<int> result{3, 5};
  EXPECT_EQ(u + v, result);

  std::size_t m = 4, n = 5;
  Matrix<int> matrix{m, n};

  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      matrix[i, j] = static_cast<int>(i + j);
      EXPECT_EQ((matrix[i, j]), i + j);
    }
  }

  EXPECT_STRNE("hello", "world");
}

TEST(Math, Edge) {
  Matrix<int> matrix{0, 0};
  EXPECT_EQ(matrix.shape(), (std::tuple<int, int>{0, 0}));
}

TEST(Math, Elementwise) {
  unsigned m = 4;
  unsigned n = 4;
  Matrix<unsigned> a{m, n};
  Matrix<unsigned> b{m, n};
  for (unsigned i = 0; i < m; ++i) {
    for (unsigned j = 0; j < n; ++j) {
      a[i, j] = 2 * (i + j);
      b[i, j] = 3 * (i + j);
    }
  }
  EXPECT_EQ(a, a);
  EXPECT_EQ(b, b);

  auto sum = elementwise(a, b, std::plus<unsigned>());
  // expect compile error
  // from concept
  // auto result = elementwise(a, b, [](unsigned i, unsigned j) { return 1.0 });
  for (unsigned i = 0; i < m; ++i) {
    for (unsigned j = 0; j < n; ++j) {
      EXPECT_EQ((sum[i, j]), 5 * (i + j));
    }
  }
}
