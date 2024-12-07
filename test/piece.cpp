#include "melon/piece.hpp"
#include "melon/cli/text_io.hpp"
#include "melon/math/matrix.hpp"

#include <gtest/gtest.h>
#include <print>

using namespace melon;

namespace {

template<typename T>
void print(const math::Matrix<T>& matrix) {
  auto [m, n] = matrix.shape();
  std::println("matrix ({}, {})", m, n);
  for (std::size_t i = m; i > 0; --i) {  // i is unsigned -> can't be < 0
    for (std::size_t j = 0; j < n; ++j) {
      std::print("{} ", matrix[i - 1, j]);
    }
    std::print("\n");
  }
  std::print("\n");
}

}  // namespace

TEST(Piece, BasicMove) {
  constexpr std::size_t N = 8;
  constexpr int X = 3;
  constexpr int Y = 4;

  math::Matrix<Piece> board{{N, N}, Piece{0, 0}};
  board[Y, X] = Piece{2, 1};
  std::println("{}", text_io::serialize(board));

  math::Vector<int> pos{X, Y};
  auto move_matrix = board[Y, X].matrix(Piece::MatrixType::MOVE, Piece::Position{pos, &board});
  print(move_matrix);
  math::Matrix<byte> expected;
  expected = {
    {True, False, False, True, False, False, True, False},
    {False, True, False, True, False, True, False, False},
    {False, False, True, True, True, False, False, False},
    {True, True, True, False, True, True, True, True},
    {False, False, True, True, True, False, False, False},
    {False, True, False, True, False, True, False, False},
    {True, False, False, True, False, False, True, False},
    {False, False, False, True, False, False, False, True}
  };
  print(expected);
  EXPECT_TRUE(move_matrix == expected);
}
