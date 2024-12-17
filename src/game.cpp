#include "melon/game.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"
#include "melon/traits.hpp"

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

void Game::handle_select(const math::Vector<int>& square) {
  const auto piece = board().at(square);
  // TODO: maybe support a real time chess option (replace team check with empty check)
  if (not piece or piece->team() != this->turn()) return;
  const Piece::Place place{.xy = square, .board = &board(), .move_history = &move_history()};
  const auto move_matrix = piece->matrix(Piece::MatrixType::MOVE, place);
  const auto attack_matrix = piece->matrix(Piece::MatrixType::ATTACK, place);
  const auto [m, n] = board().shape();
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      const bool occupied_by_enemy = board()[i, j].id() != 0 and board()[i, j].team() != piece->team();
      mask[i, j] = occupied_by_enemy ? attack_matrix[i, j] : move_matrix[i, j];
    }
  }
  select = square;
}

void Game::handle_move(const math::Vector<int>& square) {
  assert(select);
  if (  // if square has been flagged as being able to be moved to
    const auto is_flagged = mask.at(square);
    is_flagged and static_cast<bool>(*is_flagged)
  ) {
    // when the if condition is satisfied, the move is guaranteed to succeed
    // store previous board state before modifying
    boards.push_back(board());
    board()[square] = board()[*select];
    board()[*select].destruct();
    // this if checks whether the mask value represents a valid melon::Action value
    if (mask[square] != 0 and mask[square] != 1) trigger_effects(*select, square);
    board()[square].move();
    moves.emplace_back(*select, square);
  }
  select = std::nullopt;
}

void Game::touch(const math::Vector<int>& square) noexcept {
  switch (mode()) {
    case Mode::SELECT:
      handle_select(square);
      break;
    case Mode::MOVE:
      handle_move(square);
      break;
  }
}

namespace {

// helper for trigger_effects
void handle_castle(math::Matrix<Piece>& board, const math::Vector<int>& from, const math::Vector<int>& to) {
  const auto displacement = to - from;
  const math::Vector<int> direction = {.x=displacement.x / 2, .y=displacement.y / 2};  // unit vector
  auto find_rook = [&board, &from, &direction]() {
    auto square = from;
    auto piece = board.at(square);
    while (piece) {
      const auto& effects = Traits::db()[piece->id()].effects;
      if (std::ranges::contains(effects, Effect::CASTLE)) return square;
      square = square + direction;
      piece = board.at(square);
    }
    assert(false);  // not found -> programmer error in Piece::matrix
  };
  const auto rook_square = find_rook();
  // TODO: its unclear to me whether an auto-moved rook should be considered "moved"
  board[to - direction] = board[rook_square];
  board[rook_square].destruct();
}

/*
  * Matrix of squares that are considered "threatened" for a certain team.
  * These are squares that team's king couldn't legally move to.
  * fairly expensive operation, O(m*n) calls to Piece::matrix.
  * What's worse is that it needs to be called once for each possible move
  */
auto threat_matrix(const math::Matrix<Piece>& board, byte team) -> math::Matrix<byte> {
  auto [m, n] = board.shape();
  math::Matrix<byte> result{{m, n}, False};
  // adds more squares to threat matrix (result)
  auto add_squares = [&result, &m, &n](const math::Matrix<byte>& matrix){
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        result[i, j] = result[i, j] or matrix[i, j];  // NOLINT(*implicit-bool-conversion)
      }
    }
  };
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      const auto& piece = board[i, j];
      if (piece.team() == team) continue;
      const Piece::Place place{
        {static_cast<int>(j), static_cast<int>(i)},
        &board, nullptr,  // move_history only needed by en_passant (=MatrixType::MOVE)
      };
      const auto attacks = piece.matrix(Piece::MatrixType::ATTACK, place);
      add_squares(attacks);
    }
  }
  return result;
}

} // namespace

void Game::trigger_effects(const math::Vector<int>& from, const math::Vector<int>& to) {
  assert(mask[to] != 0 and mask[to] != 1);
  // some actions will always trigger an effect
  switch (static_cast<Action>(mask[to])) {
    case Action::EN_PASSANT:
      board()[to - Team::db()[board()[to].team()].facing].destruct();
      break;
    case Action::CASTLE:
      handle_castle(board(), from, to);
      break;
    case Action::DOUBLE_STEP:
      break;  // double step doesn't trigger any effects
  }
}

}  // namespace melon
