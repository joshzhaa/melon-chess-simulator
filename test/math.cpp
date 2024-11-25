#include <gtest/gtest.h>
#include <melon/math/vector.hpp>
#include <melon/math/matrix.hpp>
#include <tuple>

TEST(Math, Basic) {
  melon::math::Vector<int> u{ 1, 2 };
  melon::math::Vector<int> v{ 2, 3 };
  melon::math::Vector<int> result{ 3, 5 };
  EXPECT_EQ(u + v, result);
  
  std::size_t m = 4, n = 5;
  melon::math::Matrix<int> matrix{m, n};
  
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      matrix[i, j] = i + j;
      EXPECT_EQ((matrix[i, j]), i + j);
    }
  }

  EXPECT_STRNE("hello", "world");
}

TEST(Math, Edge) {
  melon::math::Matrix<int> matrix{0, 0};
  EXPECT_EQ(matrix.shape(), (std::tuple<int, int>{0, 0}));
  
}
