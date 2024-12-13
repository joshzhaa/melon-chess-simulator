#ifndef MELON_GAME_HPP_
#define MELON_GAME_HPP_

#include <optional>

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"

namespace melon {

enum class Mode : byte { SELECT, MOVE };

/*
 * class that defines game state
 * some chess rules require the history of the game to be considered
 * so the entire history of boards is part of current game state
 *
 * for user interaction, there are 2 modes: "select" and "move"
 * first the user selects a piece to move, then chooses a square to move to
 *
 * invariant: boards.size() > 0
 */
class Game {
  std::vector<math::Matrix<Piece>> boards;  // history of board states
  math::Matrix<byte> mask;                  // value only meaningful in Mode::MOVE, byte to avoid vector<bool>
  std::optional<math::Vector<int>> select;  // null -> no selection ("select mode")
  int padding{};                            // TODO: remove

public:
  // Constructs Game at initial state of standard chess game
  Game() noexcept;
  // the current board is the last Matrix in boards
  [[nodiscard]] auto board() noexcept -> melon::math::Matrix<melon::Piece>& { return boards.back(); }
  [[nodiscard]] auto board() const noexcept -> const melon::math::Matrix<melon::Piece>& { return boards.back(); }

  // SELECT -> expecting a piece selection, MOVE -> expecting a move selection
  [[nodiscard]] Mode mode() const noexcept { return select ? Mode::MOVE : Mode::SELECT; }
  [[nodiscard]] std::size_t ply_count() const noexcept { return boards.size(); }

  /*
   * interact with a square
   * touch out of bounds -> resets to Mode::SELECT
   * touch if Mode::SELECT -> marks this->mask with possible moves for board[this->select] to take
   * touch if Mode::MOVE -> moves piece at this->select if marked in this->mask, resets to Mode::SELECT
   */
  void touch(math::Vector<int> square) noexcept;

  // touch a series of squares
  void play(const std::vector<math::Vector<int>>& inputs) noexcept;
};

}  // namespace melon

#endif
