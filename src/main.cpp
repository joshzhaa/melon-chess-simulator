#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "melon/constants.hpp"
#include "melon/game.hpp"
#include "melon/math/matrix.hpp"
#include "melon/math/vector.hpp"
#include "melon/piece.hpp"
#include "melon/traits.hpp"
#include "melon/traits_json.hpp"

namespace {

using melon::Piece;
using melon::math::Matrix;

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

std::string serialize(const Matrix<Piece>& board, bool use_icons = true) {
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

melon::Traits make_bishop() {
  using melon::Shape, melon::Geometry, melon::Traits, melon::math::Vector;

  std::vector<Vector<int>> orientations = {
    {1, 1},
    {1, -1},
    {-1, -1},
    {-1, 1},
  };
  std::vector<Shape> shapes(orientations.size(), Shape::RAY);
  Geometry geometry{std::move(shapes), std::move(orientations)};
  return Traits{
    geometry,
    geometry,
    {},
    {}
  };
}

}  // namespace

int main() {
  using melon::Game, melon::Traits, melon::byte;

  Game game;
  std::cout << sizeof(std::string_view) << '\n';
  std::cout << serialize(game.board(), true);
  
  int i = 0;
  for (std::size_t piece_id = 0; piece_id <= melon::constants::STANDARD_TRAITS.size(); ++piece_id) {
    std::cout << i << '\n';
    json json = Traits::db()[piece_id];
    std::cout << json.dump(2) << '\n';
  }
}
