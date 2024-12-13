#ifndef MELON_CLI_TEXT_IO_HPP
#define MELON_CLI_TEXT_IO_HPP

#include <format>
#include <iostream>
#include <string>

#include "melon/piece.hpp"

namespace melon::text_io {

std::string icon(const Piece& piece) noexcept;  // unicode chess icon for piece (id, team)
std::string text(const Piece& piece) noexcept;  // ascii text representation of piece (id, team)
// text representation of board
std::string serialize(const math::Matrix<Piece>& board, bool use_icons = true) noexcept;

template <typename T>
void print(const math::Matrix<T>& matrix) noexcept {
  auto [m, n] = matrix.shape();
  std::cout << std::format("matrix ({}, {})\n", m, n);
  for (std::size_t i = m; i > 0; --i) {  // i is unsigned -> can't be < 0
    for (std::size_t j = 0; j < n; ++j) {
      std::cout << std::format("{} ", matrix[i - 1, j]);
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

template <typename T>
void print(const math::Vector<T>& vector) noexcept {
  std::cout << std::format("({}, {})\n",  vector.x, vector.y);
}

}  // namespace melon::text_io

#endif // MELON_CLI_TEXT_IO_HPP
