#include "melon/cli/text_io.hpp"

#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"
#include <array>
#include <cstddef>
#include <format>
#include <string>

namespace {

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

}  // namespace

namespace melon::text_io {

std::string icon(const Piece& piece) noexcept {
  const auto& icon_set = piece.team() == 1 ? WHITE_ICONS : BLACK_ICONS;
  const auto id = piece.id();
  if (id == 0) return "\u00b7";  // empty square -> central dot
  if (id <= icon_set.size()) return icon_set[id - 1];
  return "?";  // unrecongized Piece
}

std::string text(const Piece& piece) noexcept {  // NOLINT(bugprone-exception-escape)
  return std::format("{}:{}", piece.id(), piece.team()); 
}

std::string serialize(const math::Matrix<Piece>& board, bool use_icons) noexcept {
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

}  // namespace melon::text_io
