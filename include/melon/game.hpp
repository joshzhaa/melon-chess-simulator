#include <optional>

#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"

namespace melon {

/*
 * class that defines game state
 * some chess rules require the history of the game to be considered
 * so the entire history of boards is part of current game state
 *
 * for user interaction, there are 2 modes: "select" and "move"
 * first the user selects a piece to move, then chooses a square to move to
 */
class Game {
  std::vector<melon::math::Matrix<melon::Piece>> boards;
  std::optional<melon::math::Vector<int>> select;  // null -> no selection ("select mode")

public:
  // TODO: remove
  Game() noexcept;
  auto board() noexcept -> melon::math::Matrix<melon::Piece>& { return boards.back(); }
  auto board() const noexcept -> const melon::math::Matrix<melon::Piece>& { return boards.back(); }
  char mode() const noexcept { return select ? 'm' : 's'; }
};

}  // namespace melon
