#include "melon/piece.hpp"
#include "melon/cli/text_io.hpp"
#include "melon/math/matrix.hpp"

#include <gtest/gtest.h>
#include <print>

using namespace melon;

TEST(Piece, Moves) {
  constexpr std::size_t N = 8;
  math::Matrix<Piece> board{{N, N}, Piece{0, 0}};
  board[4, 4]= Piece{2, 1};
  std::println("{}", text_io::serialize(board));
  EXPECT_EQ(4, 5);
}
