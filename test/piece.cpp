#include "melon/piece.hpp"

#include <gtest/gtest.h>

#include <cstddef>

#include "melon/math/matrix.hpp"
#include "melon/util.hpp"

using namespace melon;

TEST(Piece, Queen) {
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
  const Piece::Place pos{
    .xy = {.x = X, .y = Y},
    .board = &board,
  };
  // check move matrix from single Q
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
  // add friendly pieces to block Q
  board[N - 1, 0] = Piece{3, 1};
  board[N - 2, 3] = Piece{3, 1};
  // move matrix cannot capture friendly pieces
  matrix = queen.matrix(Piece::MatrixType::MOVE, pos);
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
  // attack matrix can't capture friendlies either
  matrix = queen.matrix(Piece::MatrixType::ATTACK, pos);
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
  // but attack matrix can capture enemy pieces
  board[N - 1, 0] = Piece{3, 2};
  board[N - 2, 3] = Piece{3, 2};
  matrix = queen.matrix(Piece::MatrixType::ATTACK, pos);
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

TEST(Piece, Pawn) {
  constexpr std::size_t PAWN_ID = 6;
  constexpr std::size_t N = 4;
  constexpr int X = 1;
  constexpr int Y = 0;
  math::Matrix<Piece> board{
    {N, N},
    Piece{0, 0}
  };
  board[Y, X] = Piece{PAWN_ID, 1};
  const Piece& white_pawn = board[Y, X];
  const Piece::Place pos{
    .xy = {.x = X, .y = Y},
    .board = &board,
  };
  auto matrix = white_pawn.matrix(Piece::MatrixType::MOVE, pos);
  math::Matrix<byte> expected;
  expected = {
    {False, False, False, False},
    {False, False, False, False},
    {False, True,  False, False},
    {False, False, False, False},
  };
  EXPECT_EQ(matrix, expected);

  board[Y + 1, X] = Piece{2, 1};
  matrix = white_pawn.matrix(Piece::MatrixType::MOVE, pos);
  expected = {
    {False, False, False, False},
    {False, False, False, False},
    {False, False, False, False},
    {False, False, False, False},
  };
  EXPECT_EQ(matrix, expected);

  matrix = white_pawn.matrix(Piece::MatrixType::ATTACK, pos);
  expected = {
    {False, False, False, False},
    {False, False, False, False},
    {True,  False, True,  False},
    {False, False, False, False},
  };
  EXPECT_EQ(matrix, expected);

  board[Y + 1, X] = Piece{0, 0};
  matrix = white_pawn.matrix(Piece::MatrixType::ATTACK, pos);
  EXPECT_EQ(matrix, expected);

  board[N - 1, X + 1] = Piece{PAWN_ID, 2};
  const Piece& black_pawn = board[N - 1, X + 1];
  matrix = black_pawn.matrix(
    Piece::MatrixType::ATTACK,
    Piece::Place{
      .xy = {.x = X + 1, .y = N - 1},
      .board = &board,
  }
  );
  expected = {
    {False, False, False, False},
    {False, True,  False, True },
    {False, False, False, False},
    {False, False, False, False},
  };
  EXPECT_EQ(matrix, expected);
  // TODO: add an en passant test
  // TODO: add a double move test
}
