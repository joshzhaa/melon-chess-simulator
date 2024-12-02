#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "melon/constants.hpp"
#include "melon/game.hpp"
#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"
#include "melon/traits.hpp"
#include "melon/traits_json.hpp"

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
  if (id == 0) return "\u00b7";  // empty square -> central dot
  if (id <= icon_set.size()) return icon_set[id - 1];
  return "?";  // unrecongized Piece
}

std::string text(const Piece& piece) { return std::format("{}:{}", piece.id(), piece.team()); }

std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true) {
  auto [m, n] = board.shape();
  std::string result;
  for (std::size_t i = m; i > 0; --i) {  // i is unsigned -> can't be < 0
    for (std::size_t j = 0; j < n; ++j) {
      const auto& piece = board[i - 1, j];
      result.append(use_icons ? icon(piece) : text(piece));
      result.push_back(' ');
    }
    result.back() = '\n';
  }
  return result;
}

}  // namespace

int main() {
  Game game;
  std::cout << serialize(game.board(), true);

  for (std::size_t piece_id = 0; piece_id < constants::STANDARD_TRAITS.size(); ++piece_id) {
    std::cout << std::format("piece_id={}", piece_id) << '\n';
    json json = Traits::db()[piece_id];
    std::cout << json << '\n';
  }
}
