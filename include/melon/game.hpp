#ifndef MELON_GAME_HPP_
#define MELON_GAME_HPP_

#include <optional>

#include "melon/byte.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"

namespace melon {

using Move = std::tuple<math::Vector<int>, math::Vector<int>>;

/*
 * class that defines game state
 * some chess rules require the history of the game to be considered
 * so the entire history of boards is part of current game state
 *
 * for user interaction, there are 2 modes: "select" and "move"
 * first the user selects a piece to move, then chooses a square to move to
 *
 * invariant: boards.size() > 0, boards.size() == moves.size()
 * game only modifies boards.back() (after pushing a copy of previous board)
 */
class Game {                                // NOLINT(*-padded)
  std::vector<math::Matrix<Piece>> boards;  // history of board states
  std::vector<Move> moves;                  // history of piece moves
  math::Matrix<byte> mask;                  // value only meaningful in Mode::MOVE, byte to avoid vector<bool>
  std::optional<math::Vector<int>> select;  // null -> no selection ("select mode")
  byte teams{};                             // number of teams in this game

  // helpers for this->touch()
  void handle_select(const math::Vector<int>& square);
  void handle_move(const math::Vector<int>& square);
  void trigger_effects(const math::Vector<int>& from, const math::Vector<int>& to);

public:
  // Constructs Game at initial state of standard chess game
  Game() noexcept;
  // the current board is the last Matrix in boards
  [[nodiscard]] auto board() noexcept -> math::Matrix<Piece>& { return boards.back(); }
  [[nodiscard]] auto board() const noexcept -> const math::Matrix<Piece>& { return boards.back(); }
  [[nodiscard]] auto move_mask() const noexcept -> const math::Matrix<byte>& { return mask; }
  [[nodiscard]] auto move_history() const noexcept -> const std::vector<Move>& { return moves; }
  [[nodiscard]] auto board_history() const noexcept -> const std::vector<math::Matrix<Piece>>& { return boards; }

  enum class Mode : byte { SELECT, MOVE };
  // SELECT -> expecting a piece selection, MOVE -> expecting a move selection
  [[nodiscard]] Mode mode() const noexcept { return select ? Mode::MOVE : Mode::SELECT; }
  [[nodiscard]] std::size_t ply() const noexcept { return boards.size(); }
  // 1, 2, 3, ..., teams, 1, 2, 3, ...
  [[nodiscard]] byte turn() const noexcept { return 1 + ((ply() - 1) % teams); }

  /*
   * interact with a square
   * touch out of bounds -> resets to Mode::SELECT
   * touch if Mode::SELECT -> marks this->mask with possible moves for board[this->select] to take
   * touch if Mode::MOVE -> moves piece at this->select if marked in this->mask, resets to Mode::SELECT
   */
  void touch(const math::Vector<int>& square) noexcept;
};

}  // namespace melon

#endif
