#include "melon/game.hpp"

#include <format>
#include <iostream>

// parallel arrays describing default chess board configuration
// these arrays imply an orientation for x y axes
namespace {

constexpr std::size_t N = 8;

constexpr std::array<std::array<unsigned char, N>, N> DEFAULT_PIECES{{
  {2, 4, 3, 1, 0, 3, 4, 2},
  {5, 5, 5, 5, 5, 5, 5, 5},
  {6, 6, 6, 6, 6, 6, 6, 6},
  {6, 6, 6, 6, 6, 6, 6, 6},
  {6, 6, 6, 6, 6, 6, 6, 6},
  {6, 6, 6, 6, 6, 6, 6, 6},
  {5, 5, 5, 5, 5, 5, 5, 5},
  {2, 4, 3, 1, 0, 3, 4, 2},
}};

constexpr std::array<std::array<unsigned char, N>, N> DEFAULT_TEAMS{{
  {2, 2, 2, 2, 2, 2, 2, 2},
  {2, 2, 2, 2, 2, 2, 2, 2},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
}};

}  // namespace

namespace melon {

Game::Game() noexcept {
  // TODO: remove
  std::cout << std::format(
    "Game {}\n"
    "vector<Matrix<Piece>> {}\n"
    "optional<Vector<int>> {}\n",
    sizeof(*this),
    sizeof(boards),
    sizeof(select)
  );

  math::Matrix<Piece> board{N, N, Piece{6, 0}};  // empty board
  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) {
      auto piece = DEFAULT_PIECES[i][j];
      auto team = DEFAULT_TEAMS[i][j];
      board[i, j] = Piece{piece, team};
    }
  }
  boards.push_back(std::move(board));
}

}  // namespace melon
