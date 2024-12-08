#ifndef MELON_CLI_TEXT_IO_HPP
#define MELON_CLI_TEXT_IO_HPP

#include <print>
#include <string>

#include "melon/piece.hpp"

namespace melon::text_io {

std::string icon(const Piece& piece);  // unicode chess icon for piece (id, team)
std::string text(const Piece& piece);  // ascii text representation of piece (id, team)
// text representation of board
std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true);

template <typename T>
void print(const math::Matrix<T>& matrix) {
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

}  // namespace melon::text_io

#endif // MELON_CLI_TEXT_IO_HPP
