#include "melon/game.hpp"

#include <array>
#include <cstddef>
#include <utility>

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"

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
Game::Game() noexcept : mask{{N, N}}, teams{2} {
  // clang-format on
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

void Game::touch(math::Vector<int> square) noexcept {
  auto piece = board().at(square.y, square.x);
  if (not piece) {
    select = std::nullopt;
    return;
  }
  if (mode() == Mode::SELECT) {
    auto move_matrix = piece->matrix(Piece::MatrixType::MOVE, {square, &board()});
    auto attack_matrix = piece->matrix(Piece::MatrixType::ATTACK, {square, &board()});
    auto [m, n] = board().shape();
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        bool can_move = static_cast<bool>(move_matrix[i, j]);
        bool can_attack = static_cast<bool>(attack_matrix[i, j])  // due to vector<bool> limitations
          and board()[i, j].id() != 0                             // non-empty
          and board()[i, j].team() != piece->team();              // non-ally
        mask[i, j] = static_cast<byte>(can_move or can_attack);
      }
    }
    select = square;
  } else {  // mode() == Mode::MOVE
    if (  // if square has been flagged as being able to be moved to
      auto is_flagged = mask.at(square.y, square.x);
      is_flagged and static_cast<bool>(*is_flagged)
    ) {
      math::Vector<std::size_t> from{static_cast<std::size_t>(select->x), static_cast<std::size_t>(select->y)};
      math::Vector<std::size_t> to{static_cast<std::size_t>(square.x), static_cast<std::size_t>(square.y)};
      board()[to.y, to.x] = board()[from.y, from.x]; // TODO: move if Piece becomes non-trivial
      board()[from.y, from.x].destruct();
      // TODO: trigger effects
      board()[to.y, to.x].move();
    }
    select = std::nullopt;
  }
}

void Game::play(const std::vector<math::Vector<int>>& inputs) noexcept {
  for (const math::Vector<int>& input : inputs) touch(input);
}

}  // namespace melon
