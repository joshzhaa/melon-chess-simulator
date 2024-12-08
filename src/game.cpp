#include "melon/game.hpp"

#include <array>
#include <cstddef>
#include <utility>

#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"
#include "melon/util.hpp"

// parallel arrays describing default chess board configuration
// these arrays imply an orientation for x y axes
namespace {

constexpr std::size_t N = 8;

constexpr std::array<std::array<melon::byte, N>, N> DEFAULT_PIECES{
  {
   {3, 5, 4, 2, 1, 4, 5, 3},
   {6, 6, 6, 6, 6, 6, 6, 6},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {6, 6, 6, 6, 6, 6, 6, 6},
   {3, 5, 4, 2, 1, 4, 5, 3},
   }
};

constexpr std::array<std::array<melon::byte, N>, N> DEFAULT_TEAMS{
  {
   {1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0},
   {2, 2, 2, 2, 2, 2, 2, 2},
   {2, 2, 2, 2, 2, 2, 2, 2},
   }
};

}  // namespace

namespace melon {
// clang-format off
Game::Game() noexcept : moves{{N, N}} {
//clang-format on
  math::Matrix<Piece> board{
    {N, N},
    Piece{0, 0}
  };
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
