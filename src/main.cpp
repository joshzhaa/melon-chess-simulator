#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "melon/game.hpp"
#include "melon/math/matrix.hpp"
#include "melon/piece.hpp"
#include "melon/standard_pieces.hpp"

using json = nlohmann::json;

namespace {

/*
 * Serialize a board state into a string
 */
std::string serialize(const melon::math::Matrix<melon::Piece>& board) {
  static std::array icons = {
    "\u2654",  // white
    "\u2655",
    "\u2656",
    "\u2657",
    "\u2658",
    "\u2659",
    "\u265A",  // black
    "\u265B",
    "\u265C",
    "\u265D",
    "\u265E",
    "\u265F",
  };
  auto [m, n] = board.shape();
  std::string result;

  for (std::size_t j = 0; j < n; ++j) {
    result.append(" ___");
  }
  result.push_back('\n');
  for (std::size_t i = 0; i < m; ++i) {
    auto print_padding_line = [&](char pad) {
      for (std::size_t j = 0; j < n; ++j) {
        result.push_back('|');
        result.push_back(pad);
        result.push_back(pad);
        result.push_back(pad);
      }
      result.append("|\n");
    };

    auto print_center_line = [&]() {
      for (std::size_t j = 0; j < n; ++j) {
        result.append("| ");

        auto id = board[i, j].id();
        if (id != 6)
          result.append(icons[id]);
        else
          result.push_back(' ');

        result.push_back(' ');
      }
      result.append("|\n");
    };

    // print_horizontal_line();
    print_padding_line(' ');
    print_center_line();
    print_padding_line('_');
  }
  return result;
}

}  // namespace

int main() {
  // json bishop = json::parse(melon::standard_pieces::B);
  // std::cout << bishop << '\n';
  // melon::Game game;
  // std::cout << serialize(game.board());
  
}
