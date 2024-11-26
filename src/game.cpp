#include "melon/game.hpp"

#include <format>
#include <iostream>

melon::Game::Game() noexcept {
  boards.emplace_back(3, 3, melon::Piece('p'));
  std::cout << std::format(
      "Game memory layout\n"
      "vector<Matrix<Piece>> {}\n"
      "optional<Vector<int>> {}\n",
      sizeof(boards), sizeof(select));

  melon::math::Matrix<melon::Piece> board{8, 8, melon::Piece(6)};
  for (std::size_t i = 0; i < 8; ++i) {
    board[1, i] = melon::Piece(5);
    board[6, i] = melon::Piece(5);
  }
  for (auto i : std::array<std::size_t, 2>{0, 7}) {
    board[i, 0] = melon::Piece(2);  // R
    board[i, 1] = melon::Piece(4);  // N
    board[i, 2] = melon::Piece(3);  // B
    board[i, 3] = melon::Piece(1);  // Q
    board[i, 4] = melon::Piece(0);  // K
    board[i, 5] = melon::Piece(3);  // B
    board[i, 6] = melon::Piece(4);  // N
    board[i, 7] = melon::Piece(2);  // R
  }
  boards.push_back(std::move(board));
}
