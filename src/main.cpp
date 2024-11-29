#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <string>

#include "melon/game.hpp"
#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"
#include "melon/traits.hpp"

namespace {

using namespace melon;

constexpr std::array WHITE_ICONS = {
  "\u2654",
  "\u2655",
  "\u2656",
  "\u2657",
  "\u2658",
  "\u2659",
};

constexpr std::array BLACK_ICONS = {
  "\u265A",
  "\u265B",
  "\u265C",
  "\u265D",
  "\u265E",
  "\u265F",
};

std::string icon(const Piece& piece) {
  const auto& icon_set = piece.team() == 1 ? WHITE_ICONS : BLACK_ICONS;
  const auto id = piece.id();
  if (id == EMPTY_ID)
    return "\u00b7";  // empty square -> central dot
  if (id < icon_set.size())
    return icon_set[id];
  return "?";  // unrecongized Piece
}

std::string text(const Piece& piece) { return std::format("{}:{}", piece.id(), piece.team()); }

std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true) {
  auto [m, n] = board.shape();
  std::string result;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      const auto& piece = board[i, j];
      result.append(use_icons ? icon(piece) : text(piece));
      result.push_back(' ');
    }
    result.back() = '\n';
  }
  return result;
}

}  // namespace

int main() {
  melon::Game game;
  std::cout << serialize(game.board(), true);
  melon::Traits::db();
}
