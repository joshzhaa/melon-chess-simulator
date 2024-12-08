#include "melon/piece.hpp"

#include <gtest/gtest.h>

#include <cstddef>

#include "melon/math/matrix.hpp"
#include "melon/util.hpp"

using namespace melon;

TEST(Piece, BasicMove) {
  // set up board with single Q in the middle
  constexpr std::size_t N = 8;
  constexpr int X = 3;
  constexpr int Y = 4;
  math::Matrix<Piece> board{
    {N, N},
    Piece{0, 0}
  };
  board[Y, X] = Piece{2, 1};
  const Piece& queen = board[Y, X];
  const Piece::Position pos{
    .xy={.x = X, .y = Y},
    .board=&board
  };
  // check move matrix from single Q
  {
    auto matrix = queen.matrix(Piece::MatrixType::MOVE, pos);
    math::Matrix<byte> expected;
    expected = {
      {True,  False, False, True,  False, False, True,  False},
      {False, True,  False, True,  False, True,  False, False},
      {False, False, True,  True,  True,  False, False, False},
      {True,  True,  True,  False, True,  True,  True,  True },
      {False, False, True,  True,  True,  False, False, False},
      {False, True,  False, True,  False, True,  False, False},
      {True,  False, False, True,  False, False, True,  False},
      {False, False, False, True,  False, False, False, True }
    };
    EXPECT_EQ(matrix, expected);
  }
  // add friendly pieces to block Q
  board[N - 1, 0] = Piece{3, 1};
  board[N - 2, 3] = Piece{3, 1};
  // move matrix cannot capture friendly pieces
  {
    auto matrix = queen.matrix(Piece::MatrixType::MOVE, pos);
    math::Matrix<byte> expected;
    expected = {
      {False, False, False, False, False, False, True,  False},
      {False, True,  False, False, False, True,  False, False},
      {False, False, True,  True,  True,  False, False, False},
      {True,  True,  True,  False, True,  True,  True,  True },
      {False, False, True,  True,  True,  False, False, False},
      {False, True,  False, True,  False, True,  False, False},
      {True,  False, False, True,  False, False, True,  False},
      {False, False, False, True,  False, False, False, True }
    };
    EXPECT_EQ(matrix, expected);
  }
  // attack matrix can't capture friendlies either
  {
    auto matrix = queen.matrix(Piece::MatrixType::ATTACK, pos);
    math::Matrix<byte> expected;
    expected = {
      {False, False, False, False, False, False, True,  False},
      {False, True,  False, False, False, True,  False, False},
      {False, False, True,  True,  True,  False, False, False},
      {True,  True,  True,  False, True,  True,  True,  True },
      {False, False, True,  True,  True,  False, False, False},
      {False, True,  False, True,  False, True,  False, False},
      {True,  False, False, True,  False, False, True,  False},
      {False, False, False, True,  False, False, False, True }
    };
    EXPECT_EQ(matrix, expected);
  }
  // but attack matrix can capture enemy pieces
  board[N - 1, 0] = Piece{3, 2};
  board[N - 2, 3] = Piece{3, 2};
  {
    auto matrix = queen.matrix(Piece::MatrixType::ATTACK, pos);
    math::Matrix<byte> expected;
    expected = {
      {True,  False, False, False, False, False, True,  False},
      {False, True,  False, True,  False, True,  False, False},
      {False, False, True,  True,  True,  False, False, False},
      {True,  True,  True,  False, True,  True,  True,  True },
      {False, False, True,  True,  True,  False, False, False},
      {False, True,  False, True,  False, True,  False, False},
      {True,  False, False, True,  False, False, True,  False},
      {False, False, False, True,  False, False, False, True }
    };
    EXPECT_EQ(matrix, expected);
  }
}
