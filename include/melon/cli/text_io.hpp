#ifndef MELON_CLI_TEXT_IO_HPP
#define MELON_CLI_TEXT_IO_HPP

#include <print>
#include <string>

#include "melon/piece.hpp"

namespace melon::text_io {

std::string icon(const Piece& piece) noexcept;  // unicode chess icon for piece (id, team)
std::string text(const Piece& piece) noexcept;  // ascii text representation of piece (id, team)
// text representation of board
std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true) noexcept;

// TODO: remove this
template <typename T>
void print(const math::Matrix<T>& matrix) noexcept {  // NOLINT(bugprone-exception-escape)
  auto [m, n] = matrix.shape();
  std::println("matrix ({}, {})", m, n);
  for (std::size_t i = m; i > 0; --i) {  // i is unsigned -> can't be < 0
    for (std::size_t j = 0; j < n; ++j) {
      std::print("{} ", matrix[i - 1, j]);
    }
    std::print("\n");
  }
  std::print("\n");
}

// TODO: remove this
template <typename T>
void print(const math::Vector<T>& vector) noexcept {  // NOLINT(bugprone-exception-escape)
  std::println("({}, {})", vector.x, vector.y);
}

}  // namespace melon::text_io

#endif // MELON_CLI_TEXT_IO_HPP
