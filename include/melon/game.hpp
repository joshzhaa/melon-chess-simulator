#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"
#include "melon/util.hpp"

namespace melon {

enum class Mode : byte { SELECT, MOVE };

/*
 * class that defines game state
 * some chess rules require the history of the game to be considered
 * so the entire history of boards is part of current game state
 *
 * for user interaction, there are 2 modes: "select" and "move"
 * first the user selects a piece to move, then chooses a square to move to
 */
class Game {
  std::vector<math::Matrix<Piece>> boards;  // history of board states
  math::Matrix<bool> moves;                 // value only meaningful in Mode::MOVE
  std::optional<math::Vector<int>> select;  // null -> no selection ("select mode")

public:
  // Constructs Game at initial state of standard chess game
  Game() noexcept;
  // the current board is the last Matrix in boards
  auto board() noexcept -> melon::math::Matrix<melon::Piece>& { return boards.back(); }
  auto board() const noexcept -> const melon::math::Matrix<melon::Piece>& { return boards.back(); }
  // SELECT -> expecting a piece selection, MOVE -> expecting a move selection
  Mode mode() const noexcept { return select ? Mode::MOVE : Mode::SELECT; }
  // highlight squares that the selected piece can move to
  bool highlight(std::size_t i, std::size_t j) const noexcept { return moves[i, j]; }
  void touch() noexcept;
};

}  // namespace melon
